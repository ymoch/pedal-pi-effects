#ifndef YMOCH_PEDALPIEFFECTS_DSP_TYPE_H_
#define YMOCH_PEDALPIEFFECTS_DSP_TYPE_H_

#include <functional>

namespace ymoch::pedalpieffects::dsp::type {

typedef double Signal;
typedef std::function<Signal(Signal)> SignalFunction;

}  // ymoch::pedalpieffects::dsp::type

#endif  // YMOCH_PEDALPIEFFECTS_DSP_TYPE_H_
