use chrono::{offset::Local, DateTime};

pub fn curr_time(format: &str) -> String {
    let now: DateTime<Local> = Local::now();

    now.format(format).to_string()
}
