{
  description = "RMK keyboard firmware: Halcyon Kyria";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    rust-overlay = {
      url = "github:oxalica/rust-overlay";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = { self, nixpkgs, rust-overlay }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      forAllSystems = nixpkgs.lib.genAttrs systems;

      pkgsFor = system: import nixpkgs {
        inherit system;
        overlays = [ rust-overlay.overlays.default ];
      };

      rustToolchainFor = pkgs:
        pkgs.rust-bin.fromRustupToolchainFile ./rust-toolchain.toml;

    in
    {
      devShells = forAllSystems (system:
        let
          pkgs = pkgsFor system;
          rustToolchain = rustToolchainFor pkgs;
        in
        {
          default = pkgs.mkShell {
            name = "keebs-dev";

            packages = [
              rustToolchain

              # Stack overflow detection via a linker wrapper
              pkgs.flip-link

              # ELF → UF2 converter for drag-and-drop flashing
              # If missing: nix search nixpkgs elf2uf2 to find the right attr
              pkgs.elf2uf2-rs

              # Debug probe flashing and logging (probe-rs run / probe-rs attach)
              pkgs.probe-rs-tools

              # Task runner
              pkgs.cargo-make

              pkgs.pkg-config
            ] ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
              # probe-rs links libudev on Linux
              pkgs.udev
            ];

            CARGO_TARGET_THUMBV6M_NONE_EABI_LINKER =
              "${pkgs.flip-link}/bin/flip-link";

            DEFMT_LOG = "info";
          };
        }
      );

      # Each attribute builds one UF2 binary ready for drag-and-drop flashing.
      #
      # First build: cargoHash will fail with a mismatch. Copy the "got:" hash
      # from the error and replace the empty string below.
      #
      # Prerequisites: Cargo.lock must be committed before `nix build` works.
      # Run `cargo generate-lockfile` after first `nix develop`.
      packages = forAllSystems (system:
        let
          pkgs = pkgsFor system;
          rustToolchain = rustToolchainFor pkgs;

          rustPlatform = pkgs.makeRustPlatform {
            cargo = rustToolchain;
            rustc = rustToolchain;
          };

          buildFirmware = { pname, binName, keyboard }:
            rustPlatform.buildRustPackage {
              inherit pname;
              version = "0.1.0";
              src = self;

              cargoLock.lockFile = ./Cargo.lock;

              CARGO_BUILD_TARGET = "thumbv6m-none-eabi";
              CARGO_TARGET_THUMBV6M_NONE_EABI_LINKER =
                "${pkgs.flip-link}/bin/flip-link";

              nativeBuildInputs = [
                pkgs.flip-link
                pkgs.elf2uf2-rs
                pkgs.pkg-config
              ] ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
                pkgs.udev
              ];

              buildPhase = ''
                runHook preBuild
                cargo build \
                  --release \
                  --target thumbv6m-none-eabi \
                  --bin ${binName} \
                  --manifest-path ${keyboard}/Cargo.toml
                runHook postBuild
              '';

              installPhase = ''
                runHook preInstall
                mkdir -p "$out"
                elf2uf2-rs \
                  "target/thumbv6m-none-eabi/release/${binName}" \
                  "$out/${binName}.uf2"
                runHook postInstall
              '';

              # Embedded binaries cannot run on the build host
              doCheck = false;
            };

        in
        {
          halcyon-kyria-central = buildFirmware {
            pname    = "halcyon-kyria-central";
            binName  = "halcyon-kyria-central";
            keyboard = "halcyon-kyria";
          };
          halcyon-kyria-peripheral = buildFirmware {
            pname    = "halcyon-kyria-peripheral";
            binName  = "halcyon-kyria-peripheral";
            keyboard = "halcyon-kyria";
          };
        }
      );
    };
}
