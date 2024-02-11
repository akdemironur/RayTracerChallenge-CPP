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
Color Canvas::pixelAt(int x, int y) const { return pixels[y * width + x]; }
std::string Canvas::PPMHeader() const {
  return "P3\n" + std::to_string(width) + " " + std::to_string(height) +
         "\n255";
}

constexpr int MAX_COLOR_VALUE = 255;

std::string Canvas::PPMBody() const {
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

std::string Canvas::PPM() const { return PPMHeader() + "\n" + PPMBody(); }

void Canvas::savePPM(const std::string &filename) const {
  std::ofstream file;
  file.open(filename);
  file << PPM();
  file.close();
}

} // namespace RT