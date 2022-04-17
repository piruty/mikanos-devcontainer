## xquartzで動かないとき

1. xtermで以下を実行
  - `xhost + 127.0.0.1`
2. devcontainerの設定でコメントアウトを外さない
  - 外すとqemu経由で動かなかった

## パスの設定

```bash
export MIKANOS=/workspaces/mikanos-devcontainer/mikanos
export MYOS=/workspaces/mikanos-devcontainer/myos
```

## MikanOSのリンク

```bash
cd $HOME/edk2
unlink MikanLoaderPkg
ln -s $MIKANOS/MikanLoaderPkg/ ./
```

## 自作OSのリンク

```bash
cd $HOME/edk2
unlink MikanLoaderPkg
ln -s $MYOS/MikanLoaderPkg/ ./
```

## Conf設定の更新

```bash
cp $CODE_HOME/Conf/target.txt $HOME/edk2/Conf/target.txt
cp $CODE_HOME/Conf/tools_def.txt $HOME/edk2/Conf/tools_def.txt
```

## OSのビルド

```bash
cd $HOME/edk2
source edksetup.sh
build
```

## build結果の実行

```bash
# 実行してから結果が出るまでにqemuを落とすとハングする
# 辛抱強く待つこと
~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

```bash
# CPUレジスタの値を表示
(qemu) info registers

# xコマンド: メモリダンプ
# x /fmt addr というコマンドを実行する
# fmt = [個数][フォーマット][サイズ]
(qemu) x /4xb 0x067ae4c4
```

```bash
# カーネルのビルド
cd $OS_DIR/kernel
clang++ -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -c main.cpp
ld.lld --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o
```

## レジスタ

- AX: RAXレジスタの下位16ビット
- RIP: CPUが次に実行する命令のメモリアドレスを保持する
- RFLAGS
- CR0
- dec
- jnz

```bash
# ELF形式ファイルの詳細情報を取得
# -h: ELFファイルのヘッダを表示するオプション
readelf -h xxx.elf
```

```bash
# カーネルのビルドに必要な環境変数を読み込む
source $HOME/osbook/devenv/buildenv.sh

# カーネルのビルド
cd $MIKANOS/kernel
make
# clang++ $CPPFLAGS -O2 --target=x86_64-elf -fno-exceptions -ffreestanding -c main.cpp
# ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o

# mikanosイメージのビルド
cd $HOME/edk2
unlink MikanLoaderPkg
ln -s $MIKANOS/MikanLoaderPkg/ ./
build

# カーネルを使用してOSを起動する
$HOME/osbook/devenv/run_qemu.sh Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $CODE_HOME/mikanos/kernel/kernel.elf
```

```bash
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
```

## 通常のnew VS 配置new

- 配置new
  - new演算子が引数を取る
  - メモリ領域を確保しない代わりに、引数に指定したメモリ上にインスタンスを作成する
    - OSによるメモリ管理がない場合でも利用可能
    - 配列と組み合わせれば、好きな大きさのメモリ領域を確保でき、クラスのインスタンスが生成可能
- 通常のnew
  - ヒープ領域にインスタンスを生成する
    - 関数の実行が終了しても破棄されない
  - ヒープ領域の確保は、OSがメモリ管理を提供していて可能になる