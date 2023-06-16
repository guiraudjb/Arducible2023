#!/bin/bash
IFS=";"
tput setaf 1
tput setab 7
sleep 2 
clear 
sleep 1
beep
while true 
do
tail -n 1 /home/adm1/public/val2.csv > /home/adm1/public/values.csv
read  a1 a2 a3 a4 a5 a6 a7 a8 a9 a10 score < /home/adm1/public/values.csv
clear
tput cup 20 1
echo ""
figlet "                                                 $score"
sleep 1

if [[ $score -eq 340 ]]
then
beep -f 130 -l 100 -n -f 262 -l 100 -n -f 330 -l 100 -n -f 392 -l 100 -n -f 523 -l 100 -n -f 660 -l 100 -n -f 784 -l 300 -n -f 660 -l 300 -n -f 146 -l 100 -n -f 262 -l 100 -n -f 311 -l 100 -n -f 415 -l 100 -n -f 523 -l 100 -n -f 622 -l 100 -n -f 831 -l 300 -n -f 622 -l 300 -n -f 155 -l 100 -n -f 294 -l 100 -n -f 349 -l 100 -n -f 466 -l 100 -n -f 588 -l 100 -n -f 699 -l 100 -n -f 933 -l 300 -n -f 933 -l 100 -n -f 933 -l 100 -n -f 933 -l 100 -n -f 1047 -l 400
fi


done

