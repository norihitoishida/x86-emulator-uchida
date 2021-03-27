# x86-emulator-uchida
- 自作エミュレータで学ぶx86アーキテクチャ(内田 上川, 2015)
- [サポートページ](https://book.mynavi.jp/support/bookmook/x86/)
- Win向けに書かれているのでLinux用に書き換える(Makefile等)

# Table of contents
- Chapter 1: C言語とアセンブリ言語
    - ndisasmでバイナリを逆アセンブル(NASMは標準でIntel syntax)
    - objdumpでオブジェクトファイルを逆アセンブル(objdumpは標準でAT&T syntax)
    - ESP/RSP:スタックポインタ(スタック領域の一番上のアドレスを記憶するレジスタ)
    - EBP/RBP:ベースポインタ(スタック領域の一番下のアドレスを記憶するレジスタ)
    - 2進/16進
- Chapter 2: ポインタとアセンブリ言語
    - gcc, nasm, ldの主要オプション
- Chapter 3: CPUがプログラムを実行する仕組み
- Chapter 4: BIOSの仕組みと実機起動