use std::env;
use std::fs::{self, File};
use std::io::Write;
use std::path::PathBuf;

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let workspace_root = manifest_dir.parent().unwrap();
    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());

    // 1. Merge hardware.toml + shared keymap → keyboard.toml
    let hardware = fs::read_to_string(manifest_dir.join("hardware.toml"))
        .expect("failed to read hardware.toml");
    let keymap = fs::read_to_string(workspace_root.join("keymap/keyboard_layout.toml"))
        .expect("failed to read keymap/keyboard_layout.toml");

    let keymap_stripped: String = keymap
        .lines()
        .filter(|l| !l.trim_start().starts_with('#'))
        .collect::<Vec<_>>()
        .join("\n");

    let keyboard_toml = manifest_dir.join("keyboard.toml");
    fs::write(&keyboard_toml, format!("{}\n{}", hardware, keymap_stripped))
        .expect("failed to write keyboard.toml");

    println!(
        "cargo:rustc-env=KEYBOARD_TOML_PATH={}",
        keyboard_toml.display()
    );
    println!("cargo:rerun-if-changed=hardware.toml");
    println!("cargo:rerun-if-changed=../keymap/keyboard_layout.toml");
    println!("cargo:rerun-if-changed=keyboard.toml");

    // 2. Copy memory.x to OUT_DIR so the linker can find it
    let memory_x = manifest_dir.join("memory.x");
    let memory_bytes = fs::read(&memory_x).expect("failed to read memory.x");
    File::create(out_dir.join("memory.x"))
        .unwrap()
        .write_all(&memory_bytes)
        .unwrap();
    println!("cargo:rustc-link-search={}", out_dir.display());
    println!("cargo:rerun-if-changed=memory.x");

    // 3. Linker flags
    println!("cargo:rustc-link-arg=--nmagic");
    println!("cargo:rustc-link-arg=-Tlink.x");
    println!("cargo:rustc-link-arg=-Tdefmt.x");
}
