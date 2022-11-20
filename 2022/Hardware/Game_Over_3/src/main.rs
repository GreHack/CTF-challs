use std::{fmt, sync::Mutex};

use actix_web::{
    get,
    middleware::{self, Logger},
    web, App, HttpServer, Responder,
};
use md5::{Digest, Md5};
use serde::Deserialize;
use std::collections::HashMap;

#[derive(Deserialize, Debug)]
#[serde(try_from = "String")]
enum Action {
    Download,
    Read,
    Write,
}

impl Action {
    pub fn try_new(action: String) -> Result<Self, String> {
        match action.to_lowercase().as_str() {
            "download" => Ok(Action::Download),
            "read" => Ok(Action::Read),
            "write" => Ok(Action::Write),
            _ => Err(format!("{} is not one of the 3 supported actions", action)),
        }
    }
}

impl TryFrom<String> for Action {
    type Error = String;

    fn try_from(value: String) -> Result<Self, Self::Error> {
        Action::try_new(value)
    }
}

#[derive(Deserialize, Debug)]
#[serde(try_from = "String")]
struct ID(String, u8);

impl ID {
    pub fn try_new(id: String) -> Result<Self, String> {
        let lowercase_id_hex = id.to_lowercase();

        for acceptable_value in 1u8..=255 {
            let hash = Md5::new().chain_update([acceptable_value]).finalize();

            if lowercase_id_hex == format!("{:x}", hash) {
                return Ok(Self(lowercase_id_hex, acceptable_value));
            }
        }

        Err(format!("Device {} does not exist", id))
    }
}

impl TryFrom<String> for ID {
    type Error = String;

    fn try_from(value: String) -> Result<Self, Self::Error> {
        ID::try_new(value)
    }
}

impl fmt::Display for ID {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

#[derive(Deserialize, Debug)]
struct Param {
    action: Action,
    id: ID,
    #[serde(default = "default_param_val")]
    val: String,
}

fn default_param_val() -> String {
    "".to_string()
}

#[get("/remote")]
async fn remote(state: web::Data<AppState>, param: web::Query<Param>) -> impl Responder {
    match param.action {
        Action::Download => get_ps_script(param.id.0.clone()),
        Action::Read => read_action(state, param.id.1),
        Action::Write => write_action(state, param.id.1, param.val.clone()),
    }
}

const DOWNLOAD_SCRIPT: &str = include_str!("download.ps1");
fn get_ps_script(id: String) -> String {
    let server_hostname = std::env::var("HOSTNAME").unwrap_or(String::from("127.0.0.1"));
    DOWNLOAD_SCRIPT
        .replace("{server_hostname}", &server_hostname)
        .replace("{id}", &id)
}

fn read_action(state: web::Data<AppState>, device_id: u8) -> String {
    if device_id == 178 {
        return std::env::var("FLAG").unwrap_or(String::from("flag_to_be_set"));
    }
    state
        .values
        .lock()
        .unwrap()
        .get(&device_id)
        .unwrap_or(&String::from(""))
        .clone()
}

fn write_action(state: web::Data<AppState>, device_id: u8, value: String) -> String {
    let mut values = state.values.lock().unwrap();
    values.insert(device_id, value);

    String::from("Done")
}

struct AppState {
    values: Mutex<HashMap<u8, String>>,
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    let map_values = HashMap::from([
        (2, String::from("Hostname=phil\r\nSSID = GreHack22\r\nPASS = new_still_not_always_better\r\nSSID = Cars\r\nPASS = mini")),
        (67, String::from("Hostname=le_suisse_laptop\r\nSSID = GreHack22\r\nPASS = new_still_not_always_better\r\nSSID = AlpacasLand\r\nPASS = alpacas_are_awesome")),
        (149, String::from("Hostname=old\r\nSSID = GreHack19\r\nPASS = new_not_always_better")),
        (245, String::from("Hostname=look_closer\r\nSSID = Are_You_Lost\r\nPASS = too_far"))
    ]);
    let state = web::Data::new(AppState {
        values: Mutex::new(map_values),
    });

    env_logger::init_from_env(env_logger::Env::new().default_filter_or("info"));
    HttpServer::new(move || {
        App::new()
            .app_data(state.clone())
            .wrap(Logger::default())
            .wrap(
                middleware::DefaultHeaders::new()
                    .add(("Content-Type", "text/plain; charset=UTF-8"))
                    .add(("Content-Security-Policy", "default-src 'none'; base-uri 'none'; frame-ancestors 'none'; form-action 'none';"))
                    .add(("Referer-Policy", "no-referrer"))
                    .add(("X-Content-Type-Options", "no-sniff"))
                    .add(("Cross-Origin-Embedder-Policy", "require-corp"))
                    .add(("Cross-Origin-Opener-Policy", "same-origin"))
                    .add(("Cross-Origin-Resource-Policy", "same-site"))
                )
            .service(remote)
    })
    .bind(("0.0.0.0", 8080))?
    .run()
    .await
}
