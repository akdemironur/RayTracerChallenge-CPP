#include "Matrix.hpp"
#include "Pattern.hpp"
#include "Tuple.hpp"
#include <RT.hpp>
#include <iostream>
#include <memory>
#include <random>

auto main() -> int {

  std::unique_ptr<RT::Shape> middle = std::make_unique<RT::Sphere>();
  middle->transformation = RT::translation(-0.5, 1, 0.5);
  middle->material.color = RT::color(1, 1, 1);
  middle->material.ambient = 0;
  middle->material.diffuse = 0;
  middle->material.specular = 0.9;
  middle->material.reflective = 1;
  middle->material.refractiveIndex = 1.52;
  middle->material.transparency = 0.5;
  middle->material.shininess = 300;

  std::unique_ptr<RT::Shape> bot = std::make_unique<RT::Plane>();
  bot->transformation = RT::translation(0, 0, 0);
  bot->material.color = RT::color(1, 1, 1);
  bot->material.diffuse = 0.9;
  bot->material.specular = 0.1;
  bot->material.ambient = 0.5;
  bot->material.pattern = std::make_unique<RT::CheckersPattern>();

  std::unique_ptr<RT::Shape> backWall = std::make_unique<RT::Plane>();
  backWall->transformation = RT::scaling(35, 1, 1) >>=
      RT::rotationX(M_PI / 2) >>= RT::rotationZ(M_PI / 2) >>=
      RT::translation(0, 0, 60);

  backWall->material.pattern = std::make_unique<RT::GradientPattern>(
      RT::color(135, 206, 250) / 256, RT::color(1, 1, 1));
  backWall->material.diffuse = 0.9;
  backWall->material.specular = 0.01;

  auto world = RT::World(false);
  world.light = RT::Light(RT::point(-10, 10, -10), RT::color(1, 1, 1));

  world.add(std::move(middle));
  world.add(std::move(bot));
  world.add(std::move(backWall));

  std::random_device rngDevice;
  std::default_random_engine rngEngine(rngDevice());
  std::uniform_real_distribution<double> translationZRng(-4, 40);
  std::normal_distribution<double> translationXRng(-10, 10);
  std::uniform_real_distribution<double> colorRng(0, 256);
  std::normal_distribution<double> ballSizeRng(0.1, 1);
  for (int i = 0; i < 40; ++i) {
    auto s = std::make_unique<RT::Sphere>();
    s->material.color = RT::color(colorRng(rngEngine), colorRng(rngEngine),
                                  colorRng(rngEngine)) /
                        256;
    auto ballSize = ballSizeRng(rngEngine);
    s->material.shininess = 300;
    s->material.specular = 1;
    s->material.diffuse = 0.3;
    s->material.reflective = 0;
    s->material.ambient = 0.3;
    s->transformation = RT::scaling(ballSize, ballSize, ballSize) >>=
        RT::translation(translationXRng(rngEngine), ballSize,
                        translationZRng(rngEngine));
    world.add(std::move(s));
  }

  auto camera = RT::Camera(2560, 1440, M_PI / 3);
  camera.transform = RT::viewTransform(RT::point(0, 1.5, -5),
                                       RT::point(0, 1, 0), RT::vector(0, 1, 0));

  auto canvas = camera.render(world);
  canvas.savePPM("sample.ppm");

  return 0;
}
