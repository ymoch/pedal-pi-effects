#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>

#include <bcm2835.h>

#include "dsp/effect/amplification.h"
#include "dsp/effect/biquad-filter.h"
#include "dsp/effect/tube-clipping.h"
#include "dsp/flow/toggle.h"
#include "dsp/normalization.h"
#include "dsp/type.h"
#include "math/constexpr-math.h"

using std::cout;
using std::cerr;
using std::endl;
using ymoch::pedalpieffects::dsp::type::Signal;
using ymoch::pedalpieffects::dsp::normalization::Normalizer;
using ymoch::pedalpieffects::dsp::effect::biquad_filter::BiquadFilter;
using ymoch::pedalpieffects::dsp::effect::biquad_filter::HighShelfFilter;
using ymoch::pedalpieffects::dsp::effect::amplification::Amplifier;
using ymoch::pedalpieffects::dsp::effect::tube_clipping::TubeClipper;
using ymoch::pedalpieffects::dsp::flow::toggle::Toggle;
using ymoch::pedalpieffects::math::constexpr_math::Power;

namespace {

constexpr uint8_t kPwmChannel0 = 0;
constexpr uint8_t kPwmChannel1 = 1;
constexpr uint32_t kNumBit = 6;
constexpr bool kPwmMarkSpaceEnabled = true;
constexpr bool kPwmChannelEnabled = true;
constexpr uint32_t kPwmClockDivider = BCM2835_PWM_CLOCK_DIVIDER_2;
constexpr uint32_t kPwmRange = Power<2, kNumBit>::value - 1;

constexpr uint32_t kSpiClockDivider = BCM2835_SPI_CLOCK_DIVIDER_64;

constexpr uint32_t kClockFrequencyHz = 169250;  // Tuned manually.

constexpr uint8_t kPinPush1 = RPI_GPIO_P1_08;
constexpr uint8_t kPinPush2 = RPI_V2_GPIO_P1_38;
constexpr uint8_t kPinToggleSwitch1 = RPI_V2_GPIO_P1_32;
constexpr uint8_t kPinFootSwitch1 = RPI_GPIO_P1_10;
constexpr uint8_t kPinLed1 = RPI_V2_GPIO_P1_36;
}

int main(int argc, char** argv) {
  if (!bcm2835_init()) {
    cerr << "bcm2835_init failed. Are you running as root??" << endl;
    return 1;
  }
  if (!bcm2835_spi_begin()) {
    cerr << "bcm2835_spi_begin failed. Are you running as root??" << endl;
    return 1;
  }

  // define PWM
  bcm2835_gpio_fsel(18, BCM2835_GPIO_FSEL_ALT5);  // PWM0 signal on GPIO18
  bcm2835_gpio_fsel(13, BCM2835_GPIO_FSEL_ALT0);  // PWM1 signal on GPIO13

  bcm2835_pwm_set_clock(kPwmClockDivider);

  bcm2835_pwm_set_mode(kPwmChannel0, kPwmMarkSpaceEnabled, kPwmChannelEnabled);
  bcm2835_pwm_set_mode(kPwmChannel1, kPwmMarkSpaceEnabled, kPwmChannelEnabled);

  bcm2835_pwm_set_range(kPwmChannel0, kPwmRange);
  bcm2835_pwm_set_range(kPwmChannel1, kPwmRange);

  // define SPI bus configuration
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);  // default
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);               // default
  bcm2835_spi_setClockDivider(kSpiClockDivider);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                  // default
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);  // default

  char mosi[10] = {0x01, 0x00, 0x00};  // 12 bit ADC read 0x08 ch0, - 0c for ch1
  char miso[10] = {0};

  // inputs configuration
  bcm2835_gpio_fsel(kPinPush1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(kPinPush2, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(kPinToggleSwitch1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(kPinFootSwitch1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(kPinLed1, BCM2835_GPIO_FSEL_OUTP);

  // pull-up configuration
  bcm2835_gpio_set_pud(kPinPush1, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(kPinPush2, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(kPinToggleSwitch1, BCM2835_GPIO_PUD_UP);
  bcm2835_gpio_set_pud(kPinFootSwitch1, BCM2835_GPIO_PUD_UP);

  // Main Loop
  const Normalizer<uint32_t> normalizer(0, Power<2, 12>::value - 1);
  auto input_equalize =
      Toggle<BiquadFilter>(HighShelfFilter(kClockFrequencyHz, 2000, 0.7, 3.0));
  auto gain = Amplifier(1.5);
  const auto tube_clip = TubeClipper();
  const auto master_volume = Amplifier(1.0 / 1.5);

  for (uint32_t read_timer = 0;; ++read_timer) {
    // read 12 bits ADC
    bcm2835_spi_transfernb(mosi, miso, 3);
    uint32_t input_signal = miso[2] + ((miso[1] & 0x0F) << 8);

    // Read the controls every 50000 times (0.25s) to save resources.
    constexpr uint32_t kControlCheckInterval = kClockFrequencyHz * 0.25;
    if (read_timer % kControlCheckInterval == 0) {
      constexpr double kGainFactorDelta = 1.5;

      uint8_t push_1 = bcm2835_gpio_lev(kPinPush1);
      if (!push_1) {
        bcm2835_delay(100);  // 100ms delay for buttons debouncing.
        gain.factor(gain.factor() / kGainFactorDelta);
        cout << "Gain: " << gain.factor() << endl;
      }

      uint8_t push_2 = bcm2835_gpio_lev(kPinPush2);
      if (!push_2) {
        bcm2835_delay(100);  // 100ms delay for buttons debouncing.
        gain.factor(gain.factor() * kGainFactorDelta);
        cout << "Gain: " << gain.factor() << endl;
      }

      uint8_t toggle_switch_1 = bcm2835_gpio_lev(kPinToggleSwitch1);
      input_equalize.enabled(!toggle_switch_1);

      // light the effect when foot switch 1 is activated.
      uint8_t foot_switch_1 = bcm2835_gpio_lev(kPinFootSwitch1);
      bcm2835_gpio_write(kPinLed1, !foot_switch_1);
    }

    Signal signal = normalizer.Normalize(input_signal);
    signal = input_equalize(signal);
    signal = gain(signal);
    signal = tube_clip(signal);
    signal = master_volume(signal);

    // generate output PWM signal 6 bits
    uint32_t output_signal = normalizer.Unnormalize(signal);
    bcm2835_pwm_set_data(1, output_signal & 0x3F);
    bcm2835_pwm_set_data(0, output_signal >> 6);
  }

  bcm2835_spi_end();
  bcm2835_close();
  return 0;
}
