// Simple example for FS_MX1508 library
//  - simplest contructor.
//  - demo for spinning CW, CCW, stopping (freewheeling) and braking
//  - better to use a separate powersupply for the MX1508 board (10v max)
//      (don't forget the ground with the processor board ...)

#include <FS_MX1508.h>

#define pinA 18 // to be adapted to your configuration. He re ESP32 ...
#define pinB 19 // to be adapted to your configuration. Here ESP32 ...

MX1508 motorA(pinA, pinB); // default SLOW_DECAY (resolution 8 bits, frequency 1000Hz)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  Serial.println("Ramp up forward 0 to  200");
  for (int pwm = 0; pwm <= 200; pwm++) { // ramp up forward.
    motorA.motorGo(pwm);
    delay(50);
  }

  Serial.println("Motor stop: speed decrease slowly (free wheeling)");
  motorA.motorStop();  // free wheeling. Motor stops slowly
  delay(5000);

  Serial.println("Ramp up backward 0 to  -200");
  for (int pwm = 0; pwm <= 200; pwm++) { // ramp up backward.
    motorA.motorGo(-pwm);
    delay(50);
  }

  Serial.println("Motor brake: speed decrease quickly ");
  motorA.motorBrake();  //  Fast , strong brake
  delay(5000);

}
