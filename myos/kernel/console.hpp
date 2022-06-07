#pragma once

#include "graphics.hpp"

class Console {
  public:
    // コンソールの表示領域(静的変数)
    static const int kRows = 25, kColumns = 80;

    Console(PixelWriter& writer, const PixelColor& fg_color, const PixelColor& bg_color);
    // 与えられた文字列を表示する関数
    void PutString(const char* s);

  private:
    // 改行処理
    void Newline();

    PixelWriter& writer_;
    const PixelColor fg_color_, bg_color_;
    // コンソールに表示されている文字列
    // +1 = 行末の\0分
    char buffer_[kRows][kColumns + 1];
    // 文字列表示のカーソル位置
    int cursor_row_, cursor_column_;
};