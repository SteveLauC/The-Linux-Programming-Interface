use std::env::args;
use std::path::Path;

fn main() {
    for item in args().skip(1) {
        let path = Path::new(&item);
        println!("{} ==> {:?} + {:?}", path.display(), path.parent(), path.file_name())
    }
}
