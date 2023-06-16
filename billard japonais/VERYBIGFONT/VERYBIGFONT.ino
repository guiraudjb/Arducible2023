//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

uint8_t THG[8]  = 
{
  B11111,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000,
  B10000,
  B00000
};

uint8_t TBG[8]  = 
{
  B00000,
  B10000,
  B10000,
  B11000,
  B11100,
  B11110,
  B11110,
  B11111
};
uint8_t TBD[8] = {
  B00000,
  B00001,
  B00001,
  B00011,
  B00111,
  B01111,
  B01111,
  B11111
};
uint8_t THD[8] = {
  B11111,
  B01111,
  B01111,
  B00111,
  B00011,
  B00001,
  B00001,
  B00000
};
uint8_t BH[8]  = {
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
uint8_t BB[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111
};
uint8_t BTH[8] = {
  B11111,
  B11111,
  B01110,
  B01110,
  B00100,
  B00100,
  B00000,
  B00000
};
uint8_t BTB[8] = {
  B00000,
  B00000,
  B10001,
  B10001,
  B11011,
  B11011,
  B11111,
  B11111
};



uint8_t bigNums[40][12] = {
{2, 255, 1, 255, 254, 255, 255, 254, 255, 3, 255, 0},         //Pos : 0 Carac : 0
{2, 255, 254, 254, 255, 254, 254, 255, 254, 5, 255, 5},       //Pos : 1 Carac : 1
{2, 255, 1, 254, 2, 0, 2, 0, 254, 255, 255, 255},             //Pos : 2 Carac : 2
{2, 255, 1, 254, 5, 255, 254, 4, 255, 3, 255, 0},             //Pos : 3 Carac : 3
{2, 254, 255, 255, 254, 255, 255, 255, 255, 254, 254, 255},   //Pos : 4 Carac : 4
{255, 255, 255, 255, 254, 254, 4, 4, 255, 5, 5, 0},           //Pos : 5 Carac : 5
{255, 4, 4, 255, 5, 5, 255, 254, 255, 255, 255, 255},         //Pos : 6 Carac : 6
{255, 255, 255, 254, 2, 0, 254, 255, 254, 254, 255, 254},     //Pos : 7 Carac : 7
{2, 255, 1, 255, 5, 255, 255, 4, 255, 3, 255, 0},             //Pos : 8 Carac : 8
{255, 4, 155, 255, 5, 255, 254, 254, 255, 5, 5, 255},         //Pos : 9 Carac : 9
{2, 255, 1, 6, 2, 0, 254, 255, 254, 254, 5, 254},             //Pos : 10 Carac : ?
{254, 255, 254, 254, 255, 254, 254, 255, 254, 254, 5, 254},   //Pos : 11 Carac : !
{2, 255, 1, 255, 254, 255, 255, 4, 255, 255, 254, 255},       //Pos : 12 Carac : A
{255, 255, 1, 255, 5, 0, 255, 4, 1, 255, 255, 0},             //Pos : 13 Carac : B
{2, 255, 1, 255, 254, 254, 255, 254, 254, 3, 255, 0},         //Pos : 14 Carac : C
{255, 255, 1, 255, 254, 255, 255, 254, 255, 255, 255, 0},     //Pos : 15 Carac : D
{255, 255, 255, 255, 5, 254, 255, 4, 254, 255, 255, 255},     //Pos : 16 Carac : E
{255, 255, 255, 255, 5, 254, 255, 4, 254, 255, 254, 254},     //Pos : 17 Carac : F
{255, 255, 255, 255, 254, 5, 255, 254, 255, 255, 255, 255},   //Pos : 18 Carac : G
{255, 254, 255, 255, 5, 255, 255, 4, 255, 255, 254, 255},     //Pos : 19 Carac : H
{254, 4, 254, 254, 255, 254, 254, 255, 254, 254, 255, 254},   //Pos : 20 Carac : I
{255, 255, 255, 254, 255, 254, 254, 255, 254, 255, 0, 254},   //Pos : 21 Carac : J
{255, 2, 0, 255, 0, 254, 255, 1, 254, 255, 3, 1},             //Pos : 22 Carac : K
{255, 254, 254, 255, 254, 254, 255, 254, 254, 255, 255, 255}, //Pos : 23 Carac : L
{255, 7, 255, 255, 6, 255, 255, 254, 255, 255, 254, 255},     //Pos : 24 Carac : M
{1, 254, 255, 255, 1, 255, 255, 3, 255, 255, 254, 3},         //Pos : 25 Carac : N
{2, 255, 1, 255, 4, 255, 255, 5, 255, 3, 255, 0},             //Pos : 26 Carac : O
{255, 255, 255, 255, 254, 255, 255, 4, 4, 255, 254, 254},     //Pos : 27 Carac : P
{2, 255, 1, 255, 254, 255, 255, 254, 255, 3, 255, 255},       //Pos : 28 Carac : Q
{2, 255, 1, 255, 254, 0, 255, 3, 1, 255, 254, 3},             //Pos : 29 Carac : R
{2, 255, 1, 3, 1, 254, 254, 3, 1, 3, 255, 0},                 //Pos : 30 Carac : S
{255, 255, 255, 254, 255, 254, 254, 255, 254, 254, 255, 254}, //Pos : 31 Carac : T
{255, 254, 255, 255, 254, 255, 255, 254, 255, 255, 255, 255}, //Pos : 32 Carac : U
{255, 254, 255, 255, 254, 255, 3, 7, 0, 254, 6, 254},         //Pos : 33 Carac : V
{255, 254, 255, 255, 254, 255, 255, 255, 255, 6, 254, 6},     //Pos : 34 Carac : W
{1, 254, 2, 3, 255, 0, 2, 255, 1, 0, 254, 3},                 //Pos : 35 Carac : X
{255, 254, 255, 3, 255, 0, 2, 0, 255, 0, 254, 254},           //Pos : 36 Carac : Y
{255, 255, 255, 254, 2, 0, 2, 0, 254, 255, 255, 255},         //Pos : 37 Carac : Z
{254, 254, 254, 254, 254, 254, 254, 255, 254, 254, 0, 254},   //Pos : 38 Carac : ,
{254, 5, 254, 254, 4, 254, 254, 5, 254, 254, 4, 254},         //Pos : 39 Carac : :
};


void setup()
{
  lcd.init();                    // initialize the lcd 
  lcd.backlight();
  lcd.createChar(0, THG);
  lcd.createChar(1, TBG);
  lcd.createChar(2, TBD);
  lcd.createChar(3, THD);
  lcd.createChar(4, BH);
  lcd.createChar(5, BB);
  lcd.createChar(6, BTH);
  lcd.createChar(7, BTB);
  lcd.clear();  
  lcd.home();
  
}


void loop()
{

printBigNum(0, 0, 0);
printBigNum(1, 4, 0);
printBigNum(2, 8, 0);
printBigNum(3, 12, 0);
printBigNum(4, 16, 0);

delay (6000);


}

void printBigNum(int number, int startCol, int startRow) {

  // Position cursor to requested position (each char takes 3 cols plus a space col)
  lcd.setCursor(startCol, startRow);

  // Each number split over two lines, 3 chars per line. Retrieve character
  // from the main array to make working with it here a bit easier.
  uint8_t thisNumber[12];
  for (int cnt = 0; cnt < 12; cnt++) {
    thisNumber[cnt] = bigNums[number][cnt];
  }

  // First line (top half) of digit
  for (int cnt = 0; cnt < 3; cnt++) {
    lcd.print((char)thisNumber[cnt]);
  }

  // Now position cursor to next line at same start column for digit
  lcd.setCursor(startCol, startRow + 1);

  // 2nd line (bottom half)
  for (int cnt = 3; cnt < 6; cnt++) {
    lcd.print((char)thisNumber[cnt]);
  }
  lcd.setCursor(startCol, startRow + 2);
  // 3nd line (bottom half)
  for (int cnt = 6; cnt < 9; cnt++) {
    lcd.print((char)thisNumber[cnt]);
  }
  lcd.setCursor(startCol, startRow + 3);
  // 4nd line (bottom half)
  for (int cnt = 9; cnt < 12; cnt++) {
    lcd.print((char)thisNumber[cnt]);
  }
}
