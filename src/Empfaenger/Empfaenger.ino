#include <StaticThreadController.h>
#include <ThreadController.h>
#include <Thread.h>

#include <VirtualWire.h>


#define PIN_RECEIVE A0
#define PIN_ALARM 5

bool alarm = false;

Thread analog1 = Thread();
ThreadController controller = ThreadController();




String raumErkennung(int uid) {
  switch (uid) {
    case 14248:
      return "Raum 1";
    case 31875:
      return "Raum 2";
    default:
      return "Gebäude";  
  }  
}

void setup() {  
  Serial.begin(9600);
  pinMode(PIN_ALARM, OUTPUT);
  
  vw_set_rx_pin(PIN_RECEIVE);
  vw_setup(2000);

  analog1.onRun(boringCallback);
  analog1.setInterval(50);

  controller.add(&analog1);
  vw_rx_start();
}

void loop() {
  controller.run();  
}



void boringCallback() {
  if (!alarm) {
    uint8_t buf[4];
    uint8_t buflen = 4;
    
    if (vw_get_message(buf, &buflen)) {
      int uid = buf[0] << 8 | buf[1];
      int bewegungsMelder = buf[2];
      if (bewegungsMelder > 0) {
        tone(PIN_ALARM, 440);
        Serial.println("AT+CMGF=1");
        delay(500);
        Serial.println("AT+CMGS=015778902929");
        delay(1000);
        Serial.print("Bewegungsalarm im ");
        Serial.print(raumErkennung(uid));
        Serial.print(" wurde ausgeloest");
        delay(500);
        Serial.print(char(26));
        alarm = true;
      }      
    }    
  } else {
    delay(500);
    noTone(PIN_ALARM);
    alarm = false;  
  }
}

