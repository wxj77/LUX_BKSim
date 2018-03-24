# LUX_BKSim

# This code is trying to combine the background simulation works together to a location so that people can use this repo to generate the background.
# Run through these code will give you a folder bkgModelInputs(In $SIMDIR folder) as an input for background model in LUX.
# Copy the bkgModelInputs and replace the original folder in LUXLimits/.../Background/ for running limit code.

# This repo contains
# ./BackgroundSim/ from Brian T
	# it contains the code to do LUXSim energy depsition simulation
# ./BkgSimulationTools from Brian T, Wing T, Jacob C, Scott H, Curt N.
	# it contains the code to turn energy simulation to detector results.
	# the result has a cut on energy and only One single scatter in fiducial volume is selected. 
# ./WeiXX 
	# it contains the code to turn energy simulation to detector results.
	# it is a rewrite of /BkgSimulationTools 
	# it also is more flexible for sim result manipulating, it allows you to study multiple scattering in the detector, gamma X event, etc.
# ./EnvSet
	# it contain setup of the environment variables

##########################
HOW TO RUN IT
# 1) change the environment variables in EnvSet/EnvSet.sh
# 2)
	source EnvSet/EnvSet.sh
# 3)
	cd BackgroundSim
	sh MasterScript1_0_FolderGenerator.sh
	sh MasterScript1_1_10XR4LiveTime.sh
	cd ..

# 4) wait until all jobs are finished.
	
# 5)
	cd BkgSimulationTools
	sh RunAll.sh

# 6) wait until all jobs are finished.

# 7)	
	sh FilterAll.sh

# 8) wait until all jobs are finished.



 


