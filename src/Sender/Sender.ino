#include <VirtualWire.h>

#define PIN_BEWEGUNGSMELDER 6
#define PIN_SENDER 5

//Pick a random UID (between 0 and 65536) to make your module unique
//Keep in mind that you can determine in which room the alarm was triggered (see Empfaenger.ino)
uint16_t uid = 31875;

//Setting up Serial communication, Pins and VirtualWire communication
void setup() {
  Serial.begin(9600);
  pinMode(PIN_BEWEGUNGSMELDER, INPUT);
  vw_set_tx_pin(PIN_SENDER);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
}

void loop() {
  //Read the current data
  int bewegungsMelder = digitalRead(PIN_BEWEGUNGSMELDER); 
  
  //`msg` should contain the UID and sensor data, can be extended with plenty more sensor data.
  uint8_t msg[4] = {};
  
  msg[0] = uid >> 8;    // * Split the unsigned 16-bit UID Integer into two unsigned 8-bit Integers
  msg[1] = uid & 0xFF;  // * using bitwise operators and bit shifting

  msg[2] = bewegungsMelder << 6;  // [ 0 0 ] | [ X X X X X X ]
                                  // INDEX   | VALUE
                                  //Every message byte consists of two bits that indicate the unique index of the sensor and
                                  //six bits for value data (in this case either 111111 or 000000 is sent)
  
  Serial.println(msg[2]); //Print the message for developing purpose
  

  //Send the message using the VirtualWire library and wait until it's sent
  vw_send(msg, sizeof(msg));
  vw_wait_tx();

  //Wait some milliseconds to relax :)
  delay(100);
}
