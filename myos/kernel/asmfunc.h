#pragma once // コンパイル時に、ヘッダーファイルを1回だけインクルードする

#include <stdint.h>

// extern "C": Cの言語リンケージを使用する
// C言語で記述された関数からC++関数を呼び出せるようにする
// 指定しないと、コンパイラが裏で関数名などを一意にリネームしてしまう
extern "C" {
  void IoOut32(uint16_t addr, uint32_t data);
  uint32_t IoIn32(uint16_t addr);
}