use std::env;
use std::fs::{self, File};
use std::io::Write;
use std::path::PathBuf;

/// The whitespace-separated key names of one `[[keymap.layer]]` grid.
fn layer_keys(src: &str, name: &str) -> Vec<String> {
    let marker = format!("name = \"{name}\"");
    let from = src
        .find(&marker)
        .unwrap_or_else(|| panic!("keyboard_layout.toml: no layer named {name}"));
    let rest = &src[from..];
    let open = "keys = \"\"\"";
    let ks = rest
        .find(open)
        .unwrap_or_else(|| panic!("keyboard_layout.toml: layer {name} has no keys grid"))
        + open.len();
    let len = rest[ks..]
        .find("\"\"\"")
        .unwrap_or_else(|| panic!("keyboard_layout.toml: layer {name} keys grid is unterminated"));
    rest[ks..ks + len]
        .split_whitespace()
        .map(str::to_string)
        .collect()
}

/// TYPE is the hold-to-type mirror of BASE. GAME sits above BASE in the layer
/// order, and the keymap resolver only ever walks downward from the highest
/// active layer, so a momentary layer can shadow GAME only by carrying BASE's
/// own keycodes. Nothing keeps the two copies in step, so enforce it here.
fn check_type_mirrors_base(keymap: &str) {
    let base = layer_keys(keymap, "BASE");
    let typ = layer_keys(keymap, "TYPE");
    if base == typ {
        return;
    }
    let diff = base
        .iter()
        .zip(typ.iter())
        .enumerate()
        .find(|(_, (b, t))| b != t)
        .map(|(i, (b, t))| format!("first differs at cell {i}: BASE has `{b}`, TYPE has `{t}`"))
        .unwrap_or_else(|| format!("BASE has {} cells, TYPE has {}", base.len(), typ.len()));
    panic!(
        "keymap/keyboard_layout.toml: the TYPE layer must stay an exact copy of BASE \
         (it is the hold-to-type mirror reached from GAME); {diff}"
    );
}

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let workspace_root = manifest_dir.parent().unwrap();
    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap());

    // 1. Merge hardware.toml + shared keymap → keyboard.toml
    let hardware = fs::read_to_string(manifest_dir.join("hardware.toml"))
        .expect("failed to read hardware.toml");
    let keymap = fs::read_to_string(workspace_root.join("keymap/keyboard_layout.toml"))
        .expect("failed to read keymap/keyboard_layout.toml");

    check_type_mirrors_base(&keymap);

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
