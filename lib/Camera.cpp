#include "Camera.hpp"
#include "Matrix.hpp"
#include <iostream>
namespace RT {

Camera::Camera(double hsize, double vsize, double fieldOfView,
               Transformation transform)
    : hsize(hsize), vsize(vsize), fieldOfView(fieldOfView),
      transform(transform) {
  auto halfView = std::tan(fieldOfView / 2);
  auto aspect = hsize / vsize;
  if (aspect >= 1) {
    halfWidth = halfView;
    halfHeight = halfView / aspect;
  } else {
    halfWidth = halfView * aspect;
    halfHeight = halfView;
  }
  pixelSize = (halfWidth * 2) / hsize;
}

Ray Camera::rayForPixel(int px, int py) const {
  auto xOffset = (px + 0.5) * pixelSize;
  auto yOffset = (py + 0.5) * pixelSize;
  auto worldX = halfWidth - xOffset;
  auto worldY = halfHeight - yOffset;
  auto pixel = transform.inverse() * point(worldX, worldY, -1);
  auto origin = transform.inverse() * point(0, 0, 0);
  auto direction = (pixel - origin).norm();
  return Ray(origin, direction);
}

Canvas Camera::render(const World &world) const {
  Canvas image(hsize, vsize);
  for (auto y = 0; y < vsize; y++) {
    for (auto x = 0; x < hsize; x++) {
      auto ray = rayForPixel(x, y);
      auto color = world.colorAt(ray);
      image.writePixel(x, y, color);
    }
  }
  return image;
}

} // namespace RT