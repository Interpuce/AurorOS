# Contributing to AurorOS & compiling OS

Thanks for your interest in contributing to AurorOS. This is a quick guide how to contribute to AurorOS.

## Contributing to AurorOS Kernel

If you want to contribute to AurorOS kernel you need to have previous experience with OS-dev. You can break the code very easily, so always [compile the OS](#compiling-auroros) and check the changes. Every change must be described with English and can't rebuild the entire kernel (if you want, make issue as feature request).

## Contributing to the apps

The apps are not that easy to break, so you can do more changes. Avoid creating new apps, because they are **your** creation.

## Compiling and running AurorOS

### Compilation

First - make sure you have Linux distribution installed and you are going to execute these commands on Linux distribution.

To compile and run virtually AurorOS you will need `make`, `python3`, `gcc`, `nasm` and `qemu-system-x86_64` installed. 

On Debian-based distributions you can install these by executing this command:

```sudo apt install make python3 gcc nasm qemu-system-x86_64```

On Arch-based distributions you can execute this command to install packages:
```sudo pacman -S make python gcc nasm qemu```

Run this command in AurorOS repo root folder:

```make```

or alternative command:

```python3 compile/build.py```

### Running OS in virtual machine

You can run AurorOS using this command:

```qemu-system-x86_64 -kernel kernel.bin```

## Running OS on a real computer

This is not currently possible, since we don't have ISO.