use std::env::args;

fn main() {
    args()
        .enumerate()
        .for_each(|(index, arg): (usize, String)| {
            println!("av[{}] = {}", index, arg);
        });
}
