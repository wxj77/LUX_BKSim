#!/bin/bash
folder=$1

function sourceName(){
  if [ "$s" = "Kr85" ]; then name="^{85}Kr";fi
  if [ "$s" = "Rn220" ]; then name="^{212}Pb";fi
  if [ "$s" = "Rn222" ]; then name="^{214}Pb";fi
  if [ "$s" = "Xe127" ]; then name="^{127}Xe";fi
}

for s in Kr85 Rn220 Rn222 Xe127; do
  ls ${folder}/XENON/${s}/rootfiles/*.root > test1.txt;
  sourceName
  root -q -b -l "LUXSimForDiffBetaDecayInXenon.C(\"test1.txt\", \"${s}.root\", \"${s}\", \"${name}\")"
done
