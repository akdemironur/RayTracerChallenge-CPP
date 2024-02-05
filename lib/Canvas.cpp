#include "Canvas.hpp"

namespace RT {

Canvas::Canvas(int width, int height) : width(width), height(height) {
  pixels = std::vector<Color>(height * width, color(0, 0, 0));
}
void Canvas::writePixel(int x, int y, Color c) { pixels[y * width + x] = c; }
Color Canvas::pixelAt(int x, int y) const { return pixels[y * width + x]; }
std::string Canvas::PPMHeader() const {
  return "P3\n" + std::to_string(width) + " " + std::to_string(height) +
         "\n255";
}

std::string Canvas::PPMBody() const {
  std::string body;
  auto normalize = [](double d) {
    return std::to_string(std::max(std::min(int(std::lrint(255 * d)), 255), 0));
  };
  for (auto &p : pixels) {
    body += normalize(p.red) + " " + normalize(p.green) + " " +
            normalize(p.blue) + "\n";
  }
  return body;
}

std::string Canvas::PPM() const { return PPMHeader() + "\n" + PPMBody(); }

} // namespace RT