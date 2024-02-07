#include "Pattern.hpp"
#include <memory>

namespace RT {

StripePattern::StripePattern()
    : Pattern(), a(RT::color(1, 1, 1)), b(RT::color(0, 0, 0)){};
StripePattern::StripePattern(const Color &a, const Color &b) : a(a), b(b){};
std::unique_ptr<Pattern> StripePattern::clone() const {
  return std::make_unique<StripePattern>(*this);
};

Color StripePattern::patternAt(const Point &p) const {
  if (static_cast<int>(std::floor(p.x)) % 2 == 0) {
    return a;
  }
  return b;
}

} // namespace RT