with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "AdventOfCode";
  buildInputs = [ gcc ];
}