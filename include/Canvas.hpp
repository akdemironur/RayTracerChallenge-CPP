#pragma once
#include "Tuple.hpp"
#include "Util.hpp"
#include <fstream>
#include <string>
#include <vector>
namespace RT {

class Canvas {
public:
  Canvas(int width, int height);
  void writePixel(int pixelX, int pixelY, Color color);
  [[nodiscard]] auto pixelAt(int pixelX, int pixelY) const -> Color;
  [[nodiscard]] auto PPMHeader() const -> std::string;
  [[nodiscard]] auto PPMBody() const -> std::string;
  [[nodiscard]] auto PPM() const -> std::string;
  void savePPM(const std::string &filename) const;
  int width, height;

private:
  std::vector<Color> pixels;
};
} // namespace RT