use error_chain::error_chain;

error_chain! {
    foreign_links {
        Io(std::io::Error);
        HttpRequest(reqwest::Error);
    }
}

fn main() -> Result<()> {
    let res = reqwest::blocking::get("http://127.0.0.1:8080/")?;

    if ! res.status().is_success() {
        return Err(Error::from("Accessing homepage is not successful"));
    }

    Ok(())
}
