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
  [[nodiscard]] Ray rayForPixel(int pixelX, int pixelY) const;
  [[nodiscard]] Canvas render(const World &world) const;
};

} // namespace RT