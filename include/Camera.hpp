
#pragma once
#include "Canvas.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "World.hpp"
namespace RT {

class Camera {
public:
  Camera(double hsize, double vsize, double fieldOfView,
         Transformation transform = identityMatrix<4>());
  double hsize;
  double vsize;
  double fieldOfView;
  Transformation transform;
  double pixelSize;
  double halfWidth;
  double halfHeight;
  Ray rayForPixel(int px, int py) const;
  Canvas render(const World &world) const;
};

} // namespace RT