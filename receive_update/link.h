#include "physical.h"

#define BUFFER_SIZE 1024
#define MESSAGE_FLAG 0xAA

namespace LINK {

  byte byte_buffer[BUFFER_SIZE];
  byte cur_byte;
  int byte_pos;
  byte bit_pos;
  bool is_receiving;

  void reset() {
    is_receiving = false;
    byte_pos = 0;
    bit_pos = 0;
    cur_byte = 0x00;
  }

  void setup() {
    // put your setup code here, to run once:
    PHY::setup();
    reset();
  }

  bool isReceiving() {
    return is_receiving;
  }

  int numBytes() {
    return byte_pos;
  }

  byte getByte(int pos){
    return byte_buffer[pos];
  }

  bool update() {
    if(PHY::update(cur_byte)){
      is_receiving = true;

      if(bit_pos < 7){
        bit_pos++;
      }
      else{
        if(cur_byte == MESSAGE_FLAG){
          if(byte_pos > 0){
            is_receiving = false;
          }
        }
        else{
          byte_buffer[byte_pos] = cur_byte;
          cur_byte = 0x00;
          bit_pos = 0;
          byte_pos++;
        }
      }
    }

    return is_receiving;

  }



}
