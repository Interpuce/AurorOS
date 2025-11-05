# Contributing to AurorOS & Building the System

Thank you for your interest in contributing to **AurorOS**!  
This guide will help you understand how to contribute and compile the system properly.

---

## 🧭 Code Writing Standards

To maintain consistency and avoid unnecessary commits, please follow these conventions carefully.

1. Use [snake_case](https://en.wikipedia.org/wiki/Snake_case) for all variable, function, and file names.  
2. Avoid creating new directories in the repository root unless absolutely necessary. (See [App Contribution Notes](#-contributing-to-apps)).  
3. Never include headers using relative paths such as:
```c
   #include "../include/file.h"
```

Instead, always use:

```c
#include <file.h>
```

4. Do **not** place C source files or other non-header files inside the `include/` directory.

> ⚠️ **Pull requests that do not follow these standards will be rejected.**

---

## 🧩 Contributing to the Kernel

Kernel contributions require prior **OS development experience**.
The kernel is a sensitive part of AurorOS, and careless changes can break the system.

Before submitting changes:

* Always [compile and run AurorOS](#-building-and-running-auroros) to verify your modifications.
* Provide a clear, English description of what you changed and why.
* Avoid making large rewrites. If you want to suggest a major redesign, please open a **feature request issue** first.

---

## 🧱 Contributing to Apps

Applications are less fragile than the kernel, but please still review the codebase before contributing.

> 💡 **Tip:** Avoid creating entirely new apps unless necessary or previously discussed with the team.

Before contributing to apps:

* Review and understand the [AurorOS Standard Library](https://github.com/Interpuce/stdlib).

---

## ⚙️ Building and Running AurorOS

### Prerequisites

> ⚠️ **Disclaimer:** You compile AurorOS at your own risk.
> We do **not officially support** custom user builds.

Ensure you are using a **Linux distribution** — all build scripts and tools are intended for Linux environments.

You’ll need the following packages installed:
`git`, `make`, `python3`, `gcc`, `nasm`, `binutils`, and `qemu-system-x86_64`.

#### 🟦 Debian/Ubuntu:

```bash
sudo apt install make python3 gcc binutils nasm qemu-system-x86_64
```

#### 🟩 Arch Linux:

```bash
sudo pacman -S make python gcc binutils nasm qemu
```

---

### 🧰 Cloning the Repository

Clone the repository with shallow history to save space and time:

```bash
git clone https://github.com/Interpuce/AurorOS . --depth 1
```

---

### 🏗️ Compiling AurorOS

Run one of the following commands in the AurorOS repository root:

```bash
make
```

or (alternative build script):

```bash
make shbuild
```

---

## 💿 Running AurorOS

After compilation, an **ISO image** will be generated in the `iso/` directory.
You can then boot it using QEMU or your preferred virtual machine:

```bash
qemu-system-x86_64 -cdrom iso/auroros.iso
```
