#ifndef YMOCH_PEDALPIEFFECTS_DSP_PROCESSOR_H_
#define YMOCH_PEDALPIEFFECTS_DSP_PROCESSOR_H_

#include "type.h"

namespace ymoch::pedalpieffects::dsp::processor {

class Processor {
  virtual ~Processor();
  virtual type::Signal process(type::Signal signal) = 0;
};

}  // ymoch::pedalpieffects::dsp::processor

#endif  // YMOCH_PEDALPIEFFECTS_DSP_PROCESSOR_H_
