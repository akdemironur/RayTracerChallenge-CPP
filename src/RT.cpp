#include "Matrix.hpp"
#include "Pattern.hpp"
#include "Tuple.hpp"
#include <RT.hpp>
#include <iostream>
#include <memory>
#include <random>

auto main() -> int {

  auto camera = RT::Camera(2000, 2000, 0.785398);
  camera.transform = RT::viewTransform(
      RT::point(-6, 6, -10), RT::point(6, 0, 6), RT::vector(-0.45, 1, 0));

  auto world = RT::World(false);
  world.lights.emplace_back(RT::point(50, 100, -50), RT::color(1, 1, 1));
  world.lights.emplace_back(RT::point(-400, 50, -10), RT::color(0.2, 0.2, 0.2));

  auto whiteMaterial = RT::Material();
  whiteMaterial.color = RT::color(1, 1, 1);
  whiteMaterial.ambient = 0.1;
  whiteMaterial.diffuse = 0.7;
  whiteMaterial.specular = 0.0;
  whiteMaterial.reflective = 0.0;

  auto redMaterial = RT::Material(whiteMaterial);
  redMaterial.color = RT::color(0.941, 0.322, 0.388);

  auto blueMaterial = RT::Material(whiteMaterial);
  blueMaterial.color = RT::color(0.537, 0.831, 0.914);

  auto purpleMaterial = RT::Material(whiteMaterial);
  purpleMaterial.color = RT::color(0.373, 0.404, 0.550);

  auto standardTransform = RT::translation(1, -1, 1) >>=
      RT::scaling(0.5, 0.5, 0.5);

  auto largeObject = standardTransform >>= RT::scaling(3.5, 3.5, 3.5);

  auto mediumObject = standardTransform >>= RT::scaling(3, 3, 3);

  auto smallObject = standardTransform >>= RT::scaling(2, 2, 2);

  auto backDrop = std::make_unique<RT::Plane>();
  backDrop->material.color = RT::color(1, 1, 1);
  backDrop->material.ambient = 1;
  backDrop->material.diffuse = 0;
  backDrop->material.specular = 0;
  backDrop->transformation = RT::rotationX(M_PI / 2) >>=
      RT::translation(0, 0, 500);
  world.add(std::move(backDrop));

  auto sphere1 = std::make_unique<RT::Sphere>();
  sphere1->material.color = RT::color(0.373, 0.404, 0.550);
  sphere1->material.diffuse = 0.2;
  sphere1->material.ambient = 0;
  sphere1->material.specular = 1;
  sphere1->material.shininess = 200;
  sphere1->material.reflective = 0.7;
  sphere1->material.transparency = 0.7;
  sphere1->material.refractiveIndex = 1.5;
  sphere1->transformation = largeObject;
  world.add(std::move(sphere1));

  auto cube1 = std::make_unique<RT::Cube>();
  cube1->material = whiteMaterial;
  cube1->transformation = mediumObject >>= RT::translation(4, 0, 0);
  world.add(std::move(cube1));

  auto cube2 = std::make_unique<RT::Cube>();
  cube2->material = blueMaterial;
  cube2->transformation = largeObject >>= RT::translation(8.5, 1.5, -0.5);
  world.add(std::move(cube2));

  auto cube3 = std::make_unique<RT::Cube>();
  cube3->material = redMaterial;
  cube3->transformation = largeObject >>= RT::translation(0, 0, 4);
  world.add(std::move(cube3));

  auto cube4 = std::make_unique<RT::Cube>();
  cube4->material = whiteMaterial;
  cube4->transformation = smallObject >>= RT::translation(4, 0, 4);
  world.add(std::move(cube4));

  auto cube5 = std::make_unique<RT::Cube>();
  cube5->material = purpleMaterial;
  cube5->transformation = mediumObject >>= RT::translation(7.5, 0.5, 4);
  world.add(std::move(cube5));

  auto cube6 = std::make_unique<RT::Cube>();
  cube6->material = whiteMaterial;
  cube6->transformation = mediumObject >>= RT::translation(-0.25, 0.25, 8);
  world.add(std::move(cube6));

  auto cube7 = std::make_unique<RT::Cube>();
  cube7->material = blueMaterial;
  cube7->transformation = largeObject >>= RT::translation(4, 1, 7.5);
  world.add(std::move(cube7));

  auto cube8 = std::make_unique<RT::Cube>();
  cube8->material = redMaterial;
  cube8->transformation = mediumObject >>= RT::translation(10, 2, 7.5);
  world.add(std::move(cube8));

  auto cube9 = std::make_unique<RT::Cube>();
  cube9->material = whiteMaterial;
  cube9->transformation = smallObject >>= RT::translation(8, 2, 12);
  world.add(std::move(cube9));

  auto cube10 = std::make_unique<RT::Cube>();
  cube10->material = whiteMaterial;
  cube10->transformation = smallObject >>= RT::translation(20, 1, 9);
  world.add(std::move(cube10));

  auto cube11 = std::make_unique<RT::Cube>();
  cube11->material = blueMaterial;
  cube11->transformation = largeObject >>= RT::translation(-0.5, -5, 0.25);
  world.add(std::move(cube11));

  auto cube12 = std::make_unique<RT::Cube>();
  cube12->material = redMaterial;
  cube12->transformation = largeObject >>= RT::translation(4, -4, 0);
  world.add(std::move(cube12));

  auto cube13 = std::make_unique<RT::Cube>();
  cube13->material = whiteMaterial;
  cube13->transformation = largeObject >>= RT::translation(8.5, -4, 0);
  world.add(std::move(cube13));

  auto cube14 = std::make_unique<RT::Cube>();
  cube14->material = whiteMaterial;
  cube14->transformation = largeObject >>= RT::translation(0, -4, 4);
  world.add(std::move(cube14));

  auto cube15 = std::make_unique<RT::Cube>();
  cube15->material = purpleMaterial;
  cube15->transformation = largeObject >>= RT::translation(-0.5, -4.5, 8);
  world.add(std::move(cube15));

  auto cube16 = std::make_unique<RT::Cube>();
  cube16->material = whiteMaterial;
  cube16->transformation = largeObject >>= RT::translation(0, -8, 4);
  world.add(std::move(cube16));

  auto cube17 = std::make_unique<RT::Cube>();
  cube17->material = whiteMaterial;
  cube17->transformation = largeObject >>= RT::translation(-0.5, -8.5, 8);
  world.add(std::move(cube17));

  auto canvas = camera.render(world);
  canvas.savePPM("sample.ppm");

  return 0;
}
