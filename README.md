# zbl
zbl is a hobby project of mine that consists of a bootloader a kernel and a file system. Do not run this on a physical machine
it makes too many assumptions about hardware such as that it is on the main hard drive and that there is no partition table.
It works in QEMU and can read files and directories specified by the command mkgenfs
To build cross binutils
cd toolchains
cd binutils.cross
mkdir build
cd build
../confiugre --target=i386-zbl --enable-languages=c,c++
make all
make install
To build cross gcc
cd toolchains
cd gcc-6.1.0.cross
mkdir build
cd build
../configure --target=i386-zbl --enable-languages=c,c++
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
