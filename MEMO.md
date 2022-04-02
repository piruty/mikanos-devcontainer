## xquartzで動かないとき

1. xtermで以下を実行
  - `xhost + 127.0.0.1`
2. devcontainerの設定でコメントアウトを外さない
  - 外すとqemu経由で動かなかった

## MikanOSのリンク

```bash
cd $HOME/edk2
unlink MikanLoaderPkg
ln -s $OS_DIR/MikanLoaderPkg/ ./
```

## 自作OSのリンク

```bash
cd $HOME/edk2
unlink MikanLoaderPkg
ln -s $CODE_HOME/myos/MikanLoaderPkg/ ./
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