# Contributing to AurorOS & compiling OS

Thanks for your interest in contributing to AurorOS. This is a quick guide how to contribute to AurorOS.

## Contributing important ToDo's

If you want to help with AurorOS please consider contributing to one of these aspects:

- security system (better and new user system)
- drivers

## Contributing to AurorOS Kernel

If you want to contribute to AurorOS kernel you need to have previous experience with OS-dev. You can break the code very easily, so always [compile the OS](#compiling-auroros) and check the changes. Every change must be described with English and can't rebuild the entire kernel (if you want, make issue as feature request).

## Contributing to the apps

The apps are not that easy to break, so you can do more changes. Avoid creating new apps, because they are **your** creation.

## Compiling and running AurorOS

### Compilation

> [!CAUTION]
> You will do this only at your own risk. We don't officially support any OS builds compiled by the user.

First - make sure you have Linux distribution installed and you are going to execute these commands on Linux distribution.

To clone, compile and run virtually AurorOS you will need `git`, `make`, `python3`, `gcc`, `nasm`, `binutils` and `qemu-system-x86_64` installed. 

On Debian-based distributions you can install these by executing this command:

```sudo apt install make python3 gcc binutils nasm qemu-system-x86_64```

On Arch-based distributions you can execute this command to install packages:
```sudo pacman -S make python gcc binutils nasm qemu```

Clone the repository to the current folder skipping unnecessary commit history:

```git clone https://github.com/Interpuce/AurorOS . --depth 1```

Run this command in AurorOS repo root folder:

```make```

or alternatively:

```make shbuild```

### Running OS in virtual machine

You can run AurorOS using this command:

```qemu-system-x86_64 -kernel kernel.bin```

### Running OS on a real computer

This is not currently possible, since we don't have ISO.
