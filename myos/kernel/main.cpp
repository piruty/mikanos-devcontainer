#include <cstdint>
#include <cstddef>

#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "font.hpp"

// 配置newの演算子の定義
void *operator new(size_t size, void *buf)
{
  return buf;
}

// 配置newの演算子の定義
void operator delete(void *obj) noexcept
{
}

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter *pixel_writer;

// extern "C": C言語形式で関数を定義する
// C++の定義だと、関数をコンパイルしたときに装飾される（マンダリング）ので、これを防ぐ
extern "C" void KernelMain(const FrameBufferConfig &frame_buffer_config)
{
  switch (frame_buffer_config.pixel_format)
  {
  case kPixelRGBResv8BitPerColor:
    // new演算子が引数を撮っている = 配置 new
    pixel_writer = new (pixel_writer_buf) RGBResv8BitPerColorPixelWriter{frame_buffer_config};
    break;
  case kPixelBGRResv8BitPerColor:
    pixel_writer = new (pixel_writer_buf) BGRResv8BitPerColorPixelWriter{frame_buffer_config};
    break;
  }
  // 全体を白で塗りつぶす
  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x)
  {
    for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y)
    {
      pixel_writer->Write(x, y, {255, 255, 255});
    }
  }
  for (int x = 0; x < 200; ++x)
  {
    for (int y = 0; y < 200; ++y)
    {
      pixel_writer->Write(x, y, {0, 255, 0});
    }
  }
  // #@@ write_fonts begin
  int i = 0;
  for (char c = '!'; c <= '~'; ++c, ++i) {
    WriteAscii(*pixel_writer, 8 * i, 50, c, {0, 0, 0});
  }
  // #@@ write_fonts end
  while (1)
    __asm__("hlt"); // __asm__(): インラインアセンブラ。アセンブリ言語の命令を埋め込む
}
