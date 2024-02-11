#pragma once
#include "Canvas.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "World.hpp"
namespace RT {

class Camera {
public:
  Camera(int hsize, int vsize, double fieldOfView,
         Transformation transform = identityMatrix<4>());
  int hsize;
  int vsize;
  double fieldOfView;
  Transformation transform;
  double pixelSize;
  double halfWidth;
  double halfHeight;
  [[nodiscard]] auto rayForPixel(int pixelX, int pixelY) const -> Ray;
  [[nodiscard]] auto render(const World &world) const -> Canvas;
};

} // namespace RT