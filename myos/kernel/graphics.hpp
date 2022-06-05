#pragma once

#include "frame_buffer_config.hpp"

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
  virtual void Write(int x, int y, const PixelColor &c) override;
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter
{
public:
  using PixelWriter::PixelWriter;
  virtual void Write(int x, int y, const PixelColor &c) override;
};