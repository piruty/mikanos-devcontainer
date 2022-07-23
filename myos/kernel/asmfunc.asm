; asmfunc.asm
;
; System V AMD64 Callng Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

; IOアドレス空間はメモリアドレス空間とは異なっている
; 2つのアドレスを区別するため、IOアドレス空間の読み込みには専用のIO命令が必要
; これらの命令はC++では定義できないため、アセンブラで実装しC++から呼び出して使う

; IOポートアドレスaddrに32ビット変数dataを出力する
global IoOut32 ; void IoOut32(uint16_t addr, uint32_t data);
IoOut32:
  mov dx, di ; dx = addr
  mov eax, esi ; eax = data
  out dx, eax ; dxに指定されたIOアドレスポートに、EAXに設定された32ビット整数を出力
  ret

; IOポートアドレスaddrから32ビット変数を入力して返す
global IoIn32 ; uint32_t IoIn32(uint16_t addr);
IoIn32:
  mov dx, di ; dx = addr
  in eax, dx ; dxに指定されたIOアドレスポートから、EAXに設定された32ビット整数を入力してEAXに設定
  ret