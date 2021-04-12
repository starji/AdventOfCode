with import <nixpkgs> {};
stdenv.mkDerivation rec {
   name = "beasttest";
   env = buildEnv { name = name; paths = buildInputs; };
   buildInputs = [ stdenv cmake gnumake git boost159 rtags clang gdb ];
}
