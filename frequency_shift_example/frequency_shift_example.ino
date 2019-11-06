#include <PWM.h>

#define PWM_PIN 3
#define BIT_RATE 3000
#define LOW_FREQ 40000  
#define HIGH_FREQ 60000

void setup() {
  // put your setup code here, to run once:
  InitTimersSafe();
  pinMode(PWM_PIN, OUTPUT);
  SetPinFrequencySafe(PWM_PIN, LOW_FREQ);
  pwmWrite(PWM_PIN, 128);
}

bool isHigh = false;
int counter = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(micros() - counter > (1000000/BIT_RATE)){
    counter = micros();
    isHigh = !isHigh;
    if(isHigh) SetPinFrequencySafe(PWM_PIN, HIGH_FREQ);
    else SetPinFrequencySafe(PWM_PIN, LOW_FREQ);
  }
}
