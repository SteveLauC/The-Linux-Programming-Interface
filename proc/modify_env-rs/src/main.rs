use itertools::Itertools;
use std::{
    collections::HashMap,
    env::{args, remove_var, set_var, vars_os, vars},
};

fn main() {
    // erase all  the environment varialbes
    for (key, _) in vars_os() {
        remove_var(key);
    }

    for item in args().skip(1) {
        let v: HashMap<&str, &str> = item.split('=').tuples().collect();
        for (key, value) in v.iter() {
            set_var(key, value);
        }
    }

    set_var("GREET", "Hello world");

    remove_var("BYE");

    for (key, value) in vars() {
        println!("{}={}", key, value);
    }
}
