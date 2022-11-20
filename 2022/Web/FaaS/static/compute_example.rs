#[derive(serde::Deserialize)]
struct Example {
    a: i32,
    b: i32,
}

fn main() {
    match serde_json::from_reader(std::io::stdin()) {
        Ok(input) => compute_display_result(input),
        Err(er) => println!("{}", er.to_string())
    }
}

fn compute_display_result(input: Example) {
    println!("{}", input.a + input.b);
}
