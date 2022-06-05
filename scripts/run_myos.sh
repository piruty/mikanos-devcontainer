#! /usr/bin/env bash

# カーネルのビルドに必要な環境変数を読み込む
source $HOME/osbook/devenv/buildenv.sh

# カーネルのビルド
cd $MYOS/kernel
make
# clang++ $CPPFLAGS -O2 --target=x86_64-elf -fno-exceptions -ffreestanding -c main.cpp
# ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o

# 独自実装のカーネルを起動する
cd $HOME/edk2
unlink MikanLoaderPkg
ln -s $MYOS/MikanLoaderPkg/ ./
build

$HOME/osbook/devenv/run_qemu.sh Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MYOS/kernel/kernel.elf