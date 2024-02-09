#include "Matrix.hpp"
#define private public
#include <RT.hpp>
#include <iostream>
#include <memory>

int main() {

  std::unique_ptr<RT::Shape> middle = std::make_unique<RT::Sphere>();
  middle->transformation = RT::translation(-0.5, 1, 0.5);
  middle->material.color = RT::color(0.1, 1, 0.5);
  middle->material.diffuse = 0.7;
  middle->material.specular = 0.3;
  middle->material.reflective = 0.9;
  middle->material.refractiveIndex = 1.5;
  middle->material.transparency = 0;

  std::unique_ptr<RT::Shape> right = std::make_unique<RT::Cube>();
  right->transformation =
      RT::translation(1.5, 0.5, -0.5) * RT::scaling(0.5, 0.5, 0.5);
  right->material.color = RT::color(0.5, 1, 0.1);
  right->material.diffuse = 0.7;
  right->material.specular = 0.3;

  std::unique_ptr<RT::Shape> left = std::make_unique<RT::Cube>();
  left->transformation =
      RT::translation(-1.5, 0.33, -0.75) * RT::scaling(0.33, 0.33, 0.33);
  left->material.color = RT::color(0.5, 1, 0.1);
  left->material.diffuse = 0.7;
  left->material.specular = 0.3;

  std::unique_ptr<RT::Shape> bot = std::make_unique<RT::Plane>();
  bot->transformation = RT::translation(0, 0, 0);

  std::unique_ptr<RT::Shape> backWall = std::make_unique<RT::Plane>();
  backWall->transformation = RT::rotationZ(M_PI / 2) >>=
      RT::translation(5, 0, 5);
  backWall->material.color = RT::color(0.5, 1, 0.1);

  backWall->material.diffuse = 0.7;
  backWall->material.specular = 0.3;
  backWall->material.reflective = 0.9;

  auto world = RT::World(false);
  world.light = RT::Light(RT::point(-10, 10, -10), RT::color(1, 1, 1));

  world.add(std::move(middle));
  world.add(std::move(right));
  world.add(std::move(left));
  world.add(std::move(bot));
  world.add(std::move(backWall));

  auto camera = RT::Camera(2560, 1440, M_PI / 3);
  camera.transform = RT::viewTransform(RT::point(0, 1.5, -5),
                                       RT::point(0, 1, 0), RT::vector(0, 1, 0));

  auto canvas = camera.render(world);
  canvas.savePPM("sample.ppm");

  std::cout << world.objects[2]->material.color << std::endl;

  return 0;
}
