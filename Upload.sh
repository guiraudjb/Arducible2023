#!/bin/bash
echo " motif de mise à jour ?"
read motif
git status 
git add --all
git commit -m"$motif"
git push -u origin main
exit 0
