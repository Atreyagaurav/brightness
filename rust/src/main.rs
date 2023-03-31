use std::{
    error::Error,
    fs::{read_to_string, File},
    io::Write,
};

static MAX_BRIGHTNESS_FILE: &str = "/sys/class/backlight/amdgpu_bl0/max_brightness";
static BRIGHTNESS_FILE: &str = "/sys/class/backlight/amdgpu_bl0/brightness";

fn main() -> Result<(), Box<dyn Error>> {
    let max_val: i64 = read_to_string(&MAX_BRIGHTNESS_FILE)?.trim().parse()?;
    let cur_val: i64 = read_to_string(&BRIGHTNESS_FILE)?.trim().parse()?;
    let mut perc: i64 = cur_val * 100 / max_val;
    let args: Vec<String> = std::env::args().collect();
    if args.len() == 1 {
        println!("{}", perc);
    } else {
        let change: i64 = args[1].parse()?;
        if perc + change < 0 {
            perc = 0;
        } else if perc + change > 100 {
            perc = 100;
        } else {
            perc += change;
        }
        let new_val = max_val * perc / 100;
        let mut brighness_file: File = File::options().write(true).open(BRIGHTNESS_FILE)?;
        write!(brighness_file, "{}", new_val)?;
        println!("{}", perc);
    }
    Ok(())
}
