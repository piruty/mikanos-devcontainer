#include <cstdint>

// extern "C": C言語形式で関数を定義する
// C++の定義だと、関数をコンパイルしたときに装飾される（マンダリング）ので、これを防ぐ
extern "C" void KernelMain(uint64_t frame_buffer_base, uint64_t frame_buffer_size)
{
  uint8_t *frame_buffer = reinterpret_cast<uint8_t *>(frame_buffer_base);
  for (uint64_t i = 0; i < frame_buffer_size; ++i)
  {
    frame_buffer[i] = i % 256;
  }
  while (1)
    __asm__("hlt"); // __asm__(): インラインアセンブラ。アセンブリ言語の命令を埋め込む
}
