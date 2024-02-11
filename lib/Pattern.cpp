#include "Pattern.hpp"
#include <memory>

namespace RT {

StripePattern::StripePattern() : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};
StripePattern::StripePattern(Color a, Color b)
    : a(std::move(a)), b(std::move(b)){};
std::unique_ptr<Pattern> StripePattern::clone() const {
  return std::make_unique<StripePattern>(*this);
};

Color StripePattern::patternAt(const Point &p) const {
  if (static_cast<int>(std::floor(p.x)) % 2 == 0) {
    return a;
  }
  return b;
}

TestPattern::TestPattern() = default;
;

Color TestPattern::patternAt(const Point &p) const {
  return RT::color(p.x, p.y, p.z);
}

std::unique_ptr<Pattern> TestPattern::clone() const {
  return std::make_unique<TestPattern>(*this);
}

GradientPattern::GradientPattern()
    : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};

GradientPattern::GradientPattern(Color a, Color b)
    : a(std::move(a)), b(std::move(b)){};

Color GradientPattern::patternAt(const Point &p) const {
  auto distance = b - a;
  auto fraction = p.x - std::floor(p.x);
  return a + distance * fraction;
}

std::unique_ptr<Pattern> GradientPattern::clone() const {
  return std::make_unique<GradientPattern>(*this);
}

RingPattern::RingPattern() : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};

RingPattern::RingPattern(Color a, Color b) : a(std::move(a)), b(std::move(b)){};

Color RingPattern::patternAt(const Point &p) const {
  if (int(std::floor(std::sqrt(p.x * p.x + p.z * p.z))) % 2 == 0) {
    return a;
  }
  return b;
}

std::unique_ptr<Pattern> RingPattern::clone() const {
  return std::make_unique<RingPattern>(*this);
}

CheckersPattern::CheckersPattern()
    : a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};

CheckersPattern::CheckersPattern(Color a, Color b)
    : a(std::move(a)), b(std::move(b)){};

Color CheckersPattern::patternAt(const Point &p) const {
  if ((int(std::floor(p.x)) + int(std::floor(p.y)) + int(std::floor(p.z))) %
          2 ==
      0) {
    return a;
  }
  return b;
}

std::unique_ptr<Pattern> CheckersPattern::clone() const {
  return std::make_unique<CheckersPattern>(*this);
}

} // namespace RT