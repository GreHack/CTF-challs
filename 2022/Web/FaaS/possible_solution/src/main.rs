fn main() {
    let paths = std::fs::read_dir("/").unwrap();

    for path in paths {
        println!("{}", path.unwrap().path().display())
    }

    println!("Flag: {}", std::fs::read_to_string("/flag").unwrap())
}
