🚀 TESTOSTERONE OS

TESTOSTERONE OS is an educational operating system built from scratch. This project is a deep dive into low-level programming, kernel development, and direct hardware abstraction.
📖 About the Project

The goal of this project is to build a functional, minimalistic OS while understanding every stage of the process: from the bootloader to memory management. The name reflects the drive, energy, and "hardcore" approach required to develop complex system software.
✨ Key Features

    Custom Kernel: Implementation of core system management functions.

    Memory Awareness: Detecting RAM size via CMOS.

    Low-Level Output: Direct VGA video memory manipulation for text rendering.

    x86 Architecture: Optimized for the classic i386 platform.

🛠 Tech Stack

    Languages: C, Assembly (NASM)

    Toolchain: GCC, LD

    Emulation: QEMU

    Environment: Linux-based development.

🧬 Credits & Acknowledgments

This operating system is based on the os-project by Denis Nikulin. The original source code served as the foundation and was heavily modified and expanded during the development of TESTOSTERONE OS.
🚀 Installation & Quick Start
1. Prerequisites

Install the QEMU emulator:
Bash

sudo apt update
sudo apt install qemu-kvm qemu

2. Set Up the Cross-Compiler

For x86_64 Linux systems, use a prebuilt i386-elf toolchain:
Bash

wget http://newos.org/toolchains/i386-elf-4.9.1-Linux-x86_64.tar.xz
sudo mkdir -p /usr/local/i386elfgcc
sudo tar -xf i386-elf-4.9.1-Linux-x86_64.tar.xz -C /usr/local/i386elfgcc --strip-components=1
export PATH=$PATH:/usr/local/i386elfgcc/bin

3. Build the Project

Clone the repository and run the build script:
Bash

cd TESTOSTERONE_OS/src/build
make

4. Run the OS

Launch the generated image using QEMU:
Bash

qemu-system-i386 -fda os-image.bin

🔗 Project Links

Stay updated or contribute via the official mirrors:
