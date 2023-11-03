use std::{
    error::Error,
    fs::{read_dir, read_to_string, File},
    io::Write,
};

static DEVICE_DIR: &str = "/sys/class/backlight/";
static MAX_BRIGHTNESS_FILE: &str = "max_brightness";
static BRIGHTNESS_FILE: &str = "brightness";

fn main() -> Result<(), Box<dyn Error>> {
    let mut path = read_dir(DEVICE_DIR).expect("Wrong Device Directory");
    let device = path.next().unwrap().unwrap().path();

    let max_val: i64 = read_to_string(device.join(&MAX_BRIGHTNESS_FILE))?
        .trim()
        .parse()?;
    let cur_val: i64 = read_to_string(device.join(&BRIGHTNESS_FILE))?
        .trim()
        .parse()?;
    let mut perc: i64 = cur_val * 100 / max_val;
    let args: Vec<String> = std::env::args().collect();
    if args.len() == 1 {
        println!("{}", perc);
    } else {
        let change: i64 = args[1].parse()?;
        perc = match args[1].chars().into_iter().next().unwrap() {
            '+' => {
                if perc + change > 100 {
                    100
                } else {
                    perc + change
                }
            }
            '-' => {
                if perc + change < 0 {
                    0
                } else {
                    perc + change
                }
            }
            _ => {
                if change > 100 {
                    100
                } else {
                    change
                }
            }
        };
        let new_val = max_val * perc / 100;
        let mut brighness_file: File = File::options()
            .write(true)
            .open(device.join(&BRIGHTNESS_FILE))?;
        write!(brighness_file, "{}", new_val)?;
        println!("{}", perc);
    }
    Ok(())
}
