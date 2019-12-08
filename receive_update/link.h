/**
 * link.h
 * keeps track of received information in a buffer
 * in charge of terminating connection once flag byte is received
 */

#include "physical.h"

#define BUFFER_SIZE 1024
#define MESSAGE_FLAG 0xAA

namespace LINK {
  namespace {
    byte byte_buffer[BUFFER_SIZE];
    byte cur_byte = 0x00;
    int byte_pos;
    byte bit_pos;
    bool is_receiving;
  }

  void reset() {
    /**
     * reset link layer by resetting counters
     */
    is_receiving = false;
    byte_pos = 0;
    bit_pos = 0;
    PHY::reset();
  }

  void setup() {
    /**
     * just setup physical arduino stuff
     */
    PHY::setup();
    reset();
  }

  bool isReceiving() {
    /**
     * get if we are currently receiving a transmission
     * @return true if we are receiving something
     */
    return is_receiving;
  }

  int numBytes() {
    /**
     * get number of bytes stored in the buffer
     * @return number of bytes in the buffer
     */
    return byte_pos;
  }

  byte getByte(int pos) {
    /**
     * get the byte at a certain position in the buffer
     * @param  pos index of the byte to get
     * @return     the byte at index 'pos', or -1 if pos is out of range
     */
    if(pos > -1 && pos < byte_pos){
      return byte_buffer[pos];
    }
    else return -1;
  }

  bool update() {
    /**
     * update the link transmission status
     */
    int updated = PHY::update(cur_byte);
    if(updated == 1){
      // the physical layer performed an update
      is_receiving = true;
      if(bit_pos < 7){
        bit_pos++;
      }
      else{
        if(cur_byte == MESSAGE_FLAG){
          // end transmission if we found the terminal flag byte
          if(byte_pos > 0){
            is_receiving = false;
          }
        }
        else{
          // add the new byte to the buffer
          byte_buffer[byte_pos] = cur_byte;
          byte_pos++;
        }
        bit_pos = 0;
      }
    }
    else if(updated == -1){
      // an error occured
      is_receiving = false;
      Serial.println("transmission error");
    }
    return is_receiving;
  }
}
