# zbl
zbl is a hobby project of mine that consists of a bootloader a kernel and a file system. Do not run this on a physical machine
it makes too many assumptions about hardware such as that it is on the main hard drive and that there is no partition table.
It works in QEMU and can read files and directories specified by the command mkgenfs
To build cross binutils<br>
cd toolchains<br>
cd binutils.cross<br>
mkdir build<br>
cd build<br>
../confiugre --target=i386-zbl --enable-languages=c,c++<br>
make all<br>
make install<br>
To build cross gcc<br>
cd toolchains<br>
cd gcc-6.1.0.cross<br>
mkdir build<br>
cd build<br>
../configure --target=i386-zbl --enable-languages=c,c++<br>
make all-gcc<br>
make all-target-libgcc<br>
make install-gcc<br>
make install-target-libgcc<br>
