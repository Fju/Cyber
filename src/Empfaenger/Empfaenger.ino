#include <Keypad.h>
#include <VirtualWire.h>


#define PIN_RECEIVE A0
#define PIN_ALARM 2

//States of the alarm
enum Alarm {
  ENABLED, DISABLED, ON  
};

//Alarm variable that indicates the current state, by start the alarm is enabled
Alarm alarm = ENABLED;

//Defining parameters for initializing the Keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = { //Keytable
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'#', '0', '*', 'D'}
};
//Pins that are connected with the Keypad to locate the pressed key
byte rowPins[ROWS] = { 10, 9, 8, 7 };
byte colPins[COLS] = { 6, 5, 4, 3 };

//Initializing the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
char password[4] = {'1', '5', '1', '0'}; //default password

//Converts UID of the module to a specific name,
//can be used to locate the room where the alarm was triggered
String getNameByUID(int uid) {
  switch (uid) {
    case 14248: //randomly picked UID for the first...
      return "Raum 1";
    case 31875: //... and second module
      return "Raum 2";
    default:
      return "Gebäude";  
  }  
}

//Setting up Serial communication, Pins and VirtualWire communication
void setup() {  
  Serial.begin(9600);
  pinMode(PIN_ALARM, OUTPUT);
  
  vw_set_rx_pin(PIN_RECEIVE);
  vw_setup(2000);

  vw_rx_start();
}


void loop() {  
  if (alarm == ENABLED) {
    //Create buffer for incoming messages
    uint8_t buf[4];
    uint8_t buflen = 4;
    
    if (vw_get_message(buf, &buflen)) {
      int uid = buf[0] << 8 | buf[1]; //First two bytes represent a 16-bit UID
      int bewegungsMelder = buf[2]; //Third byte contains data of the movement detector
      if (bewegungsMelder > 0) {
        //Send SMS using the Serial communication @ 9600 baud rate in case movement has been detected
        Serial.println("AT+CMGF=1");
        delay(500);
        Serial.println("AT+CMGS=015778902929"); //Enter your desired phone number here
        delay(1000);
        Serial.print("Bewegungsalarm im ");   // *
        Serial.print(getNameByUID(uid));     // * Content of the message
        Serial.print(" wurde ausgeloest");    // *
        delay(500);
        Serial.print(char(26)); //char(26) returns a special character (EOT) that ends the transmission
        //Turn on the alarm. It also prevents listening for more incoming messages from the module,
        //which is not necessary anymore.
        alarm = ON;
      }
    }
  } else if (alarm == ON) {
    //Turn on the alarm sound by a Pin
    digitalWrite(PIN_ALARM, HIGH);
    //Wait until the password is valid to disable the alarm
    waitForPassword();
    digitalWrite(PIN_ALARM, LOW);
    Serial.println("AUS");
    alarm = DISABLED;
  } 
}


char key() {
  //Return the currently pressed letter (if nothing is pressed, wait until it is)
  char letter = kpd.getKey();
  while (true) {
    if (letter) return letter;
    else letter = kpd.getKey();    
  }
}

boolean checkPasswd (char passwd[4] , char passwd_typed[4]) {
  //Checking password, password is valid if every four characters match with the machine's password
  for (int x = 0; x < 4; x++) {
    if (passwd_typed[x] != passwd[x]) return false;
  }
  return true;
}

void waitForPassword() {
  while (true) {
    //Iterate forever (or the loop is destroyed by `return`)
    char input[4];
    int i = 0;
    for (; i < 4; i++) {
      //Waiting until four characters were typed
      char currentInput = key();
      if (currentInput == '#' || currentInput == '*') {
        //These characters reset the input in order to correct typo's
        break;
      } else {
        //Set the current input to the char-Array and print the input for developing purpose
        input[i] = currentInput;
        Serial.println(currentInput);
      }
    }
    //If the user typed in each four characters and the password is valid, exit the while-Loop
    if (i == 4 &&  checkPasswd(password, input)) return;
  }
}

