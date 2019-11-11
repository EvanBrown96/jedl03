#include "physical.h"

//Pin
const int OUT_PIN = A0;

//Values
const int MAX_BIT_BUFFER_SIZE = 256;

//State
int bit_buffer[MAX_BIT_BUFFER_SIZE] = {0};
int max_bit_position = 0;
int bit_position = 0;

void addBit(int new_bit)
{
  //Add a bit to the bit buffer.
  bit_buffer[bit_position] = new_bit;
  if (max_bit_position != MAX_BIT_BUFFER_SIZE) max_bit_position++;
}

int getBinaryIntFromInt(int int_source, int position)
{
  int mask = pow(2, position);
  int result = int_source & mask;
  if (result == mask) return 1;
  else return 0;
}

void addChar(char new_char)
{
  addBit(getBinaryIntFromInt(new_char, 0));
  addBit(getBinaryIntFromInt(new_char, 1));
  addBit(getBinaryIntFromInt(new_char, 2));
  addBit(getBinaryIntFromInt(new_char, 3));
  addBit(getBinaryIntFromInt(new_char, 4));
  addBit(getBinaryIntFromInt(new_char, 5));
  addBit(getBinaryIntFromInt(new_char, 6));
  addBit(getBinaryIntFromInt(new_char, 7));
}

void addString(String new_string)
{
  for (int i = 0; i < new_string.length(); i++)
  {
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

void transmitBit(){
  if(PHY::update(bit_buffer[bit_position])) bit_position++;
  if(bit_position == max_bit_position){
    bit_position = 0;
    max_bit_position = 0;
  }
}

void setup() {
  PHY::setup();
}

void loop() {

  if(max_bit_position > 0){
    transmitBit();
  }
  else{
    if(Serial.available()){
      addString(Serial.readString());
    }
  }
  
}
