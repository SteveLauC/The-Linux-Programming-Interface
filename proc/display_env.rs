use std::env::vars;

fn main() {
    vars().for_each(|(key, value): (String, String)| println!("{}={}", key, value));
}
