// #include "link.h"
//
// void setup() {
//   // put your setup code here, to run once:
//   LINK::setup();
//   Serial.begin(9600);
// }
//
// void loop() {
//   if(!LINK::update()){
//     if(LINK::numBytes() > 0){
//       for(int i = 0; i < LINK::numBytes(); i++){
//         Serial.print(LINK::getByte(i));
//       }
//     }
//   }
//
// }

/////////////////////////////////////////////////////////////////////////////

#include "physical.h"

#define BUFFER_SIZE 1024
#define MESSAGE_FLAG 0xAA

byte byte_buffer[BUFFER_SIZE];
int byte_pos;
byte bit_pos;
bool is_receiving;

byte test = 0x00;

void setup() {
  // put your setup code here, to run once:
  PHY::setup();
  Serial.begin(9600);
}

void loop() {
  if(PHY::update(test)){
    Serial.println(test);
  }

  // put your main code here, to run repeatedly:

}
