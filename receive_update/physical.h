/**
 * physical.h
 * controls the arduino reading the physical GPIO pin
 * decodes data from manchester encoding
 */

#include "averager.h"
#include <math.h>

#define READ_PIN        A0
#define APPROX_BIT_RATE 400.0
#define PHASE_TIME      500000/APPROX_BIT_RATE
#define SAMPLE_SIZE     10
#define SAMPLE_TIME     (250000/APPROX_BIT_RATE)/SAMPLE_SIZE
#define LOW_THRESHOLD   266
#define HIGH_THRESHOLD  327

namespace PHY {
  namespace {
    Averager average(SAMPLE_SIZE);
    bool phase_set;
    bool front_half;
    unsigned long last_change;
    bool last_phase_high;
    unsigned long last_time;
    unsigned long cur_time;
  }

  void reset() {
    /**
     * resets physical layer
     */
    phase_set = false;
    front_half = true;
    last_time = 0;
    average.clear();
  }

  int update(byte& buffer) {
    /**
     * updates the buffer byte with the next bit transmitted across the line
     * @param  buffer the byte to update with a new bit (shifted left, with new LSB)
     * @return        1 if buffer updated, 0 if not, -1 if error occured (unexpected state)
     */
    cur_time = micros();
    if(cur_time - last_time >= SAMPLE_TIME) {
      // only do anything if we've waited long enough between samples
      last_time = cur_time;
      average.update(analogRead(READ_PIN));
      if(average.get() <= LOW_THRESHOLD && last_phase_high) {
        // falling edge detected
        last_phase_high = false;
        if(phase_set){
          // find out how many phases (half bit time) passed since last edge
          int phases_elapsed = round((cur_time - last_change)/static_cast<float>(PHASE_TIME));
          last_change = cur_time;
          if(phases_elapsed == 1){
            front_half = !front_half;
            if(!front_half){
              // falling edge at center of bit = 1
              buffer <<= 1;
              buffer |= 0x01;
              return 1;
            }
            // falling edge between bits = 0, but will be caught next cycle
          }
          else if(phases_elapsed == 2 && !front_half){
            // falling edge at center of bit = 1
            buffer <<= 1;
            buffer |= 0x01;
            return 1;
            // falling edge between bits after full bit up should not happen, so fall through to error
          }
          else if(phases_elapsed != 0){
            // ERROR
            return -1;
          }
        }
      }
      else if(average.get() >= HIGH_THRESHOLD && !last_phase_high) {
        // rising edge detected
        last_phase_high = true;
        if(!phase_set){
          // initialize receiving
          phase_set = true;
          last_change = cur_time;
        }
        else{
          // find out how many phases (half bit time) passed since last edge
          int phases_elapsed = round((cur_time - last_change)/static_cast<float>(PHASE_TIME));
          last_change = cur_time;
          if(phases_elapsed == 1){
            front_half = !front_half;
            if(!front_half){
              // rising edge at center of bit = 0
              buffer <<= 1;
              return 1;
            }
            // rising edge between bits = 1, but will be caught next cycle
          }
          else if(phases_elapsed == 2 && !front_half){
            // rising edge at center of bit = 1
            buffer <<= 1;
            return 1;
            // rising edge between bits after full bit low should not happen, so fall through to error
          }
          else if(phases_elapsed != 0){
            // ERROR
            return -1;
          }
        }
      }
    }
    // no update occurred, return 0
    return 0;
  }

  void setup() {
    /**
     * setup GPIO pin
     */
    pinMode(READ_PIN, INPUT);
    reset();
  }
}
