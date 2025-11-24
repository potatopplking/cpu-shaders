#pragma once

#include <initializer_list>
#include <filesystem>
#include <fstream>
#include <math.h>
#include <print>
#include <utility>
#include <numeric>
#include <array>
#include <cstddef>
#include <print>
#include <algorithm>
#include <functional>
#include <ranges>
#include <cmath>
#include <chrono>
#include <numbers>


namespace image {

namespace fs = std::filesystem;

struct color {
  uint8_t R, G, B;
};

class PPM {
public:
  PPM(fs::path p, uint32_t width, uint32_t height)
      : m_FileStream(p, std::ios::binary), m_Width(width), m_Height(height) {
    WriteHeader();
  }

  void WriteHeader() {
    constexpr uint32_t max_value = 255;
    m_FileStream << "P6" << std::endl
                 << m_Width << " " << m_Height << std::endl
                 << max_value << std::endl;
  }

  void WritePixel(color c) { m_FileStream << c.R << c.G << c.B; }

  uint32_t GetHeight() const { return m_Height; }
  uint32_t GetWidth() const { return m_Width; }

private:
  std::ofstream m_FileStream;
  uint32_t m_Width;
  uint32_t m_Height;
};

} // namespace image

