mod realpath;
mod utility;

use realpath::my_realpath;

fn main() {
    let res1 = my_realpath("link1");
    let res2 = my_realpath("link2");
    let res3 = my_realpath("link3");

    println!(
        "res1: {}\nres2: {}\nres3: {}",
        res1.unwrap().display(),
        res2.unwrap().display(),
        res3.unwrap().display()
    );
}
