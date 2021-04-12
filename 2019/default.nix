with import <nixpkgs> {};
stdenv.mkDerivation rec {
  name = "adventofcode";
  buildInputs = [ stdenv cmake gnumake git gcc gdb boost171 ];
}
