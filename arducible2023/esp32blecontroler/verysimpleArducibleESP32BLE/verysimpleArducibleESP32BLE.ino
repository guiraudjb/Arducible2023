/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("ArduciblePetanqueGame");
const int Cible1 = 2;
const int Cible2 = 4;
const int Cible3 = 5;

void setup() {
  bleKeyboard.begin();
  pinMode(Cible1, INPUT);
  pinMode(Cible2, INPUT);
  pinMode(Cible3, INPUT);

}

void loop() {
  if(bleKeyboard.isConnected()) {
    if(digitalRead(Cible1)){
    bleKeyboard.press('e');
    delay(100);
    bleKeyboard.release('e');
    delay(100);
    }
        if(digitalRead(Cible2)){
    bleKeyboard.press('r');
    delay(100);
    bleKeyboard.release('r');
    delay(100);
        }
            if(digitalRead(Cible3)){
    bleKeyboard.press('t');
    delay(100);
    bleKeyboard.release('t');
    delay(100);
      }
  }

}
