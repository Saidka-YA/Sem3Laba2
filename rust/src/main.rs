mod structures;
mod algorithms;
mod interface;

fn main() {
    if let Err(error) = interface::run() {
        eprintln!("{}", error);
        std::process::exit(1);
    }
}
