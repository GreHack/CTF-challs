use actix_multipart_extract::{File, Multipart, MultipartForm};
use actix_web::{
    middleware::{self, Logger},
    post, App, HttpResponse, HttpServer, Responder,
};
use actix_web_static_files::ResourceFiles;
use cap_primitives::ambient_authority;
use rand::distributions::{Alphanumeric, DistString};
use serde::Deserialize;
use std::{path::Path, thread};
use tempfile::tempdir;
use wasi_cap_std_sync::{Dir, WasiCtxBuilder};
use wasi_common::{
    pipe::{ReadPipe, WritePipe},
    WasiCtx,
};
use wasmtime::{Engine, Linker, Module, Store, StoreLimits, StoreLimitsBuilder};

mod wasi_ctx;
use wasi_ctx::build_wasi_ctx;

include!(concat!(env!("OUT_DIR"), "/generated.rs"));

#[derive(Deserialize, MultipartForm, Debug)]
struct ComputeUpload {
    input: String,
    #[multipart(max_size = 5MB)]
    code: File,
}

#[post("/compute")]
async fn compute(data: Multipart<ComputeUpload>) -> impl Responder {
    match exec(data) {
        Ok(content) => HttpResponse::Ok().body(content),
        Err(er) => HttpResponse::BadRequest().body(er.to_string()),
    }
}

#[derive(Deserialize, MultipartForm, Debug)]
struct ComputeFileUpload {
    #[multipart(max_size = 5MB)]
    source: File,
    #[multipart(max_size = 5MB)]
    code: File,
}

#[post("/compute_file")]
async fn compute_file(data: Multipart<ComputeFileUpload>) -> impl Responder {
    match exec_file(data) {
        Ok(content) => HttpResponse::Ok().body(content),
        Err(er) => HttpResponse::BadRequest().body(er.to_string()),
    }
}

struct StoreState {
    wasi: WasiCtx,
    limits: StoreLimits,
}

fn exec(data: Multipart<ComputeUpload>) -> Result<String, Box<dyn std::error::Error>> {
    let engine = Engine::default();

    let stdin = ReadPipe::from(data.input.as_bytes());
    let stdout = WritePipe::new_in_memory();

    let mut linker = Linker::new(&engine);
    wasmtime_wasi::add_to_linker(&mut linker, |s: &mut StoreState| &mut s.wasi)?;
    let mut wasi = build_wasi_ctx();
    wasi.set_stdin(Box::new(stdin));
    wasi.set_stdout(Box::new(stdout.clone()));

    let store_state = StoreState {
        wasi,
        limits: StoreLimitsBuilder::new()
            .memory_size(10 << 20)
            .instances(1)
            .build(),
    };

    {
        let mut store = Store::new(&engine, store_state);
        store.limiter(|s| &mut s.limits);
        store.epoch_deadline_trap();
        store.set_epoch_deadline(1);

        let module = Module::new(&engine, &data.code.bytes)?;

        let engine = store.engine().clone();
        thread::spawn(move || {
            thread::sleep(std::time::Duration::from_millis(500));
            engine.increment_epoch();
        });

        linker.module(&mut store, "", &module)?;

        let instance = linker.instantiate(&mut store, &module)?;

        instance
            .get_typed_func::<(), (), _>(&mut store, "_start")?
            .call(&mut store, ())?;
    };

    let raw_output = stdout
        .try_into_inner()
        .expect("stdout reference still exists")
        .into_inner();

    Ok(String::from_utf8(raw_output).unwrap_or(String::from("Could not get function output")))
}

// If you are looking for good software engineering practices, this is not the place
fn exec_file(data: Multipart<ComputeFileUpload>) -> Result<String, Box<dyn std::error::Error>> {
    let temp_dir = tempdir()?;

    let file_name: String = Alphanumeric.sample_string(&mut rand::thread_rng(), 32);
    std::fs::write(temp_dir.path().join(&file_name), &data.source.bytes)?;

    std::fs::write(temp_dir.path().join("flag"), "GH22{broken_💩_restrictions}")?;

    let engine = Engine::default();
    let guest_path = "/";

    let stdin = ReadPipe::from(
        Path::new(guest_path)
            .join(&file_name)
            .to_string_lossy()
            .into_owned(),
    );
    let stdout = WritePipe::new_in_memory();

    let mut wasi = WasiCtxBuilder::new()
        .stdin(Box::new(stdin))
        .stdout(Box::new(stdout.clone()))
        .preopened_dir(
            Dir::open_ambient_dir(temp_dir.path(), ambient_authority())?,
            guest_path,
        )?
        .build();

    let custom_wasi_context = build_wasi_ctx();
    wasi.clocks = custom_wasi_context.clocks;
    wasi.random = custom_wasi_context.random;
    wasi.sched = custom_wasi_context.sched;

    let mut linker = Linker::new(&engine);
    wasmtime_wasi::add_to_linker(&mut linker, |s: &mut StoreState| &mut s.wasi)?;

    let store_state = StoreState {
        wasi,
        limits: StoreLimitsBuilder::new()
            .memory_size(10 << 20)
            .instances(1)
            .build(),
    };

    {
        let mut store = Store::new(&engine, store_state);
        store.limiter(|s| &mut s.limits);
        store.epoch_deadline_trap();
        store.set_epoch_deadline(1);

        let module = Module::new(&engine, &data.code.bytes)?;

        let engine = store.engine().clone();
        thread::spawn(move || {
            thread::sleep(std::time::Duration::from_millis(500));
            engine.increment_epoch();
        });

        linker.module(&mut store, "", &module)?;

        let instance = linker.instantiate(&mut store, &module)?;

        instance
            .get_typed_func::<(), (), _>(&mut store, "_start")?
            .call(&mut store, ())?;
    };

    let raw_output = stdout
        .try_into_inner()
        .expect("stdout reference still exists")
        .into_inner();

    temp_dir.close()?;

    Ok(String::from_utf8(raw_output).unwrap_or(String::from("Could not get function output")))
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    env_logger::init_from_env(env_logger::Env::new().default_filter_or("info"));
    HttpServer::new(|| {
        let generated = generate();
        App::new()
            .wrap(Logger::default())
            .wrap(
                middleware::DefaultHeaders::new()
                    .add(("Content-Type", "text/plain; charset=UTF-8"))
                    .add(("Content-Security-Policy", "default-src 'none'; style-src 'self'; base-uri 'none'; frame-ancestors 'none'; form-action 'none';"))
                    .add(("Referer-Policy", "no-referrer"))
                    .add(("X-Content-Type-Options", "no-sniff"))
                    .add(("Cross-Origin-Embedder-Policy", "require-corp"))
                    .add(("Cross-Origin-Opener-Policy", "same-origin"))
                    .add(("Cross-Origin-Resource-Policy", "same-site"))
                )
            .service(compute)
            .service(compute_file)
            .service(ResourceFiles::new("/", generated))
    })
    .bind(("0.0.0.0", 8080))?
    .run()
    .await
}
