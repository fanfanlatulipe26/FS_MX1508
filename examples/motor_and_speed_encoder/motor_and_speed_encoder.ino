/*
  Study of a motor, with a simple speed encode (slotted disk and IR optical sensor)
  The results can be plotted to show the results of the different driving mode:
   SLOW_DECAY, FAST_DECAY, PWM_1PIN
   In forward mode: Ramp up to speed up to 200 by increasing  by 1 every 100 millisecond.
   Then ramp down from 200 to 0, by decreasing by 1 every 100 millisecond.
   Then in backward mode run the same ramp up/ramp down
   Every 100 millisecond, print the number of ticks seen by the speed encoder
   during the last 100 milliseconds.(proportional to the "speed" of the motor)

  The results can be plotted with the Serial Plotter, or copy/paste in an Excel sheet
  for further plot..
   
*/
//

#include <FS_MX1508.h>

// ESP32:
const uint8_t pinA = 18; // IN1 motor A
const uint8_t pinB = 19;  // IN2 motor A
const uint8_t pinIT = 15; // IT speed sensor ESP32

// UNO:
//const uint8_t pinA = 10; // IN1 motor A  pwm
//const uint8_t pinB = 11;  // IN2 motor A pwm for FAST_DECAY or SLOW_DECAY
//const uint8_t pinB = 7;  // IN2 motor A . Simple non pwm pin for PWM_1PIN mode
//const byte pin = 2;  // IT speed sensor UNO

volatile unsigned int counter = 0;
unsigned int lastCounter;


MX1508 motorA(pinA, pinB, SLOW_DECAY);
//MX1508 motorA(pinA, pinB, FAST_DECAY);
//MX1508 motorA(pinA, pinB, PWM_1PIN);


// Interrupt Service Routines

#if defined(ESP32)
void IRAM_ATTR ISR_count()
#elif defined(ESP8266)
void  ICACHE_RAM_ATTR ISR_count()
#else
void  ISR_count()  // AVR RP2040
#endif
{
  counter++;  // increment counter value
}

void setup() {
  Serial.begin(115200);
//  delay(10000);
  Serial.println(); Serial.println();
  Serial.println("PWM\tSpeed");
  pinMode(pinIT, INPUT);
  attachInterrupt(digitalPinToInterrupt (pinIT), ISR_count, FALLING);

  motorA.motorStop();
  motorA.motorBrake();

}

void loop() {
  static unsigned long lastMilli = 0;
  static bool cwDirection = true;
  static int pwm = 0;
  static char cw = ' ';
  motorA.motorGo(pwm);
  if (millis() - lastMilli > 100) {  //
    if (cwDirection && pwm++ >= 200) {  // 255
      cwDirection = false;
      pwm--;
    } else if (!cwDirection && pwm-- <= -200) {
      cwDirection = true;
      pwm++;
    }
    if (pwm == 0) {
      if (cw == '-')
        cw = ' ';
      else
        cw = '-';
      //  motorA.motorBrake();
      motorA.motorStop();
      delay(1000);
      motorA.motorGo(0);
    }
    // Serial.printf("pwm: %d\t", pwm);
    lastMilli = millis();
    noInterrupts();
    lastCounter = counter;
    counter = 0;
    interrupts();
    Serial.print(motorA.getPWM());
    Serial.print("\t"); Serial.print(char(cw));
    Serial.println(lastCounter);
    motorA.motorGo(pwm);
  }
}
