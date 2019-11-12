#include "physical.h"

// max single transmission size
#define BIT_BUFFER_SIZE 1024

// stores bits to send
byte bit_buffer[BIT_BUFFER_SIZE];
// number of bits to send
int saved_bits = 0;

bool addBit(int new_bit) {
  /**
   * add a single bit to the buffer
   */
  if (saved_bits < BIT_BUFFER_SIZE){ 
    bit_buffer[saved_bits] = new_bit;
    saved_bits++;
  }
}

void addChar(char new_char) {
  /**
   * add character to the buffer
   */
  for(int i = 0; i < 8; i++) {
    addBit(new_char%2);
    new_char >>= 1;
  }
}

void addString(String new_string) {
  /**
   * add a string to the buffer
   */
  for (int i = 0; i < new_string.length(); i++) {
    addChar(new_string[i]);
  }
}

// tracks what bit in the buffer is being transmitted
int transmit_pos = 0;

void transmitBit(){
  /**
   * periodic transmission attempt on the bit buffer
   */
  if(PHY::update(bit_buffer[transmit_pos])){
    // attempt to update the current bit being transmitted
    // move onto next bit if successful
    transmit_pos++;
    if(transmit_pos == saved_bits){
      transmit_pos = 0;
      saved_bits = 0;
    }
  }
}

void setup() {
  PHY::setup();
}

void loop() {

  if(saved_bits > 0){
    transmitBit();
  }
  else{
    if(Serial.available()){
      addString(Serial.readString());
    }
  }
  
}
