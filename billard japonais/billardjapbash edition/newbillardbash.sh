#!/bin/bash
function main {
setterm --cursor off
setterm --blank 0
oldIFS=$IFS	
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
		      choixnbjoueurs
		      startgame ;;
		"0" ) echo "Au revoir..." 
		      killall aplay
		      quitter
		      exit 0 ;; 
	esac

done
}
function startgame {
	while true
	do
	tput sgr0
	tput setaf 2
    tput setab 0
    sleep 1
    clear
    cat <<- FIN
                                                                    
                                         +------------+------------+
           30            30              |  joueurs   |   scores   |
          ( )            ( )             +------------+------------|
                                         |1           |            |
    10            100           10       |2           |            |
    ( )           ( )           ( )      |3           |            |
                                         |4           |            |
          50              50             |5           |            |
          ( )            ( )             |6           |            |
                                         |7           |            |
     20           20            20       |8           |            |
    ( )           ( )           ( )      +------------+---+--------+
                                         |                         |
                                         | Joueur en cour :        |
                                         |                         |
                                         |                         |
                                         |                         |
                                         |                         |
                                         |                         |
                                         |                         |
Q Quitter O pour valider le score et passer au joueur suivant______|
FIN
#find /home/adm1/public/ -size +1k -delete
sleep 1
tput setaf 1
IFS=$oldIFS
	for i in $(seq 1 $nbjoueurs)
	do
		score=${TabScores[$i]}
		
		case $i in
		1)
		tput cup 4 42
		printf "%.11s" ${i} ${NomJoueurs[$i]}
		tput cup 4 55
		printf "%03d" $score
		;;
		2)
		tput cup 5 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 5 55
		printf "%03d" $score
		;;
		3)
		tput cup 6 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 6 55
		printf "%03d" $score
		;;
		4)
		tput cup 7 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 7 55
		printf "%03d" $score
		;;
		5)
		tput cup 8 42
		printf "%.11s" ${i}${NomJoueurs[$i]}
		tput cup 8 55
		printf "%03d" $score
		;;
		6)
		tput cup 9 42
		printf "%.9s" ${i}${NomJoueurs[$i]}
		tput cup 9 55
		printf "%03d" $score
		;;
		7)
		tput cup 10 42
		printf "%.9s" ${i}${NomJoueurs[$i]}
		tput cup 10 55
		printf "%03d" $score
		;;
		8)
		tput cup 11 42
		printf "%.9s" ${i}${NomJoueurs[$i]}
		tput cup 11 55
		printf "%03d" $score
		;;
		esac
	done


	tput cup 14 64
	echo "$Joueurencour"
	
	tput cup 20 43
	
	reponsem1=0
	
	
		until [[ $reponsem1 =~ ^[oQ]$ ]] 
		do
		IFS=";"
		tail -n 1 /home/adm1/Public/val2.csv > /home/adm1/Public/values.csv
		read a1 a2 a3 a4 a5 a6 a7 a8 a9 a10 score < /home/adm1/Public/values.csv
        tput setaf 1
		if [[ $a1 -lt 17 ]]
		then
		tput cup 3 11
		echo "O"
		else
		tput cup 3 11
		echo " "
		fi
		if [[ $a2 -lt 17 ]]
		then
		tput cup 3 26
		echo "O"
		else
		tput cup 3 26
		echo " "
		fi
		if [[ $a3 -lt 17 ]]
		then
		tput cup 6 5
		echo "O"
		else
		tput cup 6 5
		echo " "
		fi
		if [[ $a4 -lt 17 ]]
		then
		tput cup 6 19
		echo "O"
		else
		tput cup 6 19
		echo " "
		fi
		
		if [[ $a5 -lt 17 ]]
		then
		tput cup 6 33
		echo "O"
		else
		tput cup 6 33
		echo " "
		fi
		
		if [[ $a6 -lt 17 ]]
		then
		tput cup 9 11
		echo "O"
		else
		tput cup 9 11
		echo " "
		fi
		if [[ $a7 -lt 17 ]]
		then
		tput cup 9 26
		echo "O"
		else
		tput cup 9 26
		echo " "
		fi
		if [[ $a8 -lt 17 ]]
		then
		tput cup 12 5
		echo "O"
		else
		tput cup 12 5
		echo " "
		fi
		if [[ $a9 -lt 17 ]]
		then
		tput cup 12 19
		echo "O"
		else
		tput cup 12 19
		echo " "
		fi
		
		if [[ $a10 -lt 17 ]]
		then
		tput cup 12 33
		echo "O"
		else
		tput cup 12 33
		echo " "
		fi
		tput setaf 2
		tput cup 15 0
		echo "                                         |                         |"
		echo "                                         |                         |"
		echo "                                         |                         |"
		echo "                                         |                         |"
		echo "                                         |                         |"
		tput cup 15 0
		figlet "           $score"
		read -n1 -t1 reponsem1 
		done

	
	case "${reponsem1}" in
	
		"o" )
		reponsemjs=0
		until [[ $reponsemjs =~ ^[oOnN]$ ]] 
	    do
	    clear
	    figlet "JOUEUR $Joueurencour ${NomJoueurs[$Joueurencour]}" 
	    figlet "score ${TabScores[$Joueurencour]}" 
	    figlet "Valider les $score points ? (O/N)" 
	    read -p "" reponsemjs
	    done
		if [[ $reponsemjs =~ [oO] ]]
		then
	    TabScores[$Joueurencour]=$(( TabScores[$Joueurencour]+$score ))
		joueursuivant
		sleep 1
		fi
		;;
		
		"Q" )echo "Au revoir..." 
		     quitter
		      ;; 
	esac
done
}
function affichagemainmenu {
	#tput blink
	tput setaf 1
    tput setab 0
    clear
    tput blink
	echo ""
	figlet "          Billard"
	figlet "  Japonais"
tput sgr0
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
tput blink
	echo "Auteur: Jean-Baptiste GUIRAUD" | beep -c -f 400 -D 50 -l 10
tput sgr0
	tput setaf 7
    tput setab 0
	tput cup 18 1
	echo " - 1 Lancer la partie"
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
function joueursuivant {
    if [[ $Joueurencour -lt $nbjoueurs ]]
	then
	Joueurencour=$((Joueurencour+1))
	else
	Joueurencour=1
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
		TabScores[$i]=0
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
}

function quitter {
tput sgr0
clear
setterm --cursor on
beep    -f 130  -l 100 \
     -n -f 262  -l 100 \
     -n -f 330  -l 100 \
     -n -f 392  -l 100 \
     -n -f 523  -l 100 \
     -n -f 660  -l 100 \
     -n -f 784  -l 300 \
     -n -f 660  -l 300 \
     -n -f 146  -l 100 \
     -n -f 262  -l 100 \
     -n -f 311  -l 100 \
     -n -f 415  -l 100 \
     -n -f 523  -l 100 \
     -n -f 622  -l 100 \
     -n -f 831  -l 300 \
     -n -f 622  -l 300 \
     -n -f 155  -l 100 \
     -n -f 294  -l 100 \
     -n -f 349  -l 100 \
     -n -f 466  -l 100 \
     -n -f 588  -l 100 \
     -n -f 699  -l 100 \
     -n -f 933  -l 300 \
     -n -f 933  -l 100 \
     -n -f 933  -l 100 \
     -n -f 933  -l 100 \
-n -f 1047 -l 400
exit 0	
}



#debut du script
main
sleep 1
exit 0                                                 
