#!/bin/bash
function main #menu principal
{
while true
do

	playmusique vidtomp3.wav
	affichagemainmenu
	tput cup 20 35
	read -p "Votre choix: " reponsem0
		until [[ $reponsem0 =~ ^[012]$ ]] 
		do
			tput cup 20 35
			read -p "Votre choix: " reponsem0
		done

	case "${reponsem0}" in
	
		"1" ) killall aplay
		      afficherlesregles ;;
		"2" ) killall aplay
		      choixnbjoueurs
		      startgame ;;
		"0" ) echo "Au revoir..." 
		      killall aplay
		      clear
		      exit 0 ;; 
	esac

done
}

function startgame {
	while true
	do
	tput sgr0
	tput setaf 7
    tput setab 0
    clear
	figlet joueur $Joueurencour -f future &
	aplay -q j${Joueurencour}.wav 2>/dev/null
	sleep 1
	figlet cible ${TabCibles[$Joueurencour]} -f future &
	aplay -q c${TabCibles[$Joueurencour]}.wav 2>/dev/null
		 
    clear
    cat <<- FIN
                    *                    +------------+---+--------+
                   * *                   |joueur      |src|taux de |
                  * 6 *                  |            |   |réussite|
                   * *                   +------------+---+--------|
       *            *            *       |1           |   |        |
      * *                       * *      |2           |   |        |
     * 4 *                     * 5 *     |3           |   |        |
      * *                       * *      |4           |   |        |
       *                         *       |5           |   |        |
                                         |6           |   |        |
                   (7)                   |7           |   |        |
                                         |8           |   |        |
       *                         *       +------------+---+--------+
      * *                       * *      |                         |
     * 2 *                     * 3 *     | Joueur en cour    :     |
      * *                       * *      |                         |
       *            *            *       | Boules restantes  :     |
                   * *                   |                         |
                  * 1 *                  | Cible à atteindre :     |
                   * *                   |                         |
                    *                    |                         |
Q Quitter S sauvegarder la partie________|_________________________|
FIN
		tput setaf 1
		tput bold 
        case ${TabCibles[$Joueurencour]} in
		1)
		tput cup 18 20
		printf 1
		;;
		
		2)
		tput cup 14 7
		printf 2
		;;
		
		3)
		tput cup 14 33
		printf 3
		;;
		4)
		tput cup 6 7
		printf 4
		;;
		5)
		tput cup 6 33
		printf 5
		;;

		6)
		tput cup 2 20
		printf 6
		;;

		7)
		tput cup 10 20
		printf 7
		;;
		esac


	for i in $(seq 1 $nbjoueurs)
	do
		score=${TabScores[$i]}
		reussite=${TabTauxDereussite[$i]}
		reussite=${reussite/./,}

		
		case $i in
		1)
		tput cup 4 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 4 55
		printf "%03d" $score
		tput cup 4 59
		printf "%08.4f" $reussite
		;;
		2)
		tput cup 5 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 5 55
		printf "%03d" $score
		tput cup 5 59
		printf "%08.4f" $reussite
		;;
		3)
		tput cup 6 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 6 55
		printf "%03d" $score
		tput cup 6 59
		printf "%08.4f" $reussite
		;;
		4)
		tput cup 7 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 7 55
		printf "%03d" $score
		tput cup 7 59
		printf "%08.4f" $reussite
		;;
		5)
		tput cup 8 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 8 55
		printf "%03d" $score
		tput cup 8 59
		printf "%08.4f" $reussite
		;;
		6)
		tput cup 9 42
		printf "%.9s" ${i}${NomJoueurs[$i]}
		tput cup 9 55
		printf "%03d" $score
		tput cup 9 59
		printf "%08.4f" $reussite
		;;
		7)
		tput cup 10 42
		printf "%.9s" ${i}${NomJoueurs[$i]}
		tput cup 10 55
		printf "%03d" $score
		tput cup 10 59
		printf "%08.4f" $reussite
		;;
		8)
		tput cup 11 42
		printf "%.9s" ${i}${NomJoueurs[$i]}
		tput cup 11 55
		printf "%03d" $score
		tput cup 11 59
		printf "%08.4f" $reussite
		;;
		esac
	done

	tput sgr0
	tput setaf 7
    tput setab 4


	tput cup 14 64
	printf "%02d" $Joueurencour

	tput cup 16 64
	printf "%02d" $BoulesRestantes

	tput cup 18 64
	printf "%02d" ${TabCibles[$Joueurencour]}
	tput cup 19 43
	printf "Tour Numéro $NumeroDeTour"
	tput sgr0
	tput setaf 7
	tput setab 0
	tput cup 20 43
	read -p "Cible touchée (o/n) ? " reponsem1
		until [[ $reponsem1 =~ ^[onQ]$ ]] 
		do
		   tput cup 20 43
		   read -p "Cible touchée (o/n) ? " reponsem1
		done

	
	
	case "${reponsem1}" in
	
		"o" ) touche ;;
		"n" ) pastouche ;;
		"Q" ) echo "Au revoir..." 
		      break 1
		      ;; 
    esac
done
}

function touche { 
clear
TabCiblesatteintes[$Joueurencour]=$(bc -q <<< scale=4\;${TabCiblesatteintes[$Joueurencour]}+1)
TabNbTirsEffectues[$Joueurencour]=$(bc -q <<< scale=4\;${TabNbTirsEffectues[$Joueurencour]}+1)
TabTauxDereussite[$Joueurencour]=$(bc -q <<< scale=4\;${TabCiblesatteintes[$Joueurencour]}*100/${TabNbTirsEffectues[$Joueurencour]})
TabPoints[$Joueurencour]=$(bc -q <<< scale=4\;${TabPoints[$Joueurencour]}+1)
TabPointsBonus[$Joueurencour]=$(bc -q <<< scale=4\;${TabPointsBonus[$Joueurencour]}+${TabBonus[$Joueurencour]})
TabPointsStyle[$Joueurencour]=0
TabScores[$Joueurencour]=$(bc -q <<< scale=4\;${TabPoints[$Joueurencour]}+${TabPointsBonus[$Joueurencour]}+${TabPointsStyle[$Joueurencour]})
TabBonus[$Joueurencour]=$(bc -q <<< scale=4\;${TabBonus[$Joueurencour]}+1)
ciblesuivante
echo "joueur $Joueurencour ; Tour $NumeroDeTour ; boule $BoulesRestantes ; points ACQUIS ${TabPointsBonus[$Joueurencour]} ; Points ${TabPoints[$Joueurencour]} ; score total ${TabScores[$Joueurencour]} ; points bonus applicable au prochain tour${TabBonus[$Joueurencour]}" >> log
testPourPriseDecision
}

function pastouche {
clear
TabNbTirsEffectues[$Joueurencour]=$(bc -q <<< scale=4\;${TabNbTirsEffectues[$Joueurencour]}+1)
TabTauxDereussite[$Joueurencour]=$(bc -q <<< scale=4\;${TabCiblesatteintes[$Joueurencour]}*100/${TabNbTirsEffectues[$Joueurencour]})
TabBonus[$Joueurencour]=0
echo "joueur $Joueurencour ; Tour $NumeroDeTour ; boule $BoulesRestantes ; points ACQUIS ${TabPointsBonus[$Joueurencour]} ; Points ${TabPoints[$Joueurencour]} ; score total ${TabScores[$Joueurencour]} ; points bonus applicable au prochain tour${TabBonus[$Joueurencour]}" >> log
testPourPriseDecision
}

function testPourPriseDecision {
if [[ $Joueurencour -eq $nbjoueurs && $NumeroDeTour -eq 7 && $BoulesRestantes -eq 1 ]]
	then 
		echo "partie finie"
		affichageresultats
		read a
		exit 0
		break
		
	else
		if [[ $Joueurencour -eq $nbjoueurs && $BoulesRestantes -eq 1 ]]
		then
		toursuivant

		else

			if [[ $BoulesRestantes -eq 1 ]]
			then
			joueursuivant

			else
			boulesuivante
			fi
		fi
	fi
}

function boulesuivante {
if [[ $BoulesRestantes -ge 1 ]]
then
BoulesRestantes=$((BoulesRestantes-1))
fi 
}

function ciblesuivante {
	if [[ ${TabCiblesatteintes[$Joueurencour]} -lt 7 ]] 
	then
	TabCibles[$Joueurencour]=$((${TabCibles[$Joueurencour]}+1))
	else
	TabCibles[$Joueurencour]=$(( ( RANDOM % 7 )  + 1 ))
	fi 
}

function joueursuivant { 
playmusique js.wav
	if [[ $Joueurencour -lt $nbjoueurs ]]
	then
	Joueurencour=$((Joueurencour+1))
	else
	Joueurencour=1
	fi
BoulesRestantes=3
figlet ${NomJoueurs[$Joueurencour]}
sleep 2
espeak ${NomJoueurs[$Joueurencour]} -v french -s 140 -p 10
}

function toursuivant {
playmusique js.wav
Joueurencour=1
BoulesRestantes=3
NumeroDeTour=$((NumeroDeTour+1))
figlet ${NomJoueurs[$Joueurencour]}
sleep 2
espeak ${NomJoueurs[$Joueurencour]} -v french -s 140 -p 10
}

function afficherlesregles {
#partie 1
clear

tput cup 22 79
read a

    cat <<- FIN
                    *                    +------------+---+--------+
                   * *                   |joueur      |src|taux de |
                  * 6 *                  |            |   |réussite|
                   * *                   +------------+---+--------|
       *            *            *       |1           |   |        |
      * *                       * *      |2           |   |        |
     * 4 *                     * 5 *     |3           |   |        |
      * *                       * *      |4           |   |        |
       *                         *       |5           |   |        |
                                         |6           |   |        |
                   (7)                   |7           |   |        |
                                         |8           |   |        |
       *                         *       +------------+---+--------+
      * *                       * *      |                         |
     * 2 *                     * 3 *     | Joueur en cour    :     |
      * *                       * *      |                         |
       *            *            *       | Boules restantes  :     |
                   * *                   |                         |
                  * 1 *                  | Cible à atteindre :     |
                   * *                   |                         |
                    *                    |                         |
Q Quitter S sauvegarder la partie________|_________________________|
FIN

#partie 2
clear
cat <<- FIN
********************************************************************************
*1                                                                             *
*2                                                                             *
*3                                                                             *
*4                                                                             *
*5                                                                             *
*6           #─│┌┐└┘├┤┬┴┼█░▒■▲▼●─│┌┐└┘├┤┬┴┼█░▒■▲▼●                             *
*7                                                                             *
*8                                                                             *
*9                                                                             *
*10                                                                            *
*11                                                                            *
*12                                                                            *
*13                                                                            *
*14                                                                            *
*15                                                                            *
*16                                                                            *
*17                                                                            *
*18                                                                            *
*19                                                                            *
*20                                                                            *
*22                                                                            *
*********************************************Appuyer sur enter pour continuer.**
FIN
tput cup 22 79
read a
}

function affichagemainmenu {
	#tput blink
	tput setaf 1
    tput setab 0
    clear
	echo ""
	figlet "          Le defi"
	figlet " des tireurs"
	toilet -f future "   Bash Edition" --gay
	tput setaf 3
    tput setab 0
	for i in {0..22};do tput cup $i 0; echo "*";done
	for i in {0..50};do tput cup 0 $i; echo "*";done
	for i in {0..50};do tput cup 22 $i; echo "*";done
	for i in {0..22};do tput cup $i 50; echo "*";done
	for i in {0..50};do tput cup 17 $i; echo "*";done
	tput setaf 1
    tput setab 0
	tput cup 16 11
	echo "Auteur: Jean-Baptiste GUIRAUD" | beep -c -f 400 -D 50 -l 10
	tput setaf 7
    tput setab 0
	tput cup 18 1
	echo " - 1 Voir les règles du jeux."
	tput cup 19 1
	echo " - 2 Lancer la partie"
	tput cup 20 1
	echo " - 0 Quitter"
}

function playmusique {

	if [[ $(pidof aplay | wc -c) -eq 0 ]]
	then
		#echo "pas de musique"
		aplay -q $1 2>/dev/null &
	#sleep 3
	else
		#echo "musique en cours"
		killall aplay
		aplay -q $1 2>/dev/null &
	#sleep 3
	fi	
}

function choixnbjoueurs {
clear
	while true
	do
	tput cup 0 0
	echo -n "Veuillez indiquer un nombre de joueurs de 1 à 8. "
	read nbjoueurs
		
		case "${nbjoueurs}" in
		
			[1-8] ) echo "Vous avez choisi ${nbjoueurs} joueur(s) " 
		
			break
			;;
		esac
	done
	

	for i in $(seq 1 $nbjoueurs)
	do
		TabCibles[$i]=1
		TabCiblesatteintes[$i]=0
		TabNbTirsEffectues[$i]=0
		TabScores[$i]=0
		TabPoints[$i]=0
		TabPointsBonus[$i]=0
		TabBonus[$i]=0
		TabPointsStyle[$i]=0
		TabTauxDereussite[$i]=0
		NomJoueurs[$i]=""
	done
	
		for i in $(seq 1 $nbjoueurs)
		do
		echo -n "Veuillez saisir le nom du joueur $i : "
		read nom
			until [[ ${#nom} -le 20 ]] 
			do
			echo -n "Veuillez saisir le nom du joueur $i : "
			read nom
			done
		
		
		NomJoueurs[$i]=$nom
		done
Joueurencour=1
BoulesRestantes=3
NumeroDeTour=1
}

function affichageresultats {
clear
cat <<- FIN
********************************************************************************
                                 Classement :                                   
+------------+---+--------+--------+--------+
|RANG|src|taux de |
+------------+---+--------|
| 1          |   |        |
+------------+---+--------|
| 2          |   |        |
+------------+---+--------|
| 3          |   |        |
+------------+---+--------|
| 4          |   |        |
+------------+---+--------|
| 5          |   |        |
+------------+---+--------|
| 6          |   |        |
+------------+---+--------|
| 7          |   |        |
+------------+---+--------|
| 8          |   |        |
+------------+---+--------|

*********************************************Appuyer sur enter pour continuer.**
FIN
for i in {1..8}; do echo "Joueur $i ${TabScores[$i]}";done | sort -nr -k 3 | cut -f 2 -d " "
}

#debut du script

main
sleep 1
tput sgr0
clear
exit 0                                                 
