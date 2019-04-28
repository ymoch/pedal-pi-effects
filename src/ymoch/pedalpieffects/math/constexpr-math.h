#ifndef YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_
#define YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_

#include <cstdint>

namespace ymoch::pedalpieffects::math::constexpr_math {

inline constexpr uint32_t power(uint32_t base, uint32_t exponent) {
  uint32_t out = 1;
  for (auto exp = exponent; exp > 0; --exp) {
    out *= base;
  }
  return out;
}

}  // ymoch::pedalpieffects::constexpr_math

#endif  // YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_
