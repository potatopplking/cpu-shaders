#include <initializer_list>
#include <filesystem>
#include <fstream>
#include <math.h>
#include <print>
#include <utility>

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
  vec(ArgsT... args) : m_Data{args...} {}

  vec operator+(const vec& b)
  {
    vec c;
    for (size_t i = 0; i < m_Data.size(); i++) {}
    return c;
  }

private:
  std::array<T,N> m_Data;
};

template <typename T, size_t N>
vec<T,N> operator+(const vec<T,N>& a, const vec<T,N>& b)
{
  return vec<T,N>{};
}

using nvec4 = vec<float,4>;

// Math
struct vec4 {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
  float w = 0.0f;

  vec4(float a) : x{a}, y{a}, z{a}, w{a} {} // scalar promoted to vec
  vec4() : vec4(0, 0, 0, 0) {}
  vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

struct vec2 {
  float x, y;

  vec2(float a) : x{a}, y{a} {} // scalar promoted to vec
  vec2() : vec2(0, 0) {}
  vec2(float x, float y) : x{x}, y{y} {}

  vec4 xyyx() { return vec4{x, y, y, x}; }
  vec2 yx() { return vec2{y, x}; }
};

vec2 operator+(const vec2 &a, const vec2 &b) {
  return vec2{a.x + b.x, a.y + b.y};
}

vec2 operator*(const vec2 &a, float b) { return vec2{a.x * b, a.y * b}; }

vec2 operator/(const vec2 &a, float b) { return vec2{a.x / b, a.y / b}; }

vec2 operator-(const vec2 &a, const vec2 &b) {
  return vec2{a.x - b.x, a.y - b.y};
}

vec2 &operator+=(vec2 &a, const vec2 &b) {
  a.x += b.x;
  a.y += b.y;
  return a;
}

vec2 operator-(float a, const vec2 &b) {
  // GLSL behaviour: float is promoted to vec, then subtracted
  return vec2{a - b.x, a - b.y};
}

vec2 operator+(const vec2 &a, float b) { return vec2{a.x + b, a.y + b}; }

vec2 operator*(const vec2 &a, const vec2 &b) {
  return vec2{a.x * b.x, a.y * b.y};
}

vec4 operator/(vec4 a, float b) {
  return vec4{a.x / b, a.y / b, a.z / b, a.w / b};
}

vec4 operator-(const vec4 &a, const vec4 &b) {
  return vec4{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

vec4 operator+(const vec4 &a, const vec4 &b) {
  return vec4{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

vec4 operator+(const vec4 &a, float b) {
  return vec4{a.x + b, a.y + b, a.z + b, a.w + b};
}

vec4 &operator+=(vec4 &a, const vec4 &b) {
  a.x += b.x;
  a.y += b.y;
  a.z += b.z;
  a.w += b.w;
  return a;
}

vec4 operator*(float a, const vec4 &b) {
  return vec4{b.x * a, b.y * a, b.z * a, b.w * a};
}

vec4 operator*(const vec4 &a, float b) { return b * a; }

vec4 operator/(const vec4 &a, const vec4 &b) {
  return vec4{a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

vec4 normalize(vec4 v) {
  float l = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
  return v / l;
}

float dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }

vec4 sin(const vec4 &a) {
  return vec4{std::sin(a.x), std::sin(a.y), std::sin(a.z), std::sin(a.w)};
}

vec2 cos(const vec2 &a) { return vec2{std::cos(a.x), std::cos(a.y)}; }

vec4 exp(const vec4 &a) {
  return vec4{std::exp(a.x), std::exp(a.y), std::exp(a.z), std::exp(a.w)};
}

vec4 tanh(const vec4 &a) {
  return vec4{std::tanh(a.x), std::tanh(a.y), std::tanh(a.z), std::tanh(a.w)};
}

vec2 abs(const vec2 &v) { return vec2{std::fabs(v.x), std::fabs(v.y)}; }

} // namespace utils

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {

  using namespace GLSL;

  vec<float,4> test({1.0f,2.0f,3.0f,4.0f});


  const uint32_t width = 960;
  const uint32_t height = 540;
  const uint32_t animation_steps = 240;

  for (uint32_t step = 0; step < animation_steps; step++) {
    float t = ((float)step / 240) * 2 * M_PI;
    std::string filename = std::format("image-{}.ppm", step);
    auto image = image::PPM{filename, width, height};
    for (uint32_t h = 0; h < height; h++) {
      for (uint32_t w = 0; w < width; w++) {

        // GLSL prep
        vec2 FC{static_cast<float>(w), static_cast<float>(h)};
        const vec2 r{static_cast<float>(width), static_cast<float>(height)};
        vec4 o;

        //////////////////////////////
        // https://x.com/XorDev/status/1894123951401378051
        vec2 p = (FC * 2. - r) / r.y, l, i,
             v = p * (l += 4. - 4. * abs(.7 - dot(p, p)));
        for (; i.y++ < 8.; o += (sin(v.xyyx()) + 1.) * abs(v.x - v.y))
          v += cos(v.yx() * i.y + i + t) / i.y + .7;
        o = tanh(5. * exp(l.x - 4. - p.y * vec4(-1, 1, 2, 0)) / o);
        //////////////////////////////

        image.WritePixel(image::color{static_cast<uint8_t>(o.x * 255.0f),
                               static_cast<uint8_t>(o.y * 255.0f),
                               static_cast<uint8_t>(o.z * 255.0f)});
      }
    }
    std::println("Rendered image {}", filename);
  }
}
