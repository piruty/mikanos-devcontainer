#include <cstdint>
#include <cstddef>

#include "frame_buffer_config.cpp"

struct PixelColor
{
  uint8_t r, g, b;
};

// 1つの点を描画
// 0: 成功, -1: 失敗
int WritePixel(const FrameBufferConfig &config, int x, int y, const PixelColor &c)
{
  const int pixel_position = config.pixels_per_scan_line * y + x;
  if (config.pixel_format == kPixelRGBResv8BitPerColor)
  {
    uint8_t *p = &config.frame_buffer[4 * pixel_position];
    p[0] = c.r;
    p[1] = c.g;
    p[2] = c.b;
  }
  else if (config.pixel_format == kPixelBGRResv8BitPerColor)
  {
    uint8_t *p = &config.frame_buffer[4 * pixel_position];
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
  }
  else
  {
    return -1;
  }
  return 0;
}

// extern "C": C言語形式で関数を定義する
// C++の定義だと、関数をコンパイルしたときに装飾される（マンダリング）ので、これを防ぐ
extern "C" void KernelMain(const FrameBufferConfig &frame_buffer_config)
{
  // 全体を白で塗りつぶす
  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x)
  {
    for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y)
    {
      WritePixel(frame_buffer_config, x, y, {255, 255, 255});
    }
  }
  for (int x = 0; x < 200; ++x)
  {
    for (int y = 0; y < 200; ++y)
    {
      WritePixel(frame_buffer_config, 100 + x, 100 + y, {0, 255, 0});
    }
  }
  while (1)
    __asm__("hlt"); // __asm__(): インラインアセンブラ。アセンブリ言語の命令を埋め込む
}
