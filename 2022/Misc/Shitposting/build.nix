let
  pkgs = import (builtins.fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/cae3751e9f74eea29c573d6c2f14523f41c2821a.tar.gz";
    sha256 = "18x91ly0nrq8ipfdaxdaq2iz0w4xk62bwz89nz17vxxz44lrkfky";
  }) {};
  zipQuine = pkgs.fetchurl {
    url = "http://swtch.com/r.zip";
    sha256 = "sha256-0VLTaMotoA3ph8/ljIaFNEEKsnB3PVnQ15VuZx062v0=";
  };
  zipQuineWithTrollComment = pkgs.stdenvNoCC.mkDerivation {
    name = "zip-quine-troll-comment";

    src = zipQuine;

    nativeBuildInputs = [ pkgs.zip ];

    buildPhase = ''
      install -m0644 $src GH22.zip
      echo 'GH22{...} is inside me (maybe?)' | zip -z GH22.zip
    '';

    installPhase = ''
      cp GH22.zip $out
    '';

    dontUnpack = true;
    dontConfigure = true;
  };
  pdfWithFlag = pkgs.stdenvNoCC.mkDerivation {
    name = "pdf-flag";

    src = ./flag.txt;

    nativeBuildInputs = [ pkgs.pandoc pkgs.texlive.combined.scheme-small ];

    buildPhase = ''
      pandoc $src -o flag.pdf
    '';

    installPhase = ''
      cp flag.pdf $out
    '';

    dontUnpack = true;
    dontConfigure = true;
  };
  contentBin = pkgs.stdenvNoCC.mkDerivation {
    name = "content.bin";

    srcs = [ pdfWithFlag zipQuineWithTrollComment ];

    buildPhase = ''
      cat $srcs > content.bin
    '';

    installPhase = ''
      cp content.bin $out
    '';

    dontUnpack = true;
    dontConfigure = true;
  };
  memePng = ./meme.png;
in pkgs.stdenvNoCC.mkDerivation {
  name = "challenge_shitposting";

  src = pkgs.fetchurl {
    url = "https://raw.githubusercontent.com/DavidBuchanan314/tweetable-polyglot-png/014e673a1ee0b5722b27bb10856a677b234b358f/pack.py";
    sha256 = "sha256-1iJRxNrgCfrRrcsS4zUJIYaXLUHkClGtkso9xdivsDw=";
  };

  nativeBuildInputs = [ pkgs.python310 ];

  buildPhase = ''
    python $src ${memePng} ${contentBin} all.png
  '';

  installPhase = ''
    cp all.png $out
  '';

  dontUnpack = true;
  dontConfigure = true;
}
