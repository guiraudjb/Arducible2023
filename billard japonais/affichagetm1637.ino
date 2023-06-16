#include <TM1637.h>
TM1637 tm0(2, 3);
TM1637 tm1(4, 5);
TM1637 tm2(6, 7);
TM1637 tm3(8, 9);
TM1637 tm4(10, 11);

int tour=2;
int score=0;
String PanneauScore="134";
int longeur=0;
String Ecran="";

int scores[] = {0,20,50,100,340};
String scorestring="";
int nbjoueurs = 4;
int joueurencours = 1;
int tourEncours = 1;



void setup()
{
Serial.begin(9600);
tm0.begin();
tm0.setBrightness(5);
tm1.begin();
tm1.setBrightness(10);
tm2.begin();
tm2.setBrightness(1);
tm3.begin();
tm3.setBrightness(1);
tm4.begin();
tm4.setBrightness(1);


}

void loop()
{
affichage();

Serial.println(String(score).length());


}

void affichage()
{


 
for (int i = 1; i <= 4; i++) {
switch (i) {
      case 1 :
tm1.display(" J1 ")->blink(500);
      break;
      case 2 :
tm2.display(" J2 ");
      break;
      case 3 :
tm3.display(" J3 ");
      break;
            case 4 :
tm4.display(" J4 ");
      break;
}
  
  
  }

/*
tm1.display(" J1 ")->blink(500);
tm2.display(" J2 ");
tm3.display(" J3 ");
tm4.display(20);
*/
switch (String(score).length()) {
      case 1 :
PanneauScore=String(tour) + "  " + String(score);
      break;
      case 2 :
PanneauScore=String(tour) + " " + String(score);
      break;
      case 3 :
PanneauScore=String(tour) + String(score);
      break;
}

/*if(String(score).length()==2)
{
PanneauScore=String(tour) + " " + String(score);
}
*/
tm0.display(PanneauScore);


}


void preparationaffichagescore()
{
switch (String(score).length()) {
      case 1 :
scorestring=
      break;
      case 2 :
PanneauScore=String(tour) + " " + String(score);
      break;
      case 3 :
PanneauScore=String(tour) + String(score);
      break;
}

}
}
