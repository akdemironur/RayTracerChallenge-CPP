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
  [[nodiscard]] Color pixelAt(int pixelX, int pixelY) const;
  [[nodiscard]] std::string PPMHeader() const;
  [[nodiscard]] std::string PPMBody() const;
  [[nodiscard]] std::string PPM() const;
  void savePPM(const std::string &filename) const;
  int width, height;

private:
  std::vector<Color> pixels;
};
} // namespace RT