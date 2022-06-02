#include <cstdint>
#include <cstddef>

#include "frame_buffer_config.hpp"

// font_a begin
// 横8ピクセル、高さ16ピクセル
const uint8_t kFontA[16] = {
    0b00000000, //
    0b00011000, //    **
    0b00011000, //    **
    0b00011000, //    **
    0b00011000, //    **
    0b00100100, //   *  *
    0b00100100, //   *  *
    0b00100100, //   *  *
    0b00100100, //   *  *
    0b01111110, //  ******
    0b01000010, //  *    *
    0b01000010, //  *    *
    0b01000010, //  *    *
    0b11100111, // ***  ***
    0b00000000, //
    0b00000000, //
};
// font_a end

struct PixelColor
{
  uint8_t r, g, b;
};

// 画面描画を行うためのインターフェース
class PixelWriter
{
public:
  PixelWriter(const FrameBufferConfig &config) : config_{config} {}
  // ~付き = デストラクタ
  virtual ~PixelWriter() = default;
  // =0 で終わっている = 純粋仮想関数（処理の内容は決まっていないが、戻り値と引数、関数名は決まっている）
  virtual void Write(int x, int y, const PixelColor &c) = 0;

protected:
  uint8_t *PixelAt(int x, int y)
  {
    return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
  }

private:
  const FrameBufferConfig &config_;
};

class RGBResv8BitPerColorPixelWriter : public PixelWriter
{
public:
  // usingを使うことで、親のコンストラクタをそのまま使うことができる
  using PixelWriter::PixelWriter;

  virtual void Write(int x, int y, const PixelColor &c) override
  {
    auto p = PixelAt(x, y);
    p[0] = c.r;
    p[1] = c.g;
    p[2] = c.b;
  }
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter
{
public:
  using PixelWriter::PixelWriter;

  virtual void Write(int x, int y, const PixelColor &c) override
  {
    auto p = PixelAt(x, y);
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
  }
};

// #@@ write_ascii begin
void WriteAscii(PixelWriter &writer, int x, int y, char c, const PixelColor &color)
{
  if (c != 'A')
  {
    return;
  }
  for (int dy = 0; dy < 16; ++dy)
  {
    for (int dx = 0; dx < 16; ++dx)
    {
      if ((kFontA[dy] << dx) & 0x80u)
      {
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}
// #@@ write_ascii end

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
  // #@@ write_aa begin
  WriteAscii(*pixel_writer, 50, 50, 'A', {0, 0, 0});
  WriteAscii(*pixel_writer, 58, 50, 'A', {0, 0, 0});
  // #@@ write_aa end
  while (1)
    __asm__("hlt"); // __asm__(): インラインアセンブラ。アセンブリ言語の命令を埋め込む
}
