/**
 * transmit.ino
 * high-level control - communicates between computer and link layer
 */

#include "link.h"

int addString(String new_string) {
  /**
   * add a string to the buffer
   * @param  new_string the string to add
   * @return            the number of bytes that were not added (0 = success)
   */
  for(int i = 0; i < new_string.length(); i++) {
    if(!LINK::addByte(new_string[i])) return new_string.length() - i;
  }
  return 0;
}

void setup() {
  /**
   * called by arduino on startup - sets up layers and starts serial connection
   */
  LINK::setup();
  Serial.begin(9600);
}

void loop() {
  /**
   * periodically called by arduino
   * updates transmission status, or reads from serial connection
   */
  if(!LINK::update()){
    if(Serial.available()){
      String msg = Serial.readString();
      if(msg == "test"){
        while(!Serial.available()){
          SetPinFrequencySafe(PWM_PIN, LOW_FREQ);
          delayMicroseconds(1000000/600.0);
          SetPinFrequencySafe(PWM_PIN, HIGH_FREQ);
          delayMicroseconds(1000000/600.0);
        }
      }
      else{
        addString(msg);
        Serial.print("Transmitting: ");
        Serial.println(msg);
        LINK::startTransmission();
      }
    }
  }
}
