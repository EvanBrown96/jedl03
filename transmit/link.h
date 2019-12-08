/**
 * link.h
 * keeps track of what data will be/has been transmitted
 * controls flags at start and end of message
 */

#include "physical.h"

#define BUFFER_SIZE 1024
#define MESSAGE_FLAG 0xAA

namespace LINK {
  namespace {
    bool is_transmitting;
    byte byte_buffer[BUFFER_SIZE];
    int saved_bytes;
    int byte_pos;
    byte bit_pos;

    byte getCurBit() {
      /**
       * get the current bit to be transmitted
       * @return the current bit to be transmitted
       */
      return (byte_buffer[byte_pos] >> (7 - bit_pos)) & 0x01;
    }

    void progressBit() {
      /**
       * move counters to the next bit to be transmitted
       */
      if(bit_pos < 7){
        bit_pos++;
      }
      else{
        bit_pos = 0;
        byte_pos++;
      }
    }

    void reset() {
      /**
       * reset link layer by clearing buffer and resetting counters
       */
      is_transmitting = false;
      byte_buffer[0] = MESSAGE_FLAG;
      saved_bytes = 1;
      byte_pos = 0;
      bit_pos = 0;
    }
  }

  bool addByte(byte new_byte) {
    /**
     * add a byte (i.e. character) to the buffer
     * @param  new_byte the byte to add
     * @return          true if the byte was successfully added to the buffer
     */
    if(saved_bytes < BUFFER_SIZE-1){
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
    reset();
  }

  bool isTransmitting() {
    /**
     * see if the link is transmitting or not
     * @return true if the link is transmitting
     */
    return is_transmitting;
  }

  void startTransmission() {
    /**
     * begin link transmission
     */
    is_transmitting = true;
    addByte(MESSAGE_FLAG);
  }

  bool update() {
    /**
     * update the link transmission status
     * @return true if the link is transmitting
     */
    if(isTransmitting()) {
      if(byte_pos == saved_bytes){
        // if end of transmission reached, set line to idle
        if(PHY::idle()) reset();
      }
      // update physical bit being transmitted
      else if(PHY::update(getCurBit())) {
        // if successfully updated, move to next bit
        progressBit();
      }
      return true;
    }
    return false;
  }
}
