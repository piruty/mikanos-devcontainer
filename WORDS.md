- ELF
  - Executable and Linkable Format
  - コンパイラが生成するオブジェクト および ライブラリとリンクされた実行ファイルのファイルフォーマット
- PHDR
  - ELF プログラムヘッダー
  - セグメントと呼ばれるまとまり
  - p_flagsの情報を持っている
  - p_vaddr
    - セグメントの先頭バイトがある メモリーの仮想アドレスを保持する。
  - p_filesz
    - セグメントのファイルイメージのバイト数を保持する。 これは 0 でもよい。
- EHDR
  - ELF ヘッダー
  
## 参考

- [Executable and Linkable Format - Wikipedia](https://ja.wikipedia.org/wiki/Executable_and_Linkable_Format)
- [Man page of ELF](https://linuxjm.osdn.jp/html/LDP_man-pages/man5/elf.5.html)