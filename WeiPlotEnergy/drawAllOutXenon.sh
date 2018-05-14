#!/bin/bash

# how to run bash drawAll.sh ../../background/LUXR4Background100_newnewtrim_rootfiles/
newfolder=new
folder=$1
if [[ "$#" -gt 1 ]]; then
  newfolder=$2
fi

function sourceName(){
  if [ "$s" = "Th232E" ]; then name="^{232}Th early decay chain"; activity=3; fi
  if [ "$s" = "Th232L" ]; then name="^{232}Th late decay chain"; activity=7; fi
  if [ "$s" = "Ra226" ]; then name="^{238}U late decay chain"; activity=6; fi
  if [ "$s" = "K40" ]; then name="^{40}K decay";activity=1; fi
  if [ "$s" = "Sc46" ]; then name="^{46}Sc decay";activity=1; fi
  if [ "$s" = "Co60" ]; then name="^{60}Co decay";activity=1;fi
  if [ "$s" = "Kr85" ]; then name="^{85}Kr beta decay";activity=1;fi
  if [ "$s" = "Rn220" ]; then name="^{212}Pb beta decay";activity=1;fi
  if [ "$s" = "Rn222" ]; then name="^{214}Pb beta decay";activity=1;fi
  if [ "$s" = "Xe127" ]; then name="^{127}Xe beta decay";activity=1;fi
}

for s in Th232E Th232L Ra226 K40 Co60 Sc46; do
  ls ${folder}/${s}/rootfiles/*.root > ${newfolder}.txt;
  sourceName
  echo ${name} ${activity}
  mkdir -p ${newfolder}
  root -q -b -l "LUXSimForDiffBetaDecayOutXenon.C(\"${newfolder}.txt\", \"${newfolder}/${s}.root\", \"${newfolder}/${s}\", \"${name}\", ${activity})"
done
