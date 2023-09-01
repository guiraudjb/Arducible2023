#include <Adafruit_NeoPixel.h>
//#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
//#endif
#define PIN        13 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 5 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
#include <TM1637.h>
TM1637 tm0(2, 3);
TM1637 tm1(4, 5);
TM1637 tm2(6, 7);

//Déclaration des variables pour la lecture du hardware
int boutonG = 30;
int boutonD = 31;
int buttonValueA = 0; //stocker la valeur du bouton 1
int buttonValueB = 0; //stocker la valeur du bouton 2
int buzzer = 12;
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
int limiteTest=20;
String padding="";
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
int oldNbJoueurs=0;
int nbjoueurs = 1;
int nbtours = 3;
int JoueurEnCours = 1;
int tourEncours = 1;
boolean initialisation = true;
boolean debutpartie = true;
boolean partieEncours = false;
boolean AnnulerValidation = true;
boolean partieFinie = false;
boolean jouerSons = false;



void setup(){
//#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
//  clock_prescale_set(clock_div_1);
//#endif
pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
pinMode(boutonG, INPUT_PULLUP); // déclaration du pin pour le bouton 1
pinMode(boutonD, INPUT_PULLUP); // déclaration du pin pour le bouton 2
Serial.begin(9600);
tm0.begin();
tm0.setBrightness(4);
tm0.display(8888);
tm1.begin();
tm1.setBrightness(4);
tm1.display(8888);
tm2.begin();
tm2.setBrightness(4);
tm2.display(8888);  tone(buzzer,1319,600);

debugMode();
playCoin();

}

void loop(){
 if (initialisation==true)
    {
    whiteled();
    InitGame();
    }  
    
  if (partieEncours==true) 
    {
    jouer();
    GererInterruption();
    }
  
  if ( partieFinie==true ) 
    {
    triclassement();
  for (int i=1;i<3;i++)
      {
      buttonValueB = digitalRead(boutonD);
        messageTemporise("TOP-PLAYER-IS-" + String(classement[1]),10000);
      }
    TemporisationBouton();
    initialisation = true;
    partieFinie = false;
    }             
 }

// temporisation et appel du mode debug
void debugMode(){
  whiteled();
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();
  while (currentMillis - previousMillis < 10000){
    currentMillis = millis();
    buttonValueA = digitalRead(boutonG);
    buttonValueB = digitalRead(boutonD); 
   
      if ( buttonValueA==0 &&  buttonValueB==0){
          TemporisationBouton();
          while (true){
          Readphotores();
          calculScore ();
          afficherSerial();
            //if(digitalRead(boutonG) && digitalRead(boutonG) ){
            //break;
            //}
            }
          }
          } 
      }
  
//Fonction pour afficher la valeur des capteurs sur le moniteur port série
void afficherSerial(){
  Serial.println ("P1=" + String(photo1) + " P2=" + String(photo2) + " P3=" + String(photo3) + " P4=" + String(photo4) + " P5=" + String(photo5) +
  " P6=" + String(photo6) + " P7=" + String(photo7) + " P8=" + String(photo8) + " P9=" + String(photo9) + " P10=" + String(photo10) +
  " Score = " + String(score) + " ValB1 = " + String(buttonValueA) + " ValB2 = " + String(buttonValueB));
  }
//Fonction pour l'initialisation de la partie (choisir le nombre de joueurs)
void InitGame(){
  oldNbJoueurs=0;
  oldscore =0;
  score = 0;
  tourEncours = 1;
  JoueurEnCours = 1;
  scores[1] = 0;
  scores[2] = 0;
do
{
  buttonValueA = digitalRead(boutonG);
  buttonValueB = digitalRead(boutonD); 
    if (buttonValueA==LOW)
      {
      TemporisationBouton();  
        oldNbJoueurs=nbjoueurs;
        if (nbjoueurs == 2)
          {
          nbjoueurs =1;
          }
        else
          {
          nbjoueurs++;
          }
      }
    
      if (nbjoueurs != oldNbJoueurs)
      {
      
      paddingstr(nbjoueurs);
      message(padding);  
      oldNbJoueurs=nbjoueurs;
      tm1.display("    ");
      tm2.display("    ");
      switch (nbjoueurs) {
      case 1 :
      
      tm1.display(" P1 ");
      tm2.display("    ");
      break;
      case 2 :
            
      tm1.display(" P1 ");
      tm2.display(" P2 ");
      break;
      }
      }     
      
       }
      while(  buttonValueB==HIGH );
      TemporisationBouton();
        
      playCoin();
      initialisation = false;
      whiteled();
      debutpartie = true; 
      partieEncours = true;
      messageTemporise("TURN-" + String(tourEncours), 3000);
      messageTemporise("PLAYER-" + String(JoueurEnCours),3000);
      affichage();
             
  }
//Fonction pour la gestion du jeu en cours (détection bille, calcul score, actualisation affichage, attente action joueur)
void jouer(){
  oldscore = score;
  Readphotores();
  calculScore ();
  afficherSerial();
  buttonValueA = digitalRead(boutonG);
  buttonValueB = digitalRead(boutonD); 
 
  if (debutpartie == true)
  {
    debutpartie = false;  
  }
    if ( score != oldscore )
      {
      playCoin();
      if ( score == 340 )
      {
        affichage();
        mariovictory();
      }
      
      affichage();
      }
      //muter le son avec le bouton gauche
      buttonValueA = digitalRead(boutonG);
    if (buttonValueA == 0){
    TemporisationBouton();
    if (jouerSons == true){
    jouerSons = false;    
    tone(buzzer,350,1000);
    delay(1000);
    noTone(buzzer);
    Serial.println(jouerSons);
    }else{
    jouerSons = true;   
    Serial.println(jouerSons);
    tone(buzzer,750,1000);
    delay(1000);
    noTone(buzzer);
    }
    }
   
  }
//Fonction pour la gestion du jeu en cours (Action à mener suite à une action du joueur) 
 void GererInterruption(){

    if (buttonValueB==LOW)
      {
      message(" GO?");
      TemporisationBouton();
      AnnulerValidation = false;
        do 
          {
          buttonValueA = digitalRead(boutonG);
          buttonValueB = digitalRead(boutonD);
            if (buttonValueA == LOW)
              {
              TemporisationBouton();
              AnnulerValidation = true;
              affichage();
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
          
          playCoin();// scores[JoueurEnCours] = scores[JoueurEnCours] + score;    
          scores[JoueurEnCours] += score;
          affichage();
          //score = 0;
            if ( JoueurEnCours == nbjoueurs && tourEncours == nbtours) 
              {
              playCoin();
              partieEncours = false;
              partieFinie = true;
              messageTemporise("THE END",3000);
              
                         }
            else if (  JoueurEnCours == nbjoueurs ) 
              {
              JoueurEnCours = 1;
              tourEncours++;
              messageTemporise("TURN-" + String(tourEncours), 3000);
              messageTemporise("PLAYER-" + String(JoueurEnCours),4000);
              affichage();
              }
            else 
              {
              JoueurEnCours++;
               messageTemporise("PLAYER-" + String(JoueurEnCours),4000);
              affichage();
              }  
          }  
      }

}



//Fonction pour allumer les leds neopixel
void whiteled(){
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.setPixelColor(1, pixels.Color(255, 255, 255));
  pixels.setPixelColor(2, pixels.Color(255, 255, 255));
  pixels.setPixelColor(3, pixels.Color(255, 255, 255));
  pixels.setPixelColor(4, pixels.Color(255, 255, 255));
  pixels.show();
  }
//Fonction pour récupérer la valeur des photorésistances
void Readphotores(){
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
//Fonction pour calculer le score sur plateau d'après la valeur des photorésistances
void calculScore (){    
  s1  = ( photo1  < limiteTest ?  30 : 0 ) ;           
  s2  = ( photo2  < limiteTest ?  30 : 0 ) ;
  s3  = ( photo3  < limiteTest ?  10 : 0 ) ;
  s4  = ( photo4  < limiteTest ? 100 : 0 ) ;
  s5  = ( photo5  < limiteTest ?  10 : 0 ) ;
  s6  = ( photo6  < limiteTest ?  50 : 0 ) ;
  s7  = ( photo7  < limiteTest ?  50 : 0 ) ;
  s8  = ( photo8  < limiteTest ?  20 : 0 ) ;
  s9  = ( photo9  < limiteTest ?  20 : 0 ) ;    
  s10 = ( photo10 < limiteTest ?  20 : 0 ) ;
  score = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10;
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
  while (classement[0]==1); // tant qu'on a fait une permutati
}

//Fonction pour la temporisation des actions de l'utilisateur(attente boutons relachés)
void TemporisationBouton(){
    while( buttonValueA==LOW || buttonValueB==LOW )
    {
    buttonValueA = digitalRead(boutonG);
    buttonValueB = digitalRead(boutonD); 
    delay(200);
    }
  }

//Fonctions pour gérer les afficheurs tm1637
void affichage(){

switch (JoueurEnCours){
case 1 :
tm1.setBrightness(10);
paddingstr(scores[1]); 
tm1.display(padding);
break;
case 2 :
tm2.setBrightness(10);
paddingstr(scores[2]); 
tm2.display(padding);
break;
}
paddingstr(score);
tm0.display(padding);
}
void messageTemporise(String msg, int duree){
unsigned long startTime = millis();
do
tm0.display(msg)->scrollLeft(500);  
while (millis() < startTime + duree);
}
void message(String msg){
tm0.display(msg);  
}
void paddingstr(int number){
switch (String(number).length()) {
      case 1 :
padding="   " + String(number);
      break;
      case 2 :
padding= "  " + String(number);
      break;
      case 3 :
padding=" " + String(number);
      break;
      case 4 :
padding=String(number);
      break;
}
}

//Fonction pour jouer les musiques
void playCoin(){
   if (jouerSons == true){
  tone(buzzer,988,125);
  delay(125);
  noTone(buzzer);
  tone(buzzer,1319,600);
  delay(600);
   noTone(buzzer);
  }
}
void looserSound(){
   if (jouerSons == true){
tone(buzzer,784 ,250 );
delay(350);
tone(buzzer,622 ,250 );
delay(350);
tone(buzzer,554 ,250 );
delay(350);
tone(buzzer,494 ,1000 );
delay(1000);
}
}
void mariovictory(){
  if (jouerSons == true){
  tone(buzzer, 130 , 100);
  delay(100); 
  tone(buzzer, 262 , 100);
  delay(100); 
  tone(buzzer, 330 , 100);
  delay(100); 
  tone(buzzer, 392 , 100);
  delay(100); 
  tone(buzzer, 523 , 100);
  delay(100); 
  tone(buzzer, 660 , 100);
  delay(100); 
  tone(buzzer, 784 , 300);
  delay(300); 
  tone(buzzer, 660 , 300);
  delay(300); 
  tone(buzzer, 146 , 100);
  delay(100); 
  tone(buzzer, 262 , 100);
  delay(100); 
  tone(buzzer, 311 , 100);
  delay(100); 
  tone(buzzer, 415 , 100);
  delay(100); 
  tone(buzzer, 523 , 100);
  delay(100); 
  tone(buzzer, 622 , 100);
  delay(100); 
  tone(buzzer, 831 , 300);
  delay(300); 
  tone(buzzer, 622 , 300);
  delay(300); 
  tone(buzzer, 155 , 100);
  delay(100); 
  tone(buzzer, 294 , 100);
  delay(100); 
  tone(buzzer, 349 , 100);
  delay(100); 
  tone(buzzer, 466 , 100);
  delay(100); 
  tone(buzzer, 588 , 100);
  delay(100); 
  tone(buzzer, 699 , 100);
  delay(100); 
  tone(buzzer, 933 , 300);
  delay(300); 
  tone(buzzer, 933 , 100);
  delay(100); 
  tone(buzzer, 933 , 100);
  delay(100); 
  tone(buzzer, 933 , 100);
  delay(100); 
  tone(buzzer, 1047 , 400);
  delay(400);
  }
  }
