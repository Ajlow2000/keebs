use std::env;
use std::fs::{self, File};
use std::io::{Read, Write};
use std::path::{Path, PathBuf};

use const_gen::*;
use xz2::read::XzEncoder;

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

    // 2. Generate config_generated.rs from vial.json
    println!("cargo:rerun-if-changed=vial.json");
    generate_vial_config(&manifest_dir, &out_dir);

    // 3. Copy memory.x to OUT_DIR so the linker can find it
    let memory_x = manifest_dir.join("memory.x");
    let memory_bytes = fs::read(&memory_x).expect("failed to read memory.x");
    File::create(out_dir.join("memory.x"))
        .unwrap()
        .write_all(&memory_bytes)
        .unwrap();
    println!("cargo:rustc-link-search={}", out_dir.display());
    println!("cargo:rerun-if-changed=memory.x");

    // 4. Linker flags
    println!("cargo:rustc-link-arg=--nmagic");
    println!("cargo:rustc-link-arg=-Tlink.x");
    println!("cargo:rustc-link-arg=-Tdefmt.x");
}

fn generate_vial_config(manifest_dir: &Path, out_dir: &Path) {
    let out_file = out_dir.join("config_generated.rs");
    let vial_path = manifest_dir.join("vial.json");

    let mut content = String::new();
    match File::open(&vial_path) {
        Ok(mut f) => {
            f.read_to_string(&mut content)
                .expect("cannot read vial.json");
        }
        Err(e) => {
            println!("cargo:warning=vial.json not found ({e}); using empty config");
            content = "{}".to_owned();
        }
    }

    let vial_cfg = json::stringify(json::parse(&content).unwrap());
    let mut compressed: Vec<u8> = Vec::new();
    XzEncoder::new(vial_cfg.as_bytes(), 6)
        .read_to_end(&mut compressed)
        .unwrap();

    let keyboard_id: Vec<u8> = vec![0xB9, 0xBC, 0x09, 0xB2, 0x9D, 0x37, 0x4C, 0xEA];
    let declarations = [
        const_declaration!(pub VIAL_KEYBOARD_DEF = compressed),
        const_declaration!(pub VIAL_KEYBOARD_ID = keyboard_id),
    ]
    .map(|s| "#[allow(clippy::redundant_static_lifetimes)]\n".to_owned() + &s)
    .join("\n");

    fs::write(out_file, declarations).unwrap();
}
