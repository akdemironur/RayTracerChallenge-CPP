#include <RT.hpp>
#include <iostream>

int main() {

  auto floor = RT::Sphere();

  floor.transformation = RT::scaling(10, 0.01, 10);
  floor.material.color = RT::color(1, 0.9, 0.9);
  floor.material.specular = 0;

  auto leftWall = RT::Sphere();
  leftWall.transformation = RT::translation(0, 0, 5) *
                            RT::rotationY(-M_PI / 4) * RT::rotationX(M_PI / 2) *
                            RT::scaling(10, 0.01, 10);
  leftWall.material = floor.material;

  auto rightWall = RT::Sphere();
  rightWall.transformation = RT::translation(0, 0, 5) *
                             RT::rotationY(M_PI / 4) * RT::rotationX(M_PI / 2) *
                             RT::scaling(10, 0.01, 10);
  rightWall.material = floor.material;

  auto middle = RT::Sphere();
  middle.transformation = RT::translation(-0.5, 1, 0.5);
  middle.material.color = RT::color(0.1, 1, 0.5);
  middle.material.diffuse = 0.7;
  middle.material.specular = 0.3;

  auto right = RT::Sphere();
  right.transformation =
      RT::translation(1.5, 0.5, -0.5) * RT::scaling(0.5, 0.5, 0.5);
  right.material.color = RT::color(0.5, 1, 0.1);
  right.material.diffuse = 0.7;
  right.material.specular = 0.3;

  auto left = RT::Sphere();
  left.transformation =
      RT::translation(-1.5, 0.33, -0.75) * RT::scaling(0.33, 0.33, 0.33);
  left.material.color = RT::color(1, 0.8, 0.1);
  left.material.diffuse = 0.7;
  left.material.specular = 0.3;

  auto world = RT::World(false);
  world.light = RT::Light(RT::point(-10, 10, -10), RT::color(1, 1, 1));
  world.add(floor);
  world.add(leftWall);
  world.add(rightWall);
  world.add(middle);
  world.add(right);
  world.add(left);

  auto camera = RT::Camera(2560, 1440, M_PI / 3);
  camera.transform = RT::viewTransform(RT::point(0, 1.5, -5),
                                       RT::point(0, 1, 0), RT::vector(0, 1, 0));

  auto canvas = camera.render(world);
  canvas.savePPM("sample.ppm");

  return 0;
}
