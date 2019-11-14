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
  LINK::setup();
  Serial.begin(9600);
}

void loop() {
  
  if(!LINK::update()){
    if(Serial.available()){
      addString(Serial.readString());
      Serial.println("Transmitting");
      LINK::startTransmission();
    }
  }
  
}
