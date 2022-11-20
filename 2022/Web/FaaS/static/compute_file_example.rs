fn main() {
    let mut input_file = String::new();
    let stdin = std::io::stdin();
    match stdin.read_line(&mut input_file) {
        Ok(_) => println!("{}", compute_file(input_file.trim_end())),
        Err(_) => println!("{}", "Coult not read input file path")
    }
}


fn compute_file(path: &str) -> &str {
    let kind = infer::get(&std::fs::read(path).unwrap_or(vec![]));
    
    return match kind {
        Some(file_type) => file_type.mime_type(),
        None => "cannot infer file type",
    }
}
