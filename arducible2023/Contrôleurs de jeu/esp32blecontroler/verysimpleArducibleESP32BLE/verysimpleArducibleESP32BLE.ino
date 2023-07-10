/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("NewArduciblePetanqueGame");
const int Cible1 = 25;
const int Cible2 = 26;
const int Cible3 = 27;
const int btn1 = 18;
const int btn2 = 19;

void setup() {
  bleKeyboard.begin();
  pinMode(Cible1, INPUT);
  pinMode(Cible2, INPUT);
  pinMode(Cible3, INPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
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

 if(digitalRead(btn1)) {
        bleKeyboard.release(KEY_RIGHT_ARROW); 
    } else {
        bleKeyboard.press(KEY_RIGHT_ARROW);
    }
      
 if(digitalRead(btn2)) {
        bleKeyboard.release(KEY_LEFT_ARROW); 
    } else {
        bleKeyboard.press(KEY_LEFT_ARROW);
    }
  }
}
      
