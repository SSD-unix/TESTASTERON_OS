# 🚀 TESTOSTERONE OS

<div align="center">

[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![GitHub Stars](https://img.shields.io/github/stars/SSD-unix/TESTASTERON_OS?style=flat-square)](https://github.com/SSD-unix/TESTASTERON_OS/stargazers)
[![GitHub Forks](https://img.shields.io/github/forks/SSD-unix/TESTASTERON_OS?style=flat-square)](https://github.com/SSD-unix/TESTASTERON_OS/network)
[![GitHub Issues](https://img.shields.io/github/issues/SSD-unix/TESTASTERON_OS?style=flat-square)](https://github.com/SSD-unix/TESTASTERON_OS/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/SSD-unix/TESTASTERON_OS?style=flat-square)](https://github.com/SSD-unix/TESTASTERON_OS/pulls)

**An Educational Operating System Built From Scratch**

[View on GitHub](https://github.com/SSD-unix/TESTASTERON_OS) • [Report Bug](https://github.com/SSD-unix/TESTASTERON_OS/issues) • [Request Feature](https://github.com/SSD-unix/TESTASTERON_OS/issues)

</div>

---

## 📖 About the Project

TESTOSTERONE OS is an educational operating system built from scratch. This project is a deep dive into low-level programming, kernel development, and direct hardware abstraction.

The goal of this project is to build a functional, minimalistic OS while understanding every stage of the process: from the bootloader to memory management. The name reflects the drive, energy, and passion behind building systems software from the ground up.

---

## ✨ Key Features

- 🎯 **Custom Kernel**: Implementation of core system management functions
- 💾 **Memory Awareness**: Detecting RAM size via CMOS
- 🖥️ **Low-Level Output**: Direct VGA video memory manipulation for text rendering
- ⚙️ **x86 Architecture**: Optimized for the classic i386 platform
- 📚 **Educational Focus**: Well-documented code for learning purposes

---

## 🛠 Tech Stack

| Component | Technology |
|-----------|-----------|
| **Languages** | C, Assembly (NASM) |
| **Toolchain** | GCC, LD |
| **Emulation** | QEMU |
| **Environment** | Linux-based development |

---

## 🧬 Credits & Acknowledgments

This operating system is based on the os-project by Denis Nikulin. The original source code served as the foundation and was heavily modified and expanded during the development of TESTOSTERONE OS.

---

## 🚀 Installation & Quick Start

### 1. Prerequisites

Install the QEMU emulator:

```bash
sudo apt update
sudo apt install qemu-kvm qemu
```

### 2. Set Up the Cross-Compiler

For x86_64 Linux systems, use a prebuilt i386-elf toolchain:

```bash
wget http://newos.org/toolchains/i386-elf-4.9.1-Linux-x86_64.tar.xz
sudo mkdir -p /usr/local/i386elfgcc
sudo tar -xf i386-elf-4.9.1-Linux-x86_64.tar.xz -C /usr/local/i386elfgcc --strip-components=1
export PATH=$PATH:/usr/local/i386elfgcc/bin
```

### 3. Build the Project

Clone the repository and run the build script:

```bash
git clone https://github.com/SSD-unix/TESTASTERON_OS.git
cd TESTASTERON_OS/src/build
make
```

### 4. Run the OS

Launch the generated image using QEMU:

```bash
qemu-system-i386 -fda os-image.bin
```

---

## 📂 Project Structure

```
TESTASTERON_OS/
├── src/
│   ├── build/        # Build artifacts and Makefile
│   ├── boot/         # Bootloader code
│   ├── kernel/       # Kernel implementation
│   └── drivers/      # Hardware drivers
├── docs/             # Documentation
├── README.md         # This file
└── LICENSE           # Project license
```

---

## 🤝 Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 📧 Contact & Support

- **GitHub Issues**: [Report bugs or request features](https://github.com/SSD-unix/TESTASTERON_OS/issues)
- **GitHub Discussions**: [Join our community discussions](https://github.com/SSD-unix/TESTASTERON_OS/discussions)

---

<div align="center">

**⭐ If you find this project helpful, please consider giving it a star!**

[Back to Top](#-testosterone-os)

</div>
