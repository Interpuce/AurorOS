#!/bin/bash --posix

# Please make sure you have acknowledged AurorOS license.
# It is available at https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md


echo -e "\033[1m[NOTIFY_MULTILINE] AurorOS Build-time Dependencies Manager will now attempt to install missing dependencies.\033[0m"
echo -e "\033[1m\033[33mNote that dependencies that are already installed will either be updated or reinstalled.\033[0m"
echo -e "\033[1mIf you don't want to proceed, press \033[31mCtrl-C\033[0m \033[1mto kill the process and abort the setup.\033[0m"

sleep 0.2
echo -e "\n"

echo -e "\033[1m[CHOICE]\033[0m Select your distribution (or base, in case of derivatives):"
echo "[A]rch [D]ebian"
sleep 0.2
echo -e "\n"

read -r distro

# Install dependencies with package manager applicable for selected distro
case "$distro" in
  [Aa])
    echo -e "\033[1m[NOTIFY]\033[0m Proceeding with pacman."
    sudo --user=root pacman -Sy
    sudo --user=root pacman -S make python gcc binutils nasm qemu
    ;;
  [Dd])
    echo -e "\033[1m[NOTIFY]\033[0m Proceeding with APT."
    sudo --user=root apt update
    sudo --user=root apt install make python3 gcc binutils nasm qemu-system-x86_64
    ;;
  *)
    echo -e "\033[1m[ERROR]\033[0m Invalid selection. Please select [A]rch or [D]ebian."
    ;;
esac
# That's it.
