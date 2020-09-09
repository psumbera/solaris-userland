How to build Rust on S11.4SRU
-----------------------------

You will need GCC 9 which is available from S11.4SRU12. Install
following meta packages to get it:

pkg install developer/opensolaris/userland developer/gcc-9

Clone repo and initialize it:

git clone https://github.com/psumbera/solaris-userland.git
cd solaris-userland/
gmake setup CC=gcc CFLAGS=-fPIC CANONICAL_REPO= SPRO_VROOT=NO_STUDIO

Build Rust:

You will need to use newer version of Rust compiler and Cargo-vendor
module than is on system. Use PATH_CARGO_VENDOR and PATH_CARGO_VENDOR to
point build to required version:

cd components/rust/rustc

gmake install PATH_RUST=/builds/rustc-1.45.0/bin/ INTERNAL_ARCHIVE_MIRROR=

The resulting binaries should be installed in following subdirectory:

build/prototype/`mach`/
