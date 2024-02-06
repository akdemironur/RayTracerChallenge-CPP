#include <RT.hpp>
#include <iostream>

int main() {
  auto canvas_pixels = 100;
  auto ray_origin = RT::point(0, 0, -5);
  auto wall_z = 10.0;
  auto wall_size = 7.0;
  auto pixel_size = wall_size / canvas_pixels;
  auto half = wall_size / 2;
  auto canvas = RT::Canvas(canvas_pixels, canvas_pixels);
  auto color = RT::color(1, 0, 0);
  auto sphere = RT::Sphere();
  for (auto y = 0; y < canvas_pixels; y++) {
    auto world_y = half - pixel_size * y;
    for (auto x = 0; x < canvas_pixels; x++) {
      auto world_x = -half + pixel_size * x;
      auto position = RT::point(world_x, world_y, wall_z);
      auto r = RT::Ray(ray_origin, (position - ray_origin).normalize());
      auto xs = RT::intersect(sphere, r);
      auto hit = RT::hit(xs);
      if (hit) {
        canvas.writePixel(x, y, color);
      }
    }
  }
  canvas.savePPM("sphere.ppm");

  return 0;
}
