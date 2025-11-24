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

#include "GLSL.hpp"
#include "image.hpp"

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
