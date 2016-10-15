#include <VirtualWire.h>


#define PIN_RECEIVE A1  //RF Receiver pin = Analog pin 0


byte message[VW_MAX_MESSAGE_LEN];    // a buffer to hold the incoming messages
byte msgLength = VW_MAX_MESSAGE_LEN;

void setup() {
  Serial.begin(9600);
  vw_set_rx_pin(PIN_RECEIVE);
  vw_setup(2000);

  vw_rx_start();
}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {
    int i;

    // Message with a good checksum received, print it.
    Serial.print("Got: ");
    for (i = 0; i < buflen; i++) {
      Serial.print((char)buf[i]);
      Serial.print(' ');
    }
    Serial.println();
    
  }
}

