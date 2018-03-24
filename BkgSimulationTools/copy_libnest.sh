!#/bin/bash

git clone -b jc_NESTv2 http://github.com/luxdarkmatter/LibNEST.git LibNEST_JC
for TB in 1 2 3 4;do
#	for ZB in 1 2 3 4;do
	#	echo $TB
	#	echo $ZB
	#git clone -b jc_NESTv2 http://github.com/luxdarkmatter/LibNEST.git LibNEST_TB${TB}
	cd LibNEST_JC
	cp LUX_Detector_Settings/LUX_Run4_TB${TB}.hh ./detector.hh
	make clean
	make
	make install
	cd ..
	done
done
