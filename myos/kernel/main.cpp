#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "frame_buffer_config.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"

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

// console_buf begin
char console_buf[sizeof(Console)];
Console *console;
// console_buf end

// printk begin
// printk: カーネル内部からメッセージを出すための関数。
// カーネル内ならどこでも使える。書式整形の機能を持つ
int printk(const char *format, ...) // ...: 可変長引数
{
  va_list ap; // va_list: 可変長引数を受け取る
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}
// printk end

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
  // new_console begin
  console = new (console_buf) Console{*pixel_writer, {0, 0, 0}, {255, 255, 255}};
  // new_console end

  for (int i = 0; i < 27; ++i)
  {
    printk("printk: %d\n", i);
  }

  while (1)
    __asm__("hlt"); // __asm__(): インラインアセンブラ。アセンブリ言語の命令を埋め込む
}
