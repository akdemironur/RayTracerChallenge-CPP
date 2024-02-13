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
auto Canvas::PPMHeader() const -> std::vector<unsigned char> {
  std::string headerString =
      "P6\n" + std::to_string(width) + " " + std::to_string(height) + "\n255\n";
  std::vector<unsigned char> header(headerString.begin(), headerString.end());
  return header;
}

constexpr int MAX_COLOR_VALUE = 255;

auto Canvas::PPMBody() const -> std::vector<unsigned char> {
  auto normalize = [](double d) {
    return static_cast<unsigned char>(
        std::max(std::min(static_cast<int>(std::lrint(MAX_COLOR_VALUE * d)),
                          MAX_COLOR_VALUE),
                 0));
  };

  std::vector<unsigned char> body;
  for (const auto &p : pixels) {
    body.push_back(normalize(p.red));
    body.push_back(normalize(p.green));
    body.push_back(normalize(p.blue));
  }
  return body;
}

auto Canvas::PPM() const -> std::vector<unsigned char> {
  std::vector<unsigned char> ppmData;
  auto header = PPMHeader();
  ppmData.insert(ppmData.end(), header.begin(), header.end());
  std::vector<unsigned char> body = PPMBody();
  ppmData.insert(ppmData.end(), body.begin(), body.end());
  return ppmData;
}

void Canvas::savePPM(const std::string &filename) const {
  std::ofstream file;
  file.open(filename, std::ios::binary);
  std::vector<unsigned char> ppmData = PPM();
  file.write(reinterpret_cast<char *>(ppmData.data()),
             static_cast<std::streamsize>(ppmData.size()));
  file.close();
}

} // namespace RT