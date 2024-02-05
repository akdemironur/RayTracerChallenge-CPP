#include <Tuple.hpp>
#include <iostream>

int main() {
  RT::Color c = RT::color(4, 5, 6);
  auto p = RT::color(1, 2, 3);
  std::cout << c.red << std::endl;
  c = p;
  std::cout << c.red << std::endl;
}
