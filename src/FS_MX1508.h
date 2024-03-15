// Credit:  inspired by MX1508 AVR Library from Cheng Saetern https://github.com/Saeterncj/MX1508
#ifndef MX1508_h
#define MX1508_h

#include "Arduino.h"

typedef enum
{
  FAST_DECAY  = 0,
  SLOW_DECAY = 1,
  PWM_1PIN = 2,
} DecayMode;

class MX1508 {
  public:
#if defined(ARDUINO_ARCH_AVR)
#pragma message "WARNING: setResolution(...) and setFrequency(...) will be ignored on AVR architecture"
#endif
    // Default: SLOW_DECAY mode, PWM_2PIN , resolution 8 bits,  frequency 1000Hz
    MX1508(uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode = SLOW_DECAY);
    void motorGo(int32_t pwmVal);
    void motorGoP(int8_t pwmPercent);
    void motorStop();
    void motorBrake(uint8_t pwmPercent = 100);
    int32_t getPWM();
    void setResolution(uint32_t bitsResolution);  // will do nothing on AVR architecture
    void setFrequency(uint32_t pwmFrequency); // will do nothing on AVR architecture
  private:
    void analogWritePin(uint8_t pin, uint32_t val);
    int8_t _pinIN1;
    int8_t _pinIN2;
    int32_t _pwmVal;
    int32_t _pwmValIN1 = -1;
    int32_t _pwmValIN2 = -1;
    int   _frequency;
    int  _resolution;  // nbr bits
    int  _resolutionFactor;  // range
    DecayMode _whichMode;
};

#endif
