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

//int getBinaryIntFromInt(int int_source, int position)
//{
//  int mask = pow(2, position);
//  int result = int_source & mask;
//  if (result == mask) return 1;
//  else return 0;
//}

void addChar(char new_char) {
  for(int i = 0; i < 8; i++) {
    addBit(new_char%2);
    new_char >>= 1;
  }
//  addBit(getBinaryIntFromInt(new_char, 0));
//  addBit(getBinaryIntFromInt(new_char, 1));
//  addBit(getBinaryIntFromInt(new_char, 2));
//  addBit(getBinaryIntFromInt(new_char, 3));
//  addBit(getBinaryIntFromInt(new_char, 4));
//  addBit(getBinaryIntFromInt(new_char, 5));
//  addBit(getBinaryIntFromInt(new_char, 6));
//  addBit(getBinaryIntFromInt(new_char, 7));
}

void addString(String new_string) {
  for (int i = 0; i < new_string.length(); i++) {
    addChar(new_string[i]);
  }
}

//void transmitBit(int bit_to_transmit)
//{
//  //Transmit a bit, in whatever way you want.
//  
//}
//
//void transmitBits()
//{
//  bit_position = 0; //Make sure that we start from the beggining.
//  while (bit_position != max_bit_position)
//  {
//    transmitBit(bit_buffer[bit_position]);
//    bit_position++;
//  }
//  max_bit_position = 0; //When we are done, we set the max back to zero, thereby clearing the buffer.
//}

int transmit_pos = 0;

void transmitBit(){
  if(PHY::update(bit_buffer[transmit_pos])){
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
