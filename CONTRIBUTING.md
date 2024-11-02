# Contributing to AurorOS & compiling OS

Thanks for your interest in contributing to AurorOS. This is a quick guide how to contribute to AurorOS.

## Contributing to AurorOS Kernel

If you want to contribute to AurorOS kernel you need to have previous experience with OS-dev. You can break the code very easily, so always [compile the OS](#compiling-auroros) and check the changes. Every change must be described with English and can't rebuild the entire kernel (if you want, make issue as feature request).

## Contributing to the apps

The apps are not that easy to break, so you can do more changes. You cannot create new apps, because it will be you app and not the app for OS. Instead try feature request.

## Compiling AurorOS

Try these commands in Linux in AurorOS repo root folder:

```
chmod -x build.sh
./build.sh
```

You will need `gcc`, `nasm` and `qemu` installed. **We listed what you need. These are not package names.**