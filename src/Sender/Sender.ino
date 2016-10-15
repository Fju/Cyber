#include <VirtualWire.h>

#define PIN_BEWEGUNGSMELDER 6
#define PIN_SENDER 5

int uid = 31875; //TODO: keine feste UID

void setup() {
  Serial.begin(9600);
  
  //Setup der Sensoren
  pinMode(PIN_BEWEGUNGSMELDER, INPUT);

  //Setup für VirtualWire und kabellose Kommunikation
  vw_set_tx_pin(PIN_SENDER);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
}


void loop() {
    
  //Message Protokoll mit vorzeichenlosen 8-bit Ganzzahlen
  uint8_t msg[4] = {};
  //Modul UID von 16-bit in zwei 8-bit Integer umwandeln
  msg[0] = uid >> 8;
  msg[1] = uid & 0xFF;

  int bewegungsMelder = digitalRead(PIN_BEWEGUNGSMELDER);

  msg[2] = bewegungsMelder << 6; //Index: 0

  Serial.println(msg[2]);
  

  //Nachricht senden
  vw_send(msg, sizeof(msg));
  vw_wait_tx();

  //Warten
  delay(100);
}
