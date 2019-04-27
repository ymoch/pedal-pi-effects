#ifndef YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_
#define YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_

#include <cstdint>

namespace ymoch::pedalpieffects::math::constexpr_math {

template <uint32_t base, uint32_t exponent>
struct Power {
  constexpr static uint32_t value = base * Power<base, exponent - 1>::value;
};
template <uint32_t base>
struct Power<base, 0> {
  constexpr static uint32_t value = 1;
};

}  // ymoch::pedalpieffects::constexpr_math

#endif  // YMOCH_PEDALPIEFFECTS_MATH_CONSTEXPR_MATH_H_
