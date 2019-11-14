#include "physical.h"

#define BUFFER_SIZE 1024

namespace LINK {

  namespace {

    byte byte_buffer[BUFFER_SIZE];
    int saved_bytes = 0;
    int byte_pos = 0;
    byte bit_pos = 0;

    byte getCurBit(){
      return byte_buffer[byte_pos] % 2;
    }

    void progressBit(){
      if(bit_pos < 7){
        bit_pos++;
        byte_buffer[byte_pos] >>= 1;
      }
      else{
        bit_pos = 0;
        byte_pos++;
      }
    }

  }

  bool addByte(byte new_byte) {
    /**
     * add a byte (i.e. character) to the buffer
     * @param  new_byte the byte to add
     * @return          true if the byte was successfully added to the buffer
     */
    if(saved_bytes < BUFFER_SIZE){
      byte_buffer[saved_bytes] = new_byte;
      saved_bytes++;
      return true;
    }

    return false;

  }

  void setup() {
    /**
     * just set up physical arduino stuff
     */
    PHY::setup();
  }


  bool isTransmitting() {
    return (saved_bytes > 0);
  }

  void update() {
    // if there is something to transmit...
    if(isTransmitting()) {
      // update physical bit being transmitted
      if(PHY::update(getCurBit())){
        //Serial.println(getCurBit());
        // if successfully updated, move to next bit
        progressBit();
        if(byte_pos == saved_bytes){
          byte_pos = 0;
          saved_bytes = 0;
        }
      }
    }
  }


}
