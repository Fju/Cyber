#include <VirtualWire.h>


#define PIN_RECEIVE A0  //RF Receiver pin = Analog pin 0

bool alarm = false;

void setup() {  
  Serial.begin(9600);
  
  vw_set_rx_pin(PIN_RECEIVE);
  vw_setup(2000);

  vw_rx_start();
}

void loop() {
  if (!alarm) {
    uint8_t buf[4];
    uint8_t buflen = 4;

    if (vw_get_message(buf, &buflen)) {
      int uid = buf[0] << 8 | buf[1];
      int bewegungsMelder = buf[2];
      if (bewegungsMelder == 1 << 6) {        
        Serial.println("AT+CMGF=1");
        delay(500);
        Serial.println("AT+CMGS=015778902929");
        delay(1000);
        Serial.print("Einbruchsalarm");
        delay(500);
        Serial.print(char(26));
        alarm = true;
      }
    }
  } else {
    delay(10000);
    alarm = false;  
  }
}

