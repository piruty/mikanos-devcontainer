## xquartzで動かないとき

1. xtermで以下を実行
  - `xhost + 127.0.0.1`
2. devcontainerの設定でコメントアウトを外さない
  - 外すとqemu経由で動かなかった

```bash
cd $HOME/edk2
ln -s $OS_DIR/MikanLoaderPkg/ ./
```

## Conf設定の更新

```bash
cp $CODE_HOME/Conf/target.txt $HOME/edk2/Conf/target.txt
cp $CODE_HOME/Conf/tools_def.txt $HOME/edk2/Conf/tools_def.txt
```

## build結果の実行

```bash
~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```