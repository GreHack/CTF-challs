use error_chain::error_chain;
use regex::Regex;
use std::io::Read;

error_chain! {
    foreign_links {
        Io(std::io::Error);
        HttpRequest(reqwest::Error);
    }
}

fn main() -> Result<()> {
    let mut res = reqwest::blocking::get(
        "http://127.0.0.1:8080/remote?action=read&id=9941ae7962f3830b2e901dd62c0a0fc9",
    )?;
    let mut body = String::new();
    res.read_to_string(&mut body)?;

    let re = Regex::new(r"^GH22\{.+\}$").unwrap();

    if !re.is_match(body.as_str()) {
        return Err(Error::from(format!(
            "Flag does not seem to be present anymore, got {}",
            body
        )));
    }

    Ok(())
}
