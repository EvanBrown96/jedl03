//PINS
const int RECEIVER_PIN = A0; //Pin we recieve input on. Input is assumed to be in 1s and 0s

//SETUP
const int MAX_BIT_BUFFER_SIZE = 256; //Number of bits that can be stored before flush.

//CALIBRATION
const int BIT_TIME = 30; //Time, in ms, to wait before treating the input as a distinct bit.

//STATE
int bit_buffer[MAX_BIT_BUFFER_SIZE] = {0}; //Stores recieved bits
int bit_position = 0 //Which bit we are reading in.

void recieveBit()
{
  //Retreives the transmitted bit, and places it in the bit_buffer. Assume we are recivieing a digital signal.
  int cutoff = 1023/2;
  int read_value = analogRead(RECEIVER_PIN);
  int state;

  //Change if need be :)
  if (read_value < cutoff) state = 1;
  else state = 0;
  
  bit_buffer[bit_position] = state;
  bit_position++;
}

void recieveBits()
{
  //Reads MAX_BIT_BUFFER_SIZE bits into bit_buffer
  bit_position = 0;
  for (int i = 0; i < MAX_BIT_BUFFER_SIZE; i++)
  {
    recieveBit();
    sleep(BIT_TIME);
  }
}

char getChar(int b0, int b1, int b2, int b3, int b4, int b5, int b6, int b7, int b8)
{
  //Returns a character
  int to_return;
  to_return+=(2**7)*b0;
  to_return+=(2**6)*b1;
  to_return+=(2**5)*b2;
  to_return+=(2**4)*b3;
  to_return+=(2**3)*b4;
  to_return+=(2**2)*b5;
  to_return+=(2**1)*b6;
  to_return+=(2**0)*b7;
  return to_return;
}

String translateString()
{
  int current_char_start = 0;
  String to_return = "";
  while (1)
  {
    int b0 = current_char_start+0;
    int b1 = current_char_start+1;
    int b2 = current_char_start+2;
    int b3 = current_char_start+3;
    int b4 = current_char_start+4;
    int b5 = current_char_start+5;
    int b6 = current_char_start+6;
    int b7 = current_char_start+7;
    char next_char = getChar(b0, b1, b2, b3, b4, b5, b6, b7);
    to_return.concat(next_char);
    
  }
  return to_return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RECEIVER_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  recieveBits();
  Serial.println(translateString());
}
