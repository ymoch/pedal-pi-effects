#include <cstdint>
#include <iostream>

#include <bcm2835.h>

using std::cout;
using std::cerr;
using std::endl;

namespace {
constexpr uint8_t kPwmChannel0 = 0;
constexpr uint8_t kPwmChannel1 = 1;
constexpr bool kPwmMarkSpaceEnabled = true;
constexpr bool kPwmChannelEnabled = true;

// Max clk frequency (19.2MHz/2 = 9.6MHz)
constexpr uint32_t kPwmClockDivider = BCM2835_PWM_CLOCK_DIVIDER_2;

constexpr uint8_t kPinPush1 = RPI_GPIO_P1_08;
constexpr uint8_t kPinPush2 = RPI_V2_GPIO_P1_38;
constexpr uint8_t kPinToggleSwitch1 = RPI_V2_GPIO_P1_32;
constexpr uint8_t kPinFootSwitch1 = RPI_GPIO_P1_10;
constexpr uint8_t kPinLed1 = RPI_V2_GPIO_P1_36;
}

int main(int argc, char **argv) {
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

  // Max clk frequency (19.2MHz/2 = 9.6MHz)
  bcm2835_pwm_set_clock(kPwmClockDivider);

  bcm2835_pwm_set_mode(kPwmChannel0, kPwmMarkSpaceEnabled, kPwmChannelEnabled);
  bcm2835_pwm_set_mode(kPwmChannel1, kPwmMarkSpaceEnabled, kPwmChannelEnabled);

  // 64 is max range (6bits): 9.6MHz/64=150KHz switching PWM freq.
  bcm2835_pwm_set_range(kPwmChannel0, 64);
  bcm2835_pwm_set_range(kPwmChannel1, 64);

  // define SPI bus configuration
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); // default
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); // default
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); // 4MHz clock
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0); // default
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW); // default

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
  double amplification = 1.0;
  double amplification_delta = 1.2;
  for (uint32_t read_timer = 0;; ++read_timer) {
    // read 12 bits ADC
    bcm2835_spi_transfernb(mosi, miso, 3);
    uint32_t input_signal = miso[2] + ((miso[1] & 0x0F) << 8);

    // Read the controls every 50000 times (0.25s) to save resources.
    if (read_timer % 50000 == 0) {
      read_timer = 0;
      uint8_t push_1 = bcm2835_gpio_lev(kPinPush1);
      uint8_t push_2 = bcm2835_gpio_lev(kPinPush2);
      uint8_t toggle_switch_1 = bcm2835_gpio_lev(kPinToggleSwitch1);
      uint8_t foot_switch_1 = bcm2835_gpio_lev(kPinFootSwitch1);

      // light the effect when foot switch 1 is activated.
      bcm2835_gpio_write(kPinLed1, !foot_switch_1);

      if (!push_1) {
        bcm2835_delay(100);  // 100ms delay for buttons debouncing.
        amplification /= amplification_delta;
        cout << "Amplification: " << amplification << endl;
      }
      if (!push_2) {
        bcm2835_delay(100);  // 100ms delay for buttons debouncing.
        amplification *= amplification_delta;
        cout << "Amplification: " << amplification << endl;
      }
    }

    //**** CLEAN EFFECT ***///
    // Nothing to do, the input_signal goes directly to the PWM output.
    double normalized_signal =
        (static_cast<double>(input_signal) - 2048.0) / 2048.0;
    double amplified_signal = normalized_signal * amplification;
    uint32_t output_signal =
        std::max(std::min(amplified_signal * 2048.0 + 2048.0, 4095.0), 0.0);

    // generate output PWM signal 6 bits
    bcm2835_pwm_set_data(1, output_signal & 0x3F);
    bcm2835_pwm_set_data(0, output_signal >> 6);
  }

  bcm2835_spi_end();
  bcm2835_close();
  return 0;
}
