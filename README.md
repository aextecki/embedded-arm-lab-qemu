# 🚀 Embedded ARM Systems Lab: C & QEMU Integration

This repository documents my journey from "Zero to Hero" in **Embedded Linux**. It features a C-based GPIO controller designed for ARM architecture, developed on **Debian 13 (Trixie)**, and executed within an emulated **ARMv5** environment.

## 🛠 Project Overview
The goal of this project was to build a C application that interacts with hardware registers through the Linux `sysfs` interface. This demonstrates the full lifecycle of embedded development: **Cross-Compilation -> Virtualized Networking -> Target Deployment.**

### The "Hardware" (Virtualized)
* **Architecture:** ARMv5te (VersatilePB)
* **Kernel:** Linux 3.2.0-4-versatile
* **Emulator:** QEMU (Quick Emulator)
* **OS:** Debian 7 (Wheezy) armel

---

## 🏗 Technical Architecture

The application operates in **User Space** but controls virtual hardware by writing to the `/sys/class/gpio` interface. This triggers the Linux Kernel to toggle pins on the emulated processor.



---

## 🧗 The "Hardship" Log (Troubleshooting Documentation)
I treated every technical failure as a learning milestone. Below are the hurdles overcome during development:

### 1. The Kernel Panic (VFS: Unable to mount root fs)
* **The Struggle:** The system refused to boot, failing to find the virtual hard drive (`unknown-block(0,0)`).
* **The Solution:** Successfully mapped the **Initial RAM Disk (initrd)** to provide the kernel with the necessary drivers at boot time.
* **The Lesson:** In embedded systems, the kernel needs an initial filesystem to load drivers before it can "see" the main hard drive.

### 2. Networking & The "Connection Refused" Door
* **The Struggle:** Host-to-Guest file transfer via `scp` failed.
* **The Solution:** Configured **NAT Port Forwarding** (`hostfwd=tcp::2222-:22`) to tunnel SSH traffic between the Debian 13 host and the ARM target.
* **The Lesson:** Virtualized environments require explicit port mapping to communicate with the host machine.

### 3. "Illegal Instruction" (CPU Mismatch)
* **The Struggle:** The program crashed immediately because the compiler was targeting a modern ARM architecture.
* **The Solution:** Identified the architecture mismatch and switched the compiler target to `-march=armv5te`.
* **The Lesson:** Binaries are hardware-specific; you must compile for the exact "silicon" instructions the CPU understands.

### 4. ABI & The Missing Header (`stubs-soft.h`)
* **The Struggle:** The `gnueabihf` (Hard Float) compiler failed because the target CPU lacked a Floating Point Unit (FPU).
* **The Solution:** Switched to the `arm-linux-gnueabi` toolchain (Soft Float) to emulate math in software.
* **The Lesson:** Understanding the **ABI (Application Binary Interface)** is critical for low-power embedded chips.

---

## 🚀 How to Reproduce

### 1. Compile the Source (Host)
Use the Soft-Float toolchain to ensure compatibility:
```bash
arm-linux-gnueabi-gcc src/main.c -o gpio_controller_arm -static