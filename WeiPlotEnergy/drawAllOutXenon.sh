#!/bin/bash

# how to run bash drawAll.sh ../../background/LUXR4Background100_newnewtrim_rootfiles/
newfolder=new
folder=$1
if [[ "$#" -gt 1 ]]; then
  newfolder=$2
fi

function sourceName(){
  if [ "$s" = "Th232E" ]; then name="^{232}Th early decay chain";fi
  if [ "$s" = "Th232L" ]; then name="^{232}Th late decay chain";fi
  if [ "$s" = "Ra226" ]; then name="^{238}U late decay chain";fi
  if [ "$s" = "K40" ]; then name="^{40}K decay";fi
  if [ "$s" = "Co60" ]; then name="^{60}Co decay";fi
  if [ "$s" = "Kr85" ]; then name="^{85}Kr beta decay";fi
  if [ "$s" = "Rn220" ]; then name="^{212}Pb beta decay";fi
  if [ "$s" = "Rn222" ]; then name="^{214}Pb beta decay";fi
  if [ "$s" = "Xe127" ]; then name="^{127}Xe beta decay";fi
}

for s in Th232E Th232L Ra226 K40 Co60; do
  ls ${folder}/${s}/rootfiles/*.root > ${newfolder}.txt;
  sourceName
  mkdir -p ${newfolder}
  root -q -b -l "LUXSimForDiffBetaDecayOutXenon.C(\"${newfolder}.txt\", \"${newfolder}/${s}.root\", \"${newfolder}/${s}\", \"${name}\")"
done
