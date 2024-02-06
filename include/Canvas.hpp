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
  void writePixel(int x, int y, Color c);
  Color pixelAt(int x, int y) const;
  std::string PPMHeader() const;
  std::string PPMBody() const;
  std::string PPM() const;
  void savePPM(const std::string &filename) const;
  int width, height;

private:
  std::vector<Color> pixels;
};
} // namespace RT