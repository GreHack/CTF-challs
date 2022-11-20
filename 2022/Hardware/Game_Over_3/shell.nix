{ pkgs ? (import ./nixpkgs-pin.nix) { } }:

let
  rust = pkgs.rust-bin.stable.latest.default.override {
    targets = [ "x86_64-unknown-linux-gnu" "wasm32-wasi" ];
    extensions = [ "cargo" "rustc" "rust-src" ];
  };
in pkgs.mkShell {
  buildInputs = [
    rust
    pkgs.cargo-watch
  ];

  RUST_SRC_PATH = "${rust}/lib/rustlib/src/rust/library";
}
