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

namespace GLSL {

template <typename T, size_t N>
class vec {

public:

  template <typename... ArgsT>
    requires (std::same_as<ArgsT, T> && ...) && (sizeof...(ArgsT) == N)
  vec(ArgsT... args) : m_Array{args...} {}

  vec() : m_Array{} {}
  vec(T x) { m_Array.fill(x); }

  vec operator+(const vec& b) const { return generic_binary_operator<std::plus<>>(b); }
  vec operator-(const vec& b) const { return generic_binary_operator<std::minus<>>(b); }
  vec operator*(const vec& b) const { return generic_binary_operator<std::multiplies<>>(b); }
  vec operator/(const vec& b) const { return generic_binary_operator<std::divides<>>(b); }
  
  vec operator+(T b) const { return generic_binary_operator<std::plus<>>(b); }
  vec operator-(T b) const { return generic_binary_operator<std::minus<>>(b); }
  vec operator*(T b) const { return generic_binary_operator<std::multiplies<>>(b); }
  vec operator/(T b) const { return generic_binary_operator<std::divides<>>(b); }

  friend vec operator+(T b, const vec& v) { return v + b; } 
  friend vec operator-(T b, const vec& v) { return static_cast<T>(-1) * v + b; } 
  friend vec operator*(T b, const vec& v) { return v * b; } 

  vec& operator+=(const vec& b) { return generic_assignment_operator<std::plus<>>(b); }
  vec& operator-=(const vec& b) { return generic_assignment_operator<std::minus<>>(b); }
  vec& operator*=(const vec& b) { return generic_assignment_operator<std::multiplies<>>(b); }
  vec& operator/=(const vec& b) { return generic_assignment_operator<std::divides<>>(b); }

  const auto& GetData() const { return m_Array; }
  auto& GetData() { return m_Array; }

  auto  x() const { return m_Array[0]; }
  auto& x()       { return m_Array[0]; }
  auto  y() const { return m_Array[1]; }
  auto& y()       { return m_Array[1]; }
  auto  z() const { return m_Array[2]; }
  auto& z()       { return m_Array[2]; }

  vec<T,4> xyyx() const
  {
    return vec<T,4>{x(), y(), y(), x()};
  }
  vec<T,2> yx() const
  {
    return vec<T,2>{y(), x()};
  }

private:
  template<class Op>
  vec generic_binary_operator(const vec& b) const
  {
    vec c;
    std::ranges::transform(m_Array, b.m_Array, c.m_Array.begin(), Op{});
    return c;
  }

  template <class Op>
  vec& generic_assignment_operator(const vec& b)
  {
    std::ranges::transform(m_Array, b.m_Array, m_Array.begin(), Op{});
    return *this;
  }

  template <class Op>
  vec generic_binary_operator(T b) const
  {
    vec c;
    std::ranges::transform(m_Array, std::views::repeat(b), c.m_Array.begin(), Op{});
    return c;
  }

  std::array<T,N> m_Array;
};

template<class T, size_t N>
vec<T,N> normalize(const vec<T,N>& v) {
  const auto& data = v.GetData();
  auto l = std::accumulate(data.begin(), data.end(), 0.0f, [](float acc, float x) { return acc + x*x; } );
  std::println("sum = {}", l);
  return v / std::sqrtf(l);
}

// Two implementation of dot - for -O0 the dot_2 is significantly faster,
// for -01 the dot_1 was faster,
// for -02 and -O3 the times are identical
// (clang version 21.1.5)

template<class T, size_t N>
T dot_1(const vec<T,N> &a, const vec<T,N> &b)
{
  auto z = std::views::zip(a.GetData(), b.GetData());
  auto res = std::accumulate(z.begin(), z.end(), 0.0f, [](float acc, const auto& x) { return acc + std::get<0>(x) * std::get<1>(x); });
  return static_cast<T>(res);
}

template<class T, size_t N>
T dot_2(const vec<T,N> &a, const vec<T,N> &b)
{
  const T* pA = a.GetData().data();
  const T* pB = b.GetData().data();
  T sum{};
  for (size_t i = 0; i < N; i++)
  {
    sum += pA[i] * pB[i];
  }
  return sum;
}

template<class T, size_t N>
T dot(const vec<T,N> &a, const vec<T,N> &b)
{
  return dot_1(a,b);
}

template <class T, size_t N>
vec<T,N> sin(const vec<T,N> &a) {
  vec<T,N> res;
  std::ranges::transform(a.GetData(), res.GetData().begin(), [](auto x) { return std::sin(x); });
  return res;
}

template <class T, size_t N>
vec<T,N> cos(const vec<T,N> &a) {
  vec<T,N> res;
  std::ranges::transform(a.GetData(), res.GetData().begin(), [](auto x) { return std::cos(x); });
  return res;
}

template <class T, size_t N>
vec<T,N> exp(const vec<T,N> &a) {
  vec<T,N> res;
  std::ranges::transform(a.GetData(), res.GetData().begin(), [](auto x) { return std::exp(x); });
  return res;
}

template <class T, size_t N>
vec<T,N> tanh(const vec<T,N> &a) {
  vec<T,N> res;
  std::ranges::transform(a.GetData(), res.GetData().begin(), [](auto x) { return std::tanh(x); });
  return res;
}

template <class T, size_t N>
vec<T,N> abs(const vec<T,N> &a) {
  vec<T,N> res;
  std::ranges::transform(a.GetData(), res.GetData().begin(), [](auto x) { return std::fabs(x); });
  return res;
}

using vec2 = vec<float,2>;
using vec4 = vec<float,4>;


} // namespace utils

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {

  using namespace GLSL;

  vec<float,4> test({1.0f,2.0f,3.0f,4.0f});


  const uint32_t width = 960;
  const uint32_t height = 540;
  const uint32_t animation_steps = 240;

  for (uint32_t step = 0; step < animation_steps; step++) {
    float t = ((float)step / 240) * 2 * std::numbers::pi;
    std::string filename = std::format("image-{}.ppm", step);
    auto image = image::PPM{filename, width, height};
    for (uint32_t h = 0; h < height; h++) {
      for (uint32_t w = 0; w < width; w++) {

        #include <examples/headers/plasma.hpp>

        image.WritePixel(image::color{static_cast<uint8_t>(output.x() * 255.0f),
                               static_cast<uint8_t>(output.y() * 255.0f),
                               static_cast<uint8_t>(output.z() * 255.0f)});
      }
    }
    std::println("Rendered image {}", filename);
  }
}
