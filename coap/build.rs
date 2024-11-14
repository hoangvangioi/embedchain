use prost_build::Config;

fn main() {
    Config::new()
        .compile_protos(&["protos/sensor.proto"], &["protos"])
        .unwrap();
}
