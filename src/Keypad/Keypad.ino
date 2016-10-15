#include <Key.h>
#include <Keypad.h>


const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'#', '0', '*', 'D'}
};
char password[4] = {'1', '5', '1', '0'};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 9, 8, 7, 6 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 5, 4, 3, 2 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledpin 10
/*
char *newpass(char *passwd) {

  char *newpasswd[4];
  for (int x = 0; x <= 4; x++) {
    *newpasswd[x] = kpd.getKey();
  }
  if (kpd.getKey() == '*') {
    //Vergleichen
    Serial.print(*newpasswd);
    return *newpasswd;
  } else {
    //Abbruch
    Serial.print(passwd);
    return *passwd;
  }
}
*/


void setup()
{
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600);
}
void loop()
{
  Serial.println("Passwort eingeben!");
  if (checkPasswd(typed(), password)) {
    Serial.println("Richtig!");
  } else {
    Serial.println("Falsch!");
  }
}

char *typed() {
  char typed_passwd[4];
  int x = 0;
  for (x; x < 4; x++) {
    typed_passwd[x] = key();
    Serial.println(typed_passwd[x]);
  }
  return typed_passwd;
}



boolean checkPasswd (char passwd[4] , char passwd_typed[4]) {
  for (int x = 1; x < 4; x++) {
    if (passwd_typed[x] != passwd[x]) {
      return false;
    }
  }
  return true;
}

char key() {
  char letter;
  letter = kpd.getKey();
  while (true) {
    if (letter) {
      return letter;
    } else {

      letter = kpd.getKey();
    }
  }
}
