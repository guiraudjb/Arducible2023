#include <Keyboard.h>
const int b1 = 10;
const int Cible1 = 14;   
const int Cible2 = 15;   
const int Cible3 = 16;
const int CapteurChoc = 8;

void setup() {                
  pinMode(b1, INPUT_PULLUP);
  pinMode(Cible1,INPUT);
  pinMode(Cible2,INPUT); 
  pinMode(Cible3,INPUT); 
  pinMode(CapteurChoc, INPUT);
  Keyboard.begin();
delay(3000);  
}


void loop(){
    if(digitalRead(b1)) {
    Keyboard.release(KEY_RIGHT_ARROW); 
    //delay(100);
    } else {
        Keyboard.press(KEY_RIGHT_ARROW);
    //delay(100);
    }

    if(digitalRead(CapteurChoc)) {
      Keyboard.press('f');
    //delay(100);
    } else {
      Keyboard.release('f');
    //delay(100);
    }
    

    if(digitalRead(Cible1)) {
       Keyboard.press('e');  
    //delay(100);  
    } else {
    Keyboard.release('e');
    //delay(100);
    }
    
    if(digitalRead(Cible2)) {
        Keyboard.press('r');
      //  delay(100);
    } else {
        Keyboard.release('r');
       // delay(100); 
    }
    if(digitalRead(Cible3)) {
        Keyboard.press('t');
        //delay(100);
    } else {
        Keyboard.release('t');
        //delay(100);
    }
 
}
