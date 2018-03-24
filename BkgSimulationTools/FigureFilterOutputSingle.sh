#! /bin/bash

# Need to run in BkgSimulationTools folder.
#
# Change /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg to your path to generateFastBkg
# Change /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh to where your root lib is.
# Change /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh to where your libnest lib is.

#cd /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools

# Change SIMDIR to the folder that has all R4BG files.
# Run "./RunAll.sh"


source /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh

SIMDIR=$1
SourceType=$2
TimeBin=$3

inputfilename=${SIMDIR}/bkgModelInputs/golden${SourceType}_tb${TimeBin}_fidCut.root
outputfilename=${SIMDIR}/bkgModelInputs/golden_filterOutput_${SourceType}_tb${TimeBin}_fidCut.png
echo $outputfilename
root -l -b -q "filterOutput.C(\"${inputfilename}\",\"$outputfilename\")"
outputfilename=${SIMDIR}/bkgModelInputs/golden_filterOutputGoldenPassCut_${SourceType}_tb${TimeBin}_fidCut.png
root -l -b -q "filterOutputGoldenPassCut.C(\"${inputfilename}\",\"$outputfilename\")"
outputfilename=${SIMDIR}/bkgModelInputs/golden_filterOutputGoldenAndRealSpacePassCut_${SourceType}_tb${TimeBin}_fidCut.png
root -l -b -q "filterOutputGoldenAndRealSpacePassCut.C(\"${inputfilename}\",\"$outputfilename\")"



