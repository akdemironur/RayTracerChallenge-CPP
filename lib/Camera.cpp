#include "Camera.hpp"
#include "Matrix.hpp"
#include <format>
#include <iostream>
namespace RT {

Camera::Camera(int hsize, int vsize, double fieldOfView,
               Transformation transform)
    : hsize(hsize), vsize(vsize), fieldOfView(fieldOfView),
      transform(transform) {
  auto halfView = std::tan(fieldOfView / 2);
  const double aspect = static_cast<double>(hsize) / vsize;
  if (aspect >= 1) {
    halfWidth = halfView;
    halfHeight = halfView / aspect;
  } else {
    halfWidth = halfView * aspect;
    halfHeight = halfView;
  }
  pixelSize = (halfWidth * 2) / hsize;
}

auto Camera::rayForPixel(int pixelX, int pixelY) const -> Ray {
  const double pixelOffset = 0.5;
  auto xOffset = (pixelX + pixelOffset) * pixelSize;
  auto yOffset = (pixelY + pixelOffset) * pixelSize;
  auto worldX = halfWidth - xOffset;
  auto worldY = halfHeight - yOffset;
  auto pixel = transform.inverse() * point(worldX, worldY, -1);
  auto origin = transform.inverse() * point(0, 0, 0);
  auto direction = (pixel - origin).norm();
  return {origin, direction};
}

auto Camera::render(const World &world) const -> Canvas {
  Canvas image(hsize, vsize);
  const double pixelProgress = 1.0 / (hsize * vsize);
  double progress = 0;
  const int barWidth = 10;
  std ::cout << "Rendering: [";
  for (int i = 0; i < barWidth; i++) {
    std::cout << " ";
  }
  std::cout << "]\rRendering: [" << std::flush;
  for (auto y = 0; y < vsize; y++) {
    for (auto x = 0; x < hsize; x++) {
      auto ray = rayForPixel(x, y);
      auto color = world.colorAt(ray);
      image.writePixel(x, y, color);
      progress += pixelProgress;
      if (progress >= 1.0 / barWidth) {
        progress -= 1.0 / barWidth;
        std::cout << "=" << std::flush;
      }
    }
  }
  std::cout << "=]\n";
  return image;
}

} // namespace RT