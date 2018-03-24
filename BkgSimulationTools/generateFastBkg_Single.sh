#! /bin/bash
source /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh
/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 1
/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 2
/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 3
/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 4
