#include "font.hpp"

// hankaku_bin begin
extern const uint8_t _binary_hankaku_bin_start;
extern const uint8_t _binary_hankaku_bin_end;
extern const uint8_t _binary_hankaku_bin_size;

const uint8_t *GetFont(char c)
{
  // auto??
  auto index = 16 * static_cast<unsigned int>(c);
  if (index >= reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size))
  {
    return nullptr;
  }
  return &_binary_hankaku_bin_start + index;
}
// hankaku_bin end

void WriteAscii(PixelWriter &writer, int x, int y, char c, const PixelColor &color)
{
  const uint8_t *font = GetFont(c);
  if (font == nullptr)
  {
    return;
  }
  for (int dy = 0; dy < 16; ++dy)
  {
    for (int dx = 0; dx < 16; ++dx)
    {
      if ((font[dy] << dx) & 0x80u)
      {
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}

// #@@ write_string begin
// 渡された文字列を表示する関数
void WriteString(PixelWriter& writer, int x, int y, const char* s, const PixelColor& color)
{
  for (int i = 0; s[i] != '\0'; ++i)
  {
    WriteAscii(writer, x + 8 * i, y, s[i], color);
  }
}
// #@@ write_string end