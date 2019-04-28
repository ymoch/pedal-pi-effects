#ifndef YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_
#define YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_

#include <cstdint>

namespace ymoch::pedalpieffects::math::constexpr_math {

inline constexpr uint32_t power(uint32_t base, uint32_t exponent) {
  if (exponent == 0) {
    return 1;
  }
  return base * power(base, exponent - 1);
}

}  // ymoch::pedalpieffects::constexpr_math

#endif  // YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_
