#!/bin/bash

# how to run bash drawAll.sh ../../background/LUXR4Background100_newnewtrim_rootfiles/
newfolder=new
folder=$1
if [[ "$#" -gt 1 ]]; then
  newfolder=$2
fi

function sourceName(){
  if [ "$s" = "Kr85" ]; then name="^{85}Kr beta decay";fi
  if [ "$s" = "Rn220" ]; then name="^{212}Pb beta decay";fi
  if [ "$s" = "Rn222" ]; then name="^{214}Pb beta decay";fi
  if [ "$s" = "Xe127" ]; then name="^{127}Xe beta decay";fi
}

for s in Kr85 Rn220 Rn222; do
  ls ${folder}/XENON/${s}/rootfiles/*.root > test1.txt;
  sourceName
  mkdir -p ${newfolder}
  root -q -b -l "LUXSimForDiffBetaDecayInXenon.C(\"test1.txt\", \"${s}.root\", \"${newfolder}/${s}\", \"${name}\")"
done
