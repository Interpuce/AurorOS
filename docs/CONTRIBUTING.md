# Contributing and building information

Thank you for your interest in contributing to **AurorOS**!  
This guide will help you understand how to contribute and compile the system properly.

Please refer to [code style guide](https://github.com/Interpuce/AurorOS/blob/main/docs/CODE_STYLE.md)
when adding new code to AurorOS.

---

## Building and Running AurorOS

### Prerequisites

> [!CAUTION]
> You compile AurorOS at your own risk.
> We do **not officially support** custom user builds.

Ensure you are using a **Linux distribution** — all build scripts and tools are intended for Linux environments.

You’ll need the following packages installed:
`git`, `make`, `python3`, `gcc`, `nasm`, `binutils`, and `qemu-system-x86_64`.

#### Debian/Ubuntu:

```bash
sudo apt install make python3 gcc binutils nasm qemu-system-x86_64
```

#### Arch Linux:

```bash
sudo pacman -S make python gcc binutils nasm qemu
```

---

### Cloning the Repository

Clone the repository with shallow history to save space and time:

```bash
git clone https://github.com/Interpuce/AurorOS . --depth 1
```

---

### Compiling AurorOS

Run this in AurorOS repository root:

```bash
make
```

> [!TIP]
> Compiling AurorOS on a single thread can be slow. You can add the `-j$(nproc)` flag, or `-j` flag followed by the number of your CPU threads (e.g. `-j32`), or simply `-j` to compile with all available threads.

Alternativelly, you can go straight to the *Running AurorOS* section,
because the `run` make target is dependent on the `all` make target, which
is a default one and compiles AurorOS.

To recompile AurorOS with previously cleaning all the previously compiled 
source files, please run:

```bash
make recompile
```

### Running AurorOS

After compilation, an **ISO image** will be generated.
You can then boot it using QEMU or your preferred virtual machine:

```bash
qemu-system-x86_64 -cdrom AurorOS.iso
```

or with a handy alias:

```bash
make run
```

If the OS bootloops or debugging it using code is nearly impossible,
you can try launching AurorOS with a debugger:

```bash
make run_dbg
```
