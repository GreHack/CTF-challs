{ }:
let
  oxalicaRustOverlay = (import (builtins.fetchTarball { url = "https://github.com/oxalica/rust-overlay/archive/91af035847a74cbd49ce9f764f8ef5fc641cb9b0.tar.gz"; sha256 = "1qsb2kabfcrkhhlyhsxk66463d5fy2rdr32psmmi5is534gvz624"; }));
in (import (builtins.fetchTarball { url = "https://github.com/NixOS/nixpkgs/archive/bbf77421ac51a7c93f5f0f760da99e4dbce614fa.tar.gz"; sha256 = "1d9vps802vyacf30cqiwbg2007sgaaz6spx5zk8fw71rpm7pgghq"; })) { overlays = [ oxalicaRustOverlay ]; }