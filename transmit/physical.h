/**
 * physical.h
 * controls physical output of the arduino PWM
 * responsible for data encoding (manchester)
 * acts as a software-implemented VCO
 */

#include <PWM.h>

// pin on which to output the PWM signal
#define PWM_PIN    3
// output bit rate
#define BIT_RATE   400.0
// output to use for logical '0'
#define LOW_FREQ   47000
// output to use for logical '1'
#define HIGH_FREQ  53000
// half the time a single bit is transmitted for (us)
#define PHASE_TIME 500000/BIT_RATE

namespace PHY {
  namespace {
    // if transmitted is in the half of bit time to transmit in
    bool front_half = false;
    // time containers
    unsigned long cur_time;
    unsigned long last_time = 0;

    void setHigh() {
      /**
       * sets PWM output to the high frequency
       */
      SetPinFrequencySafe(PWM_PIN, HIGH_FREQ);
    }

    void setLow() {
      /**
       * sets PWM output to the low frequency
       */
      SetPinFrequencySafe(PWM_PIN, LOW_FREQ);
    }
  }

  void setup() {
    /**
     * sets up arduino timers and GPIO pins
     */
    InitTimersSafe();
    pinMode(PWM_PIN, OUTPUT);
    setLow();
    pwmWrite(PWM_PIN, 128);
  }

  bool idle() {
    /**
     * update the transmission line to be idle (only low output)
     * @return true if the transmission line successfully set to idle
     */
    cur_time = micros();
    if(cur_time - last_time >= PHASE_TIME){
      last_time = cur_time;
      setLow();
      return true;
    }
    return false;
  }

  bool update(byte next_bit) {
    /**
     * updates the physical state of the arduino
     * @param  next_bit the desired next bit to transmit
     * @return          true iff the physical output was updated to next_bit
     */
    cur_time = micros();
    if(cur_time - last_time >= PHASE_TIME){
      last_time = cur_time;
      if(front_half){
        front_half = false;
        if(next_bit) setLow();
        else         setHigh();
        return true;
      }
      else{
        front_half = true;
        if(next_bit) setHigh();
        else         setLow();
      }
    }
    return false;
  }
}
