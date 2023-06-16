/*
Projet de billard japonais électronique
Le billard japonais est un jeu issu des fêtes foraines et casinos des années 1930.
Le billard japonais est, malgré son nom, d'origine française.
C’est sans doute la simplicité de sa règle qui a fait de ce jouet en bois un incontournable des fêtes foraines mais également des casinos d’antan.
Source : https://www.lamaisondubillard.com/jeux-en-bois-traditionnels/1238-billard-japonais.html

Le projet consiste ici à reprendre le concepte de billard japonais et à automatier le décompte des points pour gérer une partie de 1 à 4 équipes.
La taille réduite de ce projet, en fait un candidat idéal pour le jeu de comptoir entre amis.
Ce projet s'appuie sur la téchnologie Arduino.

Matériel requis :
Partier électronique 
1 ATmega 2560 ou 1 ATmega Pro mini
2 Boutons poussoir
1 Buzzer
1 intérupteur (pour couper le son)
1 écran 20*04 avec module I2C
10 Photoresistances
10 Potentiomètre De Précision 3296
10 Billes diamètre 15 ou 16 (verre opaque ou céramique ou acier)
Conception d'un pcb dédié pour faciliter le raccordement des photoresitances + potentiomètre à l'arduino

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
  La partie à trois états :
    Initialisation de la partie
    Partie en cours
    Partie finie

Initialisation de la partie :
les joueurs déterminent au moyen du bouton A le nombre de joueurs et valident leur choix au moyen du bouton B

Partie en cours :
Le jouer désigné joue ses 10 billes et valide son score au moyen du bouton B
  Selon le cas, il faut passer soit :
    Au joueur suivant
    Au tour suivant, joueur 1
    A la fin de partie.

Partie finie :
Affichage du tableau de classement des joueurs
Je joueur valide la fin de partie pour retourner au menu d'initialisation au moyen du bouton B.




Biliothèques utilisées :
LiquidCrystal_I2C (Wire.h, LiquidCrystal_I2C.h)

Liste des ports utilisé
Audio : port 11
photoResistances : ports A1 à A10
Utilisation du moniteur série pour le calibrage au moyen des potentiomètre de précision 3296 des photorésistances et le débogage. 
Boutons : Bonton A port 6
          Bonton B port 7        
écran : 




 */



#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/*
Projet de billard japonais électronique
Déclaration des variables
 */
int buttonValueA = 0;
int buttonValueB = 0;
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

//variables pour écran
String l1;
String l2;
String l3;
String l4;

//Variable pour gestion du jeu nb joueurs nb tours tableau scores
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
int classement[] = {0,0,0,0,0,0};
int nbjoueurs = 1;
int nbtours = 3;
int joueurencours = 1;
int tourEncours = 1;
boolean initialisation = true;
boolean partieEncours = false;
boolean partieFinie = false;

// the setup routine runs once when you press reset:
void setup() {                
Serial.begin(9600);
pinMode(11, OUTPUT);//buzzer
 pinMode(6, INPUT_PULLUP); 
 pinMode(7, INPUT_PULLUP);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
}

// the loop routine runs over and over again forever:
void loop() {

  if (initialisation==true){
    InitGame();
  }  
 
  if (partieEncours==true) {
    jouer();
    GererInterruption();
    }

if ( partieFinie==true ) {
    triclassement();
    
    AfficheFin();
    
    
    do {
    buttonValueB = digitalRead(7);
    }
    while(  buttonValueB==HIGH );
    TemporisationBouton();
    initialisation = true;
    partieFinie = false;
       

}
afficherSerial();
}


    
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
      do{ // boucle do/while

        classement[0]=0; // sera utilisé dans le tri comme flag de permutation

        for (int i = 1 ; i < 4 ; i++){ // on parcourt le tableau

            if (scores[classement[i]]<scores[classement[i+1]]){ // si le score de l'élément est < à l'élément suivant, on permute
                    classement[0]=classement[i];          // on stocke le premier élément
                    classement[i]=classement[i+1];          // on remonte le suivant
                    classement[i+1]=classement[0];          // on termine la permutation
                    classement[0]=1;                // on passe le flag de permutation à 1
                  Serial.print ("S1=");
                  Serial.print( classement[1] );
                  Serial.print ("=S2=");
                  Serial.print( classement[2] );
                                    Serial.print ("=S3=");
                  Serial.print( classement[3] );
                                    Serial.print ("=S4=");
                  Serial.println( classement[4] );
                    
            } // fin if

        } // fin for

         } while (classement[0]==1); // tant qu'on a fait une permutation, on recommence.



      
   //*******************************************************************************************************   
   
}


//Déclaration des fonctions
void afficherSerial()
{
/*    Serial.print ("P1=");
    Serial.print(  photo1 );
    Serial.print (" P2=");
    Serial.print(  photo2 );
    Serial.print (" P3=");
    Serial.print(  photo3 );
    Serial.print (" P4=");
    Serial.print(  photo4 );
    Serial.print (" P5=");
    Serial.print(  photo5 );
    Serial.print (" P6=");
    Serial.print(  photo6 );
    Serial.print (" P7=");
    Serial.print(  photo7 );
    Serial.print (" P8=");
    Serial.print(  photo8 );
    Serial.print (" P9=");
    Serial.print(  photo9 );
    Serial.print (" P10=");
    Serial.print(  photo10 );
    Serial.print(" Score = ");
    Serial.print( score );
    Serial.print(" B1 ");
    Serial.print(buttonValueA);
    Serial.print(" B2 ");
    Serial.println(buttonValueB);
*/
Serial.println ("P1=" + String(photo1) + " P2=" + String(photo2) + " P3=" + String(photo3) + " P4=" + String(photo4) + " P5=" + String(photo5) +
" P6=" + String(photo6) + " P7=" + String(photo7) + " P8=" + String(photo8) + " P9=" + String(photo9) + " P10=" + String(photo10) +
" Score = " + String(score) + " ValB1 = " + String(buttonValueA) + " ValB2 = " + String(buttonValueB)
);

}
//Fonction musique coin mario
void playCoin()
{
    tone(11,988,125);
    delay(125);
    noTone(11);
    tone(11,1319,600);
    delay(600);
    noTone(11);
}

//Fonction lecture photoresistance
void Readphotores()
{
    photo1 = analogRead(A1);
    photo2 = analogRead(A2);
    photo3 = analogRead(A3);
    photo4 = analogRead(A4);
    photo5 = analogRead(A5);
    photo6 = analogRead(A6);
    photo7 = analogRead(A7);
    photo8 = analogRead(A8);
    photo9 = analogRead(A9);
    photo10 = analogRead(A10);
}

//Fonction attetente boutons relachés
void TemporisationBouton()
{
    while( buttonValueA==LOW || buttonValueB==LOW )
    {
      buttonValueA = digitalRead(6);
      buttonValueB = digitalRead(7); 
    }
}


//Fonction calcul score
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

void Afficher()
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

void AfficherEnJeu()
{
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
 
  void AfficheFin()
{
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

 void InitGame(){

    oldscore =0;
    score = -1;
    tourEncours = 1;
    joueurencours = 1;
    scores[1] = 0;
    scores[2] = 0;
    scores[3] = 0;
    scores[4] = 0;    
   
    
    l1 = " BILLARD            ";
    l2 = "   JAPONAIS         ";
    l3 = " NB JOUEURS         ";
    l4 = "                " + String(nbjoueurs) + "    ";
    Afficher();
    buttonValueA = digitalRead(6);
    buttonValueB = digitalRead(7); 
    //playCoin();
    
    if (buttonValueA==LOW){
    TemporisationBouton();  
      if (nbjoueurs == 4)
    {nbjoueurs =1;}
    else
    {nbjoueurs++;}
    
    l4 = "                " + String(nbjoueurs) + "    ";
    
    Afficher();  
    
  }
    
      if (buttonValueB==LOW)
      { 
      TemporisationBouton();  
      playCoin();
      initialisation = false;
      partieEncours = true;
      }
 }
 
 void jouer(){
   
  oldscore = score;
  Readphotores();
  calculScore ();
  buttonValueA = digitalRead(6);
  buttonValueB = digitalRead(7); 
    if ( score != oldscore ){
    if ( score == 340 ){
    playCoin();
    }
   AfficherEnJeu();
    }
  
  //afficherSerial();  
 }
 
 void GererInterruption(){
     if (buttonValueB==LOW)
    {TemporisationBouton();
      playCoin();// scores[joueurencours] = scores[joueurencours] + score;    
    scores[joueurencours] += score;
    if ( joueurencours == nbjoueurs && tourEncours == nbtours) {
    playCoin();
    partieEncours = false;
    //initialisation = true;
    partieFinie = true;

    }
    else if (  joueurencours == nbjoueurs ) {
    joueurencours = 1;
     
    tourEncours++;
    AfficherEnJeu();}
    else {
    joueurencours++;
    AfficherEnJeu();  
   }  

    
    }  
 }
