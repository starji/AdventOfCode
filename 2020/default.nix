with import <nixpkgs> {};
stdenv.mkDerivation rec {
  name = "adventofcode2020";
  buildInputs = [ stdenv cargo rustc rustfmt ];
}
