#include "Canvas.hpp"

#include <utility>

namespace RT {

Canvas::Canvas(int width, int height) : width(width), height(height) {
  pixels =
      std::vector<Color>(static_cast<size_t>(height * width), color(0, 0, 0));
}
void Canvas::writePixel(int x, int y, Color c) {
  pixels[y * width + x] = std::move(c);
}
auto Canvas::pixelAt(int x, int y) const -> Color {
  return pixels[y * width + x];
}
auto Canvas::PPMHeader() const -> std::string {
  return "P3\n" + std::to_string(width) + " " + std::to_string(height) +
         "\n255";
}

constexpr int MAX_COLOR_VALUE = 255;

auto Canvas::PPMBody() const -> std::string {
  std::string body;
  auto normalize = [](double d) {
    return std::to_string(std::max(
        std::min(int(std::lrint(MAX_COLOR_VALUE * d)), MAX_COLOR_VALUE), 0));
  };
  for (const auto &p : pixels) {
    body += normalize(p.red) + " " + normalize(p.green) + " " +
            normalize(p.blue) + "\n";
  }
  return body;
}

auto Canvas::PPM() const -> std::string {
  return PPMHeader() + "\n" + PPMBody();
}

void Canvas::savePPM(const std::string &filename) const {
  std::ofstream file;
  file.open(filename);
  file << PPM();
  file.close();
}

} // namespace RT