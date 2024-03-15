
// Credit:  inspired by MX1508 AVR Library from Cheng Saetern https://github.com/Saeterncj/MX1508
#include "FS_MX1508.h"
MX1508::MX1508(uint8_t pinIN1, uint8_t pinIN2, DecayMode decayMode) {
  _pinIN1 = pinIN1;
  _pinIN2 = pinIN2;
  _whichMode = decayMode;
  _frequency = 1000;
  _resolution = 8;
  _resolutionFactor = (1 << _resolution) - 1;
}

int32_t MX1508::getPWM() {
  return _pwmVal;
}

void MX1508::motorStop() {
  motorBrake(0);
}

void MX1508::motorBrake(uint8_t pwmPercent) {
  int brake = map(pwmPercent, 0, 100, 0, _resolutionFactor);
  _pwmVal = brake;
  analogWritePin( _pinIN1, brake);
  analogWritePin( _pinIN2, brake);
}

void MX1508::setResolution(uint32_t bitsResolution) {
#if !defined(ARDUINO_ARCH_AVR)
  analogWriteResolution(bitsResolution);
#if defined(ESP32)
  analogWriteFrequency(_frequency);  // ESP32 is different. Why ?????????
#else
  analogWriteFreq(_frequency);  // OK pour ESP8266 et pour RP2040, AVR ...
#endif
  _resolution = bitsResolution;
  _resolutionFactor = (1 << _resolution) - 1;
#endif
}

void  MX1508::setFrequency(uint32_t pwmFrequency) {
#if !defined(ARDUINO_ARCH_AVR)
  analogWriteResolution(_resolution);
#if defined(ESP32)
  analogWriteFrequency(_frequency);  // ESP32 is different. Why ?????????
#else
  analogWriteFreq(_frequency);  // OK pour ESP8266 et pour RP2040, AVR ...
#endif
  _frequency = pwmFrequency;
#endif
}

void MX1508::analogWritePin(uint8_t pin, uint32_t val)
// Do nothing if the pin already have the right pwm value (will avoid some glitches / overhead)
{
  if ( pin == _pinIN1) {
    if (_pwmValIN1 == val) return;
    analogWrite(pin, val);
    _pwmValIN1 = val;
  }
  else {
    if (_pwmValIN2 == val) return;
    analogWrite(pin, val);
    _pwmValIN2 = val;
  }
}

void MX1508::motorGoP(int8_t pwmPercent) {
  motorGo(map(pwmPercent, -100, 100, - _resolutionFactor, _resolutionFactor));
}

void MX1508::motorGo(int32_t pwmSpeed) {
  _pwmVal = pwmSpeed;
  switch (_whichMode) {
    case FAST_DECAY:   // one pin LOW, other PWM
      if (pwmSpeed >= 0) {
        analogWritePin(_pinIN2, 0);
        analogWritePin(_pinIN1, pwmSpeed);
      }
      else {
        analogWritePin(_pinIN1, 0);
        analogWritePin(_pinIN2, -pwmSpeed);
      }
      break;
    case SLOW_DECAY: // one pin HIGH, other complement of PWM
      if (pwmSpeed >= 0) {
        pwmSpeed = map(pwmSpeed, 0, _resolutionFactor, _resolutionFactor, 0);
        analogWritePin(_pinIN1, _resolutionFactor );
        analogWritePin(_pinIN2, pwmSpeed);
      }
      else {
        pwmSpeed = map(-pwmSpeed, 0, _resolutionFactor, _resolutionFactor, 0);
        analogWritePin(_pinIN1, pwmSpeed);
        analogWritePin(_pinIN2, _resolutionFactor);
      }
      break;
    case PWM_1PIN:  // only pinIN1 is PWM allowed.
      if (pwmSpeed >= 0) { // forward: driven as in FAST_DECAY
        analogWritePin(_pinIN1, pwmSpeed);
        analogWritePin(_pinIN2, 0);
      }
      else {  // backward: driven as in SLOW_DECAY
        pwmSpeed = map(-pwmSpeed, 0, _resolutionFactor, _resolutionFactor, 0);
        analogWritePin(_pinIN2, _resolutionFactor );
        analogWritePin(_pinIN1, pwmSpeed);
      }
      break;
  }
}
