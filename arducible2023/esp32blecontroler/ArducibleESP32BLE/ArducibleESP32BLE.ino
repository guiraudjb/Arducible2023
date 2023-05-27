/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>
#define CIBLE1 25
#define CIBLE2 26
#define CIBLE3 27
bool valcible1="";
bool valcible2="";
bool valcible3="";
BleKeyboard bleKeyboard("Arducible");

void setup() {
  pinMode(CIBLE1, INPUT);
  pinMode(CIBLE2, INPUT);
  pinMode(CIBLE3, INPUT);

  Serial.begin(115200);
  Serial.println("#arduible Petanque Game");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    if(digitalRead(CIBLE1)){
    Serial.println("e");
    bleKeyboard.press('e');
    delay(100);
    bleKeyboard.release('e');
    delay(100);
    }
        if(digitalRead(CIBLE2)){
    Serial.println("r");
    bleKeyboard.press('r');
    delay(100);
    bleKeyboard.release('r');
    delay(100);
        }
            if(digitalRead(CIBLE3)){
    Serial.println("t");
    bleKeyboard.press('t');
    delay(100);
    bleKeyboard.release('t');
    delay(100);
      }
  }
//    valcible1=digitalRead(CIBLE1);
//  Serial.println(String(valcible1));
}
