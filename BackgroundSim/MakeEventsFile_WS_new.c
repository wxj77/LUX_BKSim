#include "TString.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include <iostream>
#include <cstdio>

void MakeEventsFile_WS(TString fName, TString fOutName) {

// This root script pares down the output of the energy deposition only sims so that the optical sims don't have to run as many unneccessary simulations.
  printf("start make event.");
  TChain *chain = new TChain("tree");
  char name[1000];
  sprintf(name, "%s", fName.Data());
//  printf("success 1.");
  chain->Add(name);


  FILE* outFile;
  char outName[1000];

  int iEvtN;
  int iParticleID[1000000];
  int iRecordSize;
  double fTotEDep;
  double fEnergyDep_keV[1000000];
  double fPositionX_cm[1000000];
  double fPositionY_cm[1000000];
  double fPositionZ_cm[1000000];

  chain->SetBranchAddress("iEvtN", &iEvtN);
  chain->SetBranchAddress("iParticleID", &iParticleID);
  chain->SetBranchAddress("iRecordSize", &iRecordSize);
  chain->SetBranchAddress("fTotEDep", &fTotEDep);
  chain->SetBranchAddress("fEnergyDep_keV", &fEnergyDep_keV);
  chain->SetBranchAddress("fPositionX_cm", &fPositionX_cm);
  chain->SetBranchAddress("fPositionY_cm", &fPositionY_cm);
  chain->SetBranchAddress("fPositionZ_cm", &fPositionZ_cm);

  int numEntries = chain->GetEntries();
// We take look at all events that deposit some amount of energy between 0 and 100 keVee.(old)
// now we include all events.

//  int numLowE = chain->GetEntries("fTotEDep < 100. && fTotEDep > 0.");
   int numLowE = chain->GetEntries();
// We include at most 700 events per output file
if (numLowE < 700){
//  char outName[100];
  sprintf(outName, "%s_%d.data", fOutName.Data(), numLowE);

//  FILE* outFile;
  outFile = fopen(outName, "w");

// We loop over all these entries and keep those with the enrgy below 100 keV.
  for(int i = 0; i < numEntries; i++) {
    chain->GetEntry(i);
    if(1){
      int ZCutFlag = 1;
    /*if(fTotEDep < 100.) {

	double Zmin = 999999999;
	double Zmax = -999999999;
	double deltaZ = 0;
	int ZCutFlag = 0;
// Loop through the record of each entry to make sure that at least some energy is deposited in the main xenon volume between the cathode and the gate.
	for(int j = 0; j < iRecordSize; j++) {
		if (fPositionZ_cm[j] < Zmin) {Zmin=fPositionZ_cm[j];}
		if (fPositionZ_cm[j] > Zmax) {Zmax=fPositionZ_cm[j];}
		if (fPositionZ_cm[j] > 5.6 && fPositionZ_cm[j] < 53.92 ){int ZCutFlag = 1;}
	}
	deltaZ = fabs(Zmax-Zmin);
// Writes to file the energy deposition information.*/ 
      for(int j = 0; j < iRecordSize; j++) {
        if(iParticleID[j] > 1e8 && ZCutFlag == 1) {
          fprintf(outFile, "%d   %d   %f   %f   %f   %f\n", iEvtN, iParticleID[j], fEnergyDep_keV[j], fPositionX_cm[j], fPositionY_cm[j], fPositionZ_cm[j]);
        }
else if((iParticleID[j] == 22 || iParticleID[j] == 11 || iParticleID[j] == -11)&& ZCutFlag == 1) {
          fprintf(outFile, "%d   %d   %f   %f   %f   %f\n", iEvtN, 11, fEnergyDep_keV[j], fPositionX_cm[j], fPositionY_cm[j], fPositionZ_cm[j]);
        }
      }
    }
  }

  fclose(outFile);
} else {

//If we have more than 700 events, it writes to multiple files.
  int numLowETracker = 0;
  int numLowETrackerAll = 0;
  int numLowEDivTracker = 0;
  int lastDiv = 0;

  for(int i = 0; i < numEntries; i++) {
    chain->GetEntry(i);

    if(lastDiv == 1 && numLowETracker == 0 ){
   // if(lastDiv == 1 && numLowETracker == 0 && fTotEDep < 100. && fTotEDep > 0.){
  //  	char outName[100];
	int finalLowENum = numLowE - numLowETrackerAll;
    	sprintf(outName, "%s_%d_%d.data", fOutName.Data(), numLowEDivTracker, finalLowENum);
	printf("%s\n", outName);
    	//FILE* outFile;
    	outFile = fopen(outName, "w");
    }
    else if(numLowETracker == 0){ 
    //else if(numLowETracker == 0 && fTotEDep < 100. && fTotEDep > 0.){
   // 	char outName[100];
    	sprintf(outName, "%s_%d_%d.data", fOutName.Data(), numLowEDivTracker, 700);
    	//FILE* outFile;
    	outFile = fopen(outName, "w");
    }
    if(1) {
    //if(fTotEDep < 100. && fTotEDep > 0.) {
      numLowETracker++;
      numLowETrackerAll++;
      int ZCutFlag = 1;
//    printf("%d, %d\n",numLowETracker, numLowETrackerAll);
/*	double Zmin = 999999999;
	double Zmax = -999999999;
	double deltaZ = 0;
	int ZCutFlag = 0;
	for(int j = 0; j < iRecordSize; j++) {
		if (fPositionZ_cm[j] < Zmin) {Zmin=fPositionZ_cm[j];}
		if (fPositionZ_cm[j] > Zmax) {Zmax=fPositionZ_cm[j];}
		if (fPositionZ_cm[j] > 5.6 && fPositionZ_cm[j] < 53.92 ){int ZCutFlag = 1;}
	}
	deltaZ = fabs(Zmax-Zmin);
*/
      for(int j = 0; j < iRecordSize; j++) {
        if(iParticleID[j] > 1e8  && ZCutFlag == 1) {
          fprintf(outFile, "%d   %d   %f   %f   %f   %f\n", iEvtN, iParticleID[j], fEnergyDep_keV[j], fPositionX_cm[j], fPositionY_cm[j], fPositionZ_cm[j]);
        } else if((iParticleID[j] == 22 || iParticleID[j] == 11 || iParticleID[j] == -11)  && ZCutFlag == 1) {
          fprintf(outFile, "%d   %d   %f   %f   %f   %f\n", iEvtN, 11, fEnergyDep_keV[j], fPositionX_cm[j], fPositionY_cm[j], fPositionZ_cm[j]);
        }
      }
    }
    if(numLowETracker == 700){
      numLowETracker = 0;
      numLowEDivTracker++;
      fclose(outFile);
      if( (numLowE - numLowETrackerAll) < 700){
        lastDiv = 1;
      }
    }
  }
  if (lastDiv == 1){fclose(outFile);}
}
  printf("success make event.");
  printf("outfile name: %s", outName);
}
