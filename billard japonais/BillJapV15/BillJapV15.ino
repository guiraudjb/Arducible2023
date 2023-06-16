/*
Projet de billard japonais électronique
Le billard japonais est un jeu issu des fêtes foraines et casinos des années 1930.
Le billard japonais est, malgré son nom, d'origine française.
C’est sans doute la simplicité de sa règle qui a fait de ce jouet en bois un incontournable des fêtes foraines mais également des casinos d’antan.
Source : https://www.lamaisondubillard.com/jeux-en-bois-traditionnels/1238-billard-japonais.html

Le projet consiste ici à reprendre le concept de billard japonais et à automatiser le décompte des points pour gérer une partie de 1 à 4 équipes.

La taille réduite de ce projet, en fait un candidat idéal pour le jeu de comptoir entre amis.

Ce projet s'appuie sur la technologie Arduino.

Matériel requis :
Partie électronique 
1 ATmega 2560 ou 1 ATmega Pro mini
2 Boutons poussoir
1 Buzzer
1 interrupteur (pour couper le son)
1 écran 20*04 avec module I2C
10 Photorésistances GL5516
10 Potentiomètre De Précision 3296w-202 2Kohms
10 Billes diamètre 15 ou 16 (verre opaque, céramique ou acier)
Conception d'un pcb dédié pour faciliter le raccordement des photorésitances + potentiomètre à l'arduino.

Partie menuiserie
  Scie sauteuse
  Perceuse
  Défonceuse
  Imprimante 3D (Support cartes et supports photorésistances)
  Vis

Outillage 
Partie électronique et programmation :
  Un ordinateur avec les logiciels arduino et Pcbnew(Kicad)
  Fer à souder
  Tournevis

Algorithmie
La partie se joue en trois tours avec 1 à 4 joueurs.
  La partie a trois états :
    Initialisation de la partie
    Partie en cours
    Partie finie

Initialisation de la partie :
les joueurs déterminent au moyen du bouton A le nombre de joueurs et valident leur choix au moyen du bouton B

Partie en cours :
Le joueur désigné joue ses 10 billes et valide son score au moyen du bouton B
  Selon le cas, il faut passer soit :
    Au joueur suivant
    Au tour suivant, joueur 1
    A la fin de partie.

Partie finie :
Affichage du tableau de classement des joueurs.
Utilisation d'un tri à bulles.
Le joueur valide la fin de partie pour retourner au menu d'initialisation au moyen du bouton B.

Biliothèques utilisées :
LiquidCrystal_I2C (Wire.h, LiquidCrystal_I2C.h) (gestion de l'écran)
Adafruit_NeoPixel (gestion du bandeau de led)


Liste des ports utilisé
Lumières : port 10
Audio : port 11
Tilt : port 12
photoResistances : ports A0 à A9
Utilisation du moniteur série pour le calibrage au moyen des potentiomètre de précision 3296 des photorésistances et le débogage. 
Boutons : Bonton A port 18
          Bonton B port 19        
écran : port 20(SDA) et 21(SCL)
*/


//Appel des bibliothèques extérieures
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
clock_prescale_set(clock_div_1);
#endif
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        10 // On Trinket or Gemma, suggest changing this to 1
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 10 // Popular NeoPixel ring size
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);






//Déclaration des variables pour la lecture du hardware
int buttonValueA = 0; //stocker la valeur du bouton 1
int buttonValueB = 0; //stocker la valeur du bouton 2

// initialisation des 10 variables pour le stockage des valeurs de photorésistances.
int photo1 = 0;
int photo2 = 0;
int photo3 = 0;
int photo4 = 0;
int photo5 = 0;
int photo6 = 0;
int photo7 = 0;
int photo8 = 0;
int photo9 = 0;
int photo10 = 0;

//Variables pour gestion de la couleur d'ambiance 
int r = 0;
int g = 0;
int b = 0;

//Variables pour gestion du jeu nb joueurs nb tours tableau scores
int s1 = 0;
int s2 = 0;
int s3 = 0;
int s4 = 0;
int s5 = 0;
int s6 = 0;
int s7 = 0;
int s8 = 0;
int s9 = 0;
int s10 = 0;
int oldscore =0;
int score = 0;
int scores[] = {0,0,0,0,0};
int classement[] = {0,0,0,0,0};
int nbjoueurs = 1;
int nbtours = 3;
int joueurencours = 1;
int tourEncours = 1;
boolean initialisation = true;
boolean debutpartie = true;
boolean partieEncours = false;
boolean AnnulerValidation = true;
boolean partieFinie = false;

//variables pour écran
String l1;
String l2;
String l3;
String l4;



// Define the bit patters for each of our custom chars. These
// are 5 bits wide and 8 dots deep
uint8_t custChar[8][8] = {
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
},// Small top line - 0

{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
},// Small bottom line - 1


{
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
},// Small lines top and bottom -2


{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
}, // -3



{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
},// Left bottom chamfer full - 4

{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
},// Right top chamfer full -5


{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
},// Right bottom chamfer full -6

{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
},// Left top chamfer full -7





  
};

// Define our numbers 0 thru 9
// 254 is blank and 255 is the "Full Block"
uint8_t bigNums[38][6] = {
  {7, 0, 5, 4, 1, 6}, //0
  {0, 5, 254, 1, 255, 1},     //1
  {0, 0, 5, 4, 2, 2},         //2
  {3, 3, 5, 2, 2, 6},         //3
  {4, 1, 5, 254, 254, 255},   //4
  {255, 3, 3, 2, 2, 6},       //5 
  {7, 3, 3, 4, 2, 6},         //6 
  {0, 0, 5, 254, 7, 254},     //7 
  {7, 3, 5, 4, 2, 6},         //8 
  {7, 3, 5, 254, 254, 6},     //9
  {0, 3, 5, 254, 2, 254},//? 10
  {254, 255, 254, 254, 2, 254},//! 11 
 {7, 3, 5, 255, 254, 255}, //A 12
 {255, 3, 6, 255, 2, 5}, //B 13
 {7, 0, 0, 4, 1, 1}, //C 14
 {255, 0, 5, 255, 1, 6}, //D 15
 {255, 3, 3, 255, 2, 2}, //E 16
 {255, 3, 3, 255, 254, 254}, //F 17
 {7, 0, 0, 4, 1, 5}, //G 18
{255, 1, 255, 255, 254, 255},  //H 19
 {0, 255, 0, 1, 255, 1}, //I 20
 {254, 254, 255,1,1, 6}, //J 21
 {255, 1, 6, 255, 254, 5}, //K 22
 {255, 254, 254, 255, 1, 1}, //L 23
 {5,1,7,255,254,255}, //M 24
 {5,1,255,255,0,4}, //N 25
 {7, 0, 5, 4, 1, 6}, //O 26
 {255, 3, 5, 255, 254, 254}, //P 27
 {7,0,5,4,1,255}, //Q 28
 {255, 3, 5, 255, 254,5}, //R 29
 {7, 3, 3, 2, 2,6}, //S 30
 {0, 255, 0, 254, 255, 254}, //T 31
 {255, 254, 255, 4, 1, 6}, //U 32
 {5, 254, 7, 4, 1, 6}, //V 33
 {5, 1, 7, 4,255, 6}, //W 34
 {4, 1, 6, 7, 254, 5}, //X 35
 {4,1,6,254,255,254}, //Y 36
 {0,3,6,7,2,1}, //Z 37
  
};





// Boucle pour la première initialisation du jeu
void setup() {                
Serial.begin(9600);

//Déclaration des ports. Le port 10 pour la lumière est défini plus haut.
pinMode(11, OUTPUT);//buzzer sur le pin 11 pour les effets sonores

pinMode(18, INPUT_PULLUP); // déclaration du pin pour le bouton 1
pinMode(19, INPUT_PULLUP); // déclaration du pin pour le bouton 2
lcd.init();// initialize the lcd 
lcd.backlight();
pixels.begin();
pixels.clear();
pixels.setPixelColor(0, pixels.Color(255, 255, 255));
pixels.setPixelColor(1, pixels.Color(255, 255, 255));
pixels.setPixelColor(2, pixels.Color(255, 255, 255));
pixels.setPixelColor(3, pixels.Color(255, 255, 255));
pixels.setPixelColor(4, pixels.Color(255, 255, 255));
pixels.setPixelColor(5, pixels.Color(255, 255, 255));
pixels.setPixelColor(6, pixels.Color(255, 255, 255));
pixels.setPixelColor(7, pixels.Color(255, 255, 255));
pixels.setPixelColor(8, pixels.Color(255, 255, 255));
pixels.setPixelColor(9, pixels.Color(255, 255, 255));
pixels.show();   // Mise a jour de la couleur des leds.

lcd.clear();
for (int cnt = 0; cnt < sizeof(custChar) / 8; cnt++) {
    lcd.createChar(cnt, custChar[cnt]);
  }
/*  
  // Now print the numbers 0 to 9
  // Format is: number2print, startCol, startRow
  printBigNum(0, 0, 0);
  printBigNum(1, 4, 0);
  printBigNum(2, 8, 0);
  printBigNum(3, 12, 0);
  printBigNum(4, 16, 0);
  printBigNum(5, 0, 2);
  printBigNum(6, 4, 2);
  printBigNum(7, 8, 2);
  printBigNum(8, 12, 2);
  printBigNum(9, 16, 2);
delay(500);

lcd.clear();
  printBigNum(10, 0, 0);
  printBigNum(11, 4, 0);
  printBigNum(12, 8, 0);
  printBigNum(13, 12, 0);
  printBigNum(14, 16, 0);
  printBigNum(15, 0, 2);
  printBigNum(16, 4, 2);
  printBigNum(17, 8, 2);
  printBigNum(18, 12, 2);
  printBigNum(19, 16, 2);
delay(500);
lcd.clear();
  printBigNum(20, 0, 0);
  printBigNum(21, 4, 0);
  printBigNum(22, 8, 0);
  printBigNum(23, 12, 0);
  printBigNum(24, 16, 0);
  printBigNum(25, 0, 2);
  printBigNum(26, 4, 2);
  printBigNum(27, 8, 2);
  printBigNum(28, 12, 2);
  printBigNum(29, 16, 2);
delay(500);
lcd.clear();
  printBigNum(30, 0, 0);
  printBigNum(31, 4, 0);
  printBigNum(32, 8, 0);
  printBigNum(33, 12, 0);
  printBigNum(34, 16, 0);
  printBigNum(35, 0, 2);
  printBigNum(36, 4, 2);
  printBigNum(37, 8, 2);
delay(500);
AfficherTourSuivant();
delay(3000);
*/

Police();
}


// -----------------------------------------------------------------
// Print big number over 2 lines, 3 colums per half digit
// -----------------------------------------------------------------
void printBigNum(int number, int startCol, int startRow) {

  // Position cursor to requested position (each char takes 3 cols plus a space col)
  lcd.setCursor(startCol, startRow);

  // Each number split over two lines, 3 chars per line. Retrieve character
  // from the main array to make working with it here a bit easier.
  uint8_t thisNumber[6];
  for (int cnt = 0; cnt < 6; cnt++) {
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
}




// Boucle Principale du jeu
void loop() 
  {
    /*
    Readphotores();
    calculScore ();
    afficherSerial();
    */
  if (initialisation==true)
    {
    whiteled();
    InitGame();
    }  
    
  if (partieEncours==true) 
    {
    jouer();
    GererInterruption();
    afficherSerial();
    }
  
  if ( partieFinie==true ) 
    {
    triclassement();
    AfficheFin();
    Musique();  
    do 
      {
      buttonValueB = digitalRead(19);
      }
    while(  buttonValueB==HIGH );
    TemporisationBouton();
    initialisation = true;
    partieFinie = false;
    }
  }




//Fonction pour la lecture des valeurs des photoresistances
void Readphotores()
  {
  photo1 = analogRead(A0);
  photo2 = analogRead(A1);
  photo3 = analogRead(A2);
  photo4 = analogRead(A3);
  photo5 = analogRead(A4);
  photo6 = analogRead(A5);
  photo7 = analogRead(A6);
  photo8 = analogRead(A7);
  photo9 = analogRead(A8);
  photo10 = analogRead(A9);
  }

//Fonction pour le calcul du score
void calculScore () 
  {    
  s1  = ( photo1  < 10 ?  30 : 0 ) ;           
  s2  = ( photo2  < 10 ?  30 : 0 ) ;
  s3  = ( photo3  < 10 ?  10 : 0 ) ;
  s4  = ( photo4  < 10 ? 100 : 0 ) ;
  s5  = ( photo5  < 10 ?  10 : 0 ) ;
  s6  = ( photo6  < 10 ?  50 : 0 ) ;
  s7  = ( photo7  < 10 ?  50 : 0 ) ;
  s8  = ( photo8  < 10 ?  20 : 0 ) ;
  s9  = ( photo9  < 10 ?  20 : 0 ) ;    
  s10 = ( photo10 < 10 ?  20 : 0 ) ;
  score = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10;
  }
 
//Fonction pour l'affichage sur le moniteur série
void afficherSerial()
  {
  Serial.println ("P1=" + String(photo1) + " P2=" + String(photo2) + " P3=" + String(photo3) + " P4=" + String(photo4) + " P5=" + String(photo5) +
  " P6=" + String(photo6) + " P7=" + String(photo7) + " P8=" + String(photo8) + " P9=" + String(photo9) + " P10=" + String(photo10) +
  " Score = " + String(score) + " ValB1 = " + String(buttonValueA) + " ValB2 = " + String(buttonValueB));
  }

//Fonction pour jouer la musique coin mario
void playCoin()
  {
  tone(11,988,125);
  delay(125);
  noTone(11);
  tone(11,1319,600);
  delay(600);
  noTone(11);
  }

void LumiereRandom()
  {
  r = random(254);
  g = random(254);
  b = random(254);
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.setPixelColor(1, pixels.Color(r, g, b));
  pixels.setPixelColor(2, pixels.Color(r, g, b));
  pixels.setPixelColor(3, pixels.Color(r, g, b));
  pixels.setPixelColor(4, pixels.Color(r, g, b));
  pixels.setPixelColor(5, pixels.Color(r, g, b));
  pixels.setPixelColor(6, pixels.Color(r, g, b));
  pixels.setPixelColor(7, pixels.Color(r, g, b));
  pixels.setPixelColor(8, pixels.Color(r, g, b));
  pixels.setPixelColor(9, pixels.Color(r, g, b));
  pixels.show();
  }

//Fonction pour basculer l'éclairage en blanc.
void whiteled()
  {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.setPixelColor(1, pixels.Color(255, 255, 255));
  pixels.setPixelColor(2, pixels.Color(255, 255, 255));
  pixels.setPixelColor(3, pixels.Color(255, 255, 255));
  pixels.setPixelColor(4, pixels.Color(255, 255, 255));
  pixels.setPixelColor(5, pixels.Color(255, 255, 255));
  pixels.setPixelColor(6, pixels.Color(255, 255, 255));
  pixels.setPixelColor(7, pixels.Color(255, 255, 255));
  pixels.setPixelColor(8, pixels.Color(255, 255, 255));
  pixels.setPixelColor(9, pixels.Color(255, 255, 255));
  pixels.show();
  }

//Fonction pour la temporisation des actions de l'utilisateur(attente boutons relachés)
void TemporisationBouton()
  {
    while( buttonValueA==LOW || buttonValueB==LOW )
    {
    buttonValueA = digitalRead(18);
    buttonValueB = digitalRead(19); 
    }
  }

//Fonction pour l'initialisation de la partie (choisir le nombre de joueurs)
void InitGame()
  {
  oldscore =0;
  score = 0;
  tourEncours = 1;
  joueurencours = 1;
  scores[1] = 0;
  scores[2] = 0;
  scores[3] = 0;
  scores[4] = 0;    
  l1 = " BILLARD            ";
  l2 = "   JAPONAIS         ";
  l3 = "Nb joueurs : " + String(nbjoueurs) + "      ";
  l4 = "<- NB JOUEURS  GO ->";

  Afficherinit();
  buttonValueA = digitalRead(18);
  buttonValueB = digitalRead(19); 
    if (buttonValueA==LOW)
      {
      TemporisationBouton();  
        if (nbjoueurs == 4)
          {
          nbjoueurs =1;
          }
        else
          {
          nbjoueurs++;
          }
      }
    if (buttonValueB==LOW)
      { 
      TemporisationBouton();  
      playCoin();
      initialisation = false;
      whiteled();
      debutpartie = true; 
      partieEncours = true;
      }
  }

//Fonction pour la gestion du jeu en cours (détection bille, calcul score, actualisation affichage, attente action joueur)
void jouer()
  {
  oldscore = score;
  Readphotores();
  calculScore ();
  buttonValueA = digitalRead(18);
  buttonValueB = digitalRead(19); 
  if (debutpartie == true)
  {
  AfficherEnJeu();
  debutpartie = false;  
  }
    if ( score != oldscore )
      {
      AfficherEnJeu();
      Afficherbillesalecran();
     // AfficherEnJeu();
      }
  }

 
//Fonction pour la gestion du jeu en cours (Action à mener suite à une action du joueur) 
 void GererInterruption()
{
    if (buttonValueB==LOW)
      {
      TemporisationBouton();
      AnnulerValidation = false;
      l1 = "Retirer les billes! ";
      l2 = "Points:          " + String(score);
      l3 = "Joueur: "+ String(joueurencours) + "  Tour: "+ String(tourEncours) + "  ";
      l4 = "NON<- VALIDER? ->OUI";
      Afficherinit();
      buttonValueA = digitalRead(18);
      buttonValueB = digitalRead(19); 
        do 
          {
          buttonValueA = digitalRead(18);
          buttonValueB = digitalRead(19);
            if (buttonValueA == LOW)
              {
              TemporisationBouton();
              AnnulerValidation = true;
              AfficherEnJeu();  
              break;
              }
          }
          while(  buttonValueB==HIGH );
      TemporisationBouton();
      
      
        if (AnnulerValidation == false)
          {
          if (score < 100)
          {
          looserSound();
          }
          
          playCoin();// scores[joueurencours] = scores[joueurencours] + score;    
          scores[joueurencours] += score;
          score = 0;
            if ( joueurencours == nbjoueurs && tourEncours == nbtours) 
              {
              playCoin();
              partieEncours = false;
              partieFinie = true;
              }
            else if (  joueurencours == nbjoueurs ) 
              {
              joueurencours = 1;
              tourEncours++;
              AfficherTourSuivant();
              AfficherJoueurSuivant();
              AfficherEnJeu();
              }
            else 
              {
              joueurencours++;
              AfficherJoueurSuivant();
              AfficherEnJeu();  
              }  
          }  
      }
}




//Fonction pour l'affichage en mode initialisation du jeu
void Afficherinit()
{

  lcd.setCursor(0,0);
  lcd.print(l1);
  lcd.setCursor(0,1);
  lcd.print(l2);
  lcd.setCursor(0,2);
  lcd.print(l3);
  lcd.setCursor(0,3);
  lcd.print(l4);
}
  
  //Fonction pour afficher les billes validées à l'écran

void AfficherTourSuivant()
{
lcd.clear();
printBigNum(31, 2, 1);
printBigNum(26, 5, 1);
printBigNum(32, 8, 1);
printBigNum(29, 11, 1); 
printBigNum(tourEncours,15,1);
delay(2000);
}

void AfficherJoueurSuivant()
{
lcd.clear();
printBigNum(21, 6, 1);
printBigNum(joueurencours,11,1);
delay(2000);  
}
  
  void Afficherbillesalecran()
{
  /*
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    30( )  30( )    ");
  lcd.setCursor(0,1);
  lcd.print("10( )  100( )  10( )");
  lcd.setCursor(0,2);
  lcd.print("    50( )  50( )    ");
  lcd.setCursor(0,3);
  lcd.print("20( )   20( )  20( )");
    if ( photo1 < 12 )
      {
        lcd.setCursor(7,0);
        lcd.print("O");
    }
    if ( photo2 < 12 )
      {
      lcd.setCursor(14,0);
      lcd.print("O");
      }
      if ( photo3 < 12 )
    {
    lcd.setCursor(3,1);
    lcd.print("O");
    }
    if ( photo4 < 12 )
    {
    lcd.setCursor(11,1);
    lcd.print("O");
    }
    if ( photo5 < 12 )
    {
    lcd.setCursor(18,1);
    lcd.print("O");
    }
    if ( photo6 < 12 )
    {
    lcd.setCursor(7,2);
    lcd.print("O");
    }
    if ( photo7 < 12 )
    {
    lcd.setCursor(14,2);
    lcd.print("O");
    }
    if ( photo8 < 12 )
    {
    lcd.setCursor(3,3);
    lcd.print("O");
    }
    if ( photo9 < 12 )
    {
    lcd.setCursor(11,3);
    lcd.print("O");
    }
    if ( photo10 < 12 )
    {
    lcd.setCursor(18,3);
    lcd.print("O");
    }
    */
    if ( score == 340 )
    {
    mariovictory();
    }
    else
    {
    playCoin();
    //delay(750);
    }
}

//Fonction pour l'affichage en cours de jeu
void AfficherEnJeu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,0);
  lcd.print("J1 :          JOUEUR");
  lcd.setCursor(5,0);
  lcd.print("     ");
  lcd.setCursor(5,0);
  lcd.print(String(scores[1])); 

  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("J2 :                ");
  lcd.setCursor(5,1);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(String(scores[2]));
  lcd.setCursor(17,1);
  lcd.print(String(joueurencours));  
  
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("J3 :                ");
  lcd.setCursor(5,2);
  lcd.print("     ");
  lcd.setCursor(5,2);
  lcd.print(String(scores[3])); 
  lcd.setCursor(16,2);
  lcd.print(String("   "));
  lcd.setCursor(16,2);
  lcd.print(String(score));
  
  lcd.setCursor(0,3);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("J4 :        TOUR :  ");
  lcd.setCursor(5,3);
  lcd.print("     ");
  lcd.setCursor(5,3);
  lcd.print(String(scores[4]));
  lcd.setCursor(19,3);
  lcd.print(String(tourEncours));
}

//Affichage de fin de partie
void AfficheFin(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("                    ");
  lcd.setCursor(0,0);
  lcd.print(" 1ER : J" + String(classement[1])+ " SCORE " + String(scores[classement[1]]));
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print(" 2EM : J" + String(classement[2])+ " SCORE " + String(scores[classement[2]]));
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print(" 3EM : J"  + String(classement[3])+ " SCORE " + String(scores[classement[3]]));
  lcd.setCursor(0,3);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print(" 4EM : J"  + String(classement[4])+ " SCORE " + String(scores[classement[4]]));
}

//Fonction tri à bulle pour le classement des joueurs en fin de partie
void triclassement(){
//***********************************************************************************************
// classer classement[] = {0,0,0,0,0};int scores[] = {0,0,0,0,0};
// pour ne pas faire le tri plusieurs fois, sera mis à 1 à la fin du traitement, remis à 0 au départ de la partie suivante
// remplissage du tableau de classement
classement[1]=1;
classement[2]=2;
classement[3]=3;
classement[4]=4;
// tri à bulle
  do
    { // boucle do/while
    classement[0]=0; // sera utilisé dans le tri comme flag de permutation
      for (int i = 1 ; i < 4 ; i++)
        { // on parcourt le tableau
          if (scores[classement[i]]<scores[classement[i+1]])
            { // si le score de l'élément est < à l'élément suivant, on permute
            classement[0]=classement[i];          // on stocke le premier élément
            classement[i]=classement[i+1];          // on remonte le suivant
            classement[i+1]=classement[0];          // on termine la permutation
            classement[0]=1;                // on passe le flag de permutation à 1
            //Serial.print ("S1=");
            //Serial.print( classement[1] );
            //Serial.print ("=S2=");
            //Serial.print( classement[2] );
            //Serial.print ("=S3=");
            //Serial.print( classement[3] );
            //Serial.print ("=S4=");
            //Serial.println( classement[4] );
            } // fin if
          } // fin for
    } 
  while (classement[0]==1); // tant qu'on a fait une permutation, on recommence.
//*******************************************************************************************************   
}



void mariovictory()
  {
  tone(11, 130 , 100);
  delay(100); 
  tone(11, 262 , 100);
  delay(100); 
  tone(11, 330 , 100);
  delay(100); 
  tone(11, 392 , 100);
  delay(100); 
  tone(11, 523 , 100);
  delay(100); 
  tone(11, 660 , 100);
  delay(100); 
  tone(11, 784 , 300);
  delay(300); 
  tone(11, 660 , 300);
  delay(300); 
  tone(11, 146 , 100);
  delay(100); 
  tone(11, 262 , 100);
  delay(100); 
  tone(11, 311 , 100);
  delay(100); 
  tone(11, 415 , 100);
  delay(100); 
  tone(11, 523 , 100);
  delay(100); 
  tone(11, 622 , 100);
  delay(100); 
  tone(11, 831 , 300);
  delay(300); 
  tone(11, 622 , 300);
  delay(300); 
  tone(11, 155 , 100);
  delay(100); 
  tone(11, 294 , 100);
  delay(100); 
  tone(11, 349 , 100);
  delay(100); 
  tone(11, 466 , 100);
  delay(100); 
  tone(11, 588 , 100);
  delay(100); 
  tone(11, 699 , 100);
  delay(100); 
  tone(11, 933 , 300);
  delay(300); 
  tone(11, 933 , 100);
  delay(100); 
  tone(11, 933 , 100);
  delay(100); 
  tone(11, 933 , 100);
  delay(100); 
  tone(11, 1047 , 400);
  delay(400);
  }

void Police()
  {
    for (int i = 1 ; i <= 1; i++ )
    { 
    pixels.setPixelColor(5, pixels.Color(255, 0, 0));
    pixels.setPixelColor(6, pixels.Color(255, 0, 0));
    pixels.setPixelColor(7, pixels.Color(255, 0, 0));
    pixels.setPixelColor(8, pixels.Color(255, 0, 0));
    pixels.setPixelColor(9, pixels.Color(255, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    tone(11,435,1000);
    delay(800);
    pixels.setPixelColor(5, pixels.Color(0, 0, 255));
    pixels.setPixelColor(6, pixels.Color(0, 0, 255));
    pixels.setPixelColor(7, pixels.Color(0, 0, 255));
    pixels.setPixelColor(8, pixels.Color(0, 0, 255));
    pixels.setPixelColor(9, pixels.Color(0, 0, 255));
    pixels.show();   // Send the updated pixel colors to the hardware.
    tone(11,580,1000);
    delay(800);
    }
  }

void looserSound()
{
tone(11,784 ,250 );
delay(350);
tone(11,622 ,250 );
delay(350);
tone(11,554 ,250 );
delay(350);
tone(11,494 ,1000 );
delay(1000);
}
void Musique()
{
buttonValueB = digitalRead(19);   
while( buttonValueB==HIGH )
{
 tone(11,82.41 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,103.83 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,123.47 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,82.41 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,103.83 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,123.47 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,103.83 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,123.47 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,103.83 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,123.47 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,123.47 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,123.47 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,82.41 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,110.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,130.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,82.41 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,110.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,130.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,110.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,130.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,110.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,130.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,130.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,130.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,415.30 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,415.30 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,415.30 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,415.30 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,415.30 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,415.30 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,880.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,880.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,293.66 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,174.61 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,293.66 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,174.61 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,293.66 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,349.23 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,261.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,164.81 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,440.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,293.66 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,174.61 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,293.66 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,174.61 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,293.66 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,220.00 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,349.23 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,659.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,587.33 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,207.65 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,493.88 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,523.25 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,246.94 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 
 
 tone(11,329.63 ,120 );
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
}  
delay(120);
buttonValueB = digitalRead(19);
LumiereRandom();
if ( buttonValueB == LOW )
{
break;
} 

}
}
