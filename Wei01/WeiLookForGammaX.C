//c lib
#include <iostream>
#include <fstream>

//root lib
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TFile.h"

//custom header
//#include "FieldMap.h"
//#include "modelBinDef.h"

using namespace std;

void load_chain(TString txtFileList, TChain* chain)
{
  std::cout << "Loading file names from "<<txtFileList << std::endl;
  
  ifstream fileList(txtFileList);
  string file;
  if (fileList.is_open()) {
    while ( getline(fileList, file) ) {
      chain->AddFile(file.c_str());
      //std::cout << "Added to TChain: "<< file << std::endl;
    }
    fileList.close();
  }
}

int WeiLookForGammaX(TString fName, TString fOutName) {

// This root script pares down the output of the energy deposition only sims so that the optical sims don't have to run as many unneccessary simulations.
  printf("start make event.");
  TChain *chain = new TChain("tree");
	TString txtFileList = fName.Data();
///TString txtFileList = "txtFileList.txt";
  load_chain(txtFileList, chain);
  char name[100];
//  sprintf(name, "%s", fName.Data());
//  chain->Add(name);


  int iEvtN;
  int iParticleID[100000];
  int iRecordSize;
  double fTotEDep;
  double fEnergyDep_keV[100000];
  double fPositionX_cm[100000];
  double fPositionY_cm[100000];
  double fPositionZ_cm[100000];

  chain->SetBranchAddress("iEvtN", &iEvtN);
  chain->SetBranchAddress("iParticleID", &iParticleID);
  chain->SetBranchAddress("iRecordSize", &iRecordSize);
  chain->SetBranchAddress("fTotEDep", &fTotEDep);
  chain->SetBranchAddress("fEnergyDep_keV", &fEnergyDep_keV);
  chain->SetBranchAddress("fPositionX_cm", &fPositionX_cm);
  chain->SetBranchAddress("fPositionY_cm", &fPositionY_cm);
  chain->SetBranchAddress("fPositionZ_cm", &fPositionZ_cm);

double fPrimaryParPosX_mm, fPrimaryParPosY_mm , fPrimaryParPosZ_mm;
  chain->SetBranchAddress("fPrimaryParPosX_mm", &fPrimaryParPosX_mm);
  chain->SetBranchAddress("fPrimaryParPosY_mm", &fPrimaryParPosY_mm);
  chain->SetBranchAddress("fPrimaryParPosZ_mm", &fPrimaryParPosZ_mm);  



//new root variables for energy above and below cathode.
  TTree* newtree = new TTree("newtree", "newtree");
 	float Energy0, Energy1,Energy2; //0 for total, 1 for above cathode, below gate, 2 for below cathode.
	TBranch *br_en0 = newtree->Branch("Energy0",&Energy0,"Energy0/F");
	TBranch *br_en1 = newtree->Branch("Energy1",&Energy1,"Energy1/F");
	TBranch *br_en2 = newtree->Branch("Energy2",&Energy2,"Energy2/F");
	TBranch *br_PrimZ_mm = newtree->Branch("fPrimaryParPosZ_mm",&fPrimaryParPosZ_mm,"fPrimaryParPosZ_mm/D");
	float posX; TBranch *br_PosX = newtree->Branch("posX", &posX, "posX/F");
	float posY; TBranch *br_PosY = newtree->Branch("posY", &posY, "posY/F");
	float posZ; TBranch *br_PosZ = newtree->Branch("posZ", &posZ, "posZ/F");


  int numEntries = chain->GetEntries();
// We take look at all events that deposit some amount of energy between 0 and 100 keVee.
  int numLowE = chain->GetEntries("fTotEDep < 100. && fTotEDep > 0.");
	TCut BotGrid_cut = "fPrimaryParPosZ_mm < 40. && fPrimaryParPosZ_mm > 0.";
	TCut CatGrid_cut = "fPrimaryParPosZ_mm < 80. && fPrimaryParPosZ_mm >= 40.";
	TCut GatGrid_cut = "fPrimaryParPosZ_mm < 570. && fPrimaryParPosZ_mm >= 543.";
	TCut AnoGrid_cut = "fPrimaryParPosZ_mm < 543. && fPrimaryParPosZ_mm > 520.";
	TCut TopGrid_cut = "fPrimaryParPosZ_mm < 600. && fPrimaryParPosZ_mm >= 570.";
	int BotGrid = chain->GetEntries(BotGrid_cut);
	int CatGrid = chain->GetEntries(CatGrid_cut);
	int GatGrid = chain->GetEntries(GatGrid_cut);
	int AnoGrid = chain->GetEntries(AnoGrid_cut);
	int TopGrid = chain->GetEntries(TopGrid_cut);
	cout<<BotGrid+CatGrid+GatGrid+AnoGrid+TopGrid<<endl;

// We loop over all these entries and keep those with the enrgy below 100 keV.
	for(int i = 0; i < numEntries; i++) {
		chain->GetEntry(i);
	//    if(fTotEDep < 100.) 
		if(1) {
			double ZMin = 999999999.;
			double ZMax = -999999999.;
			double ZMinAboveCat = 999999999.;
			double ZMaxAboveCat = -999999999.;
			double deltaZ = 0.;
			posX=0.; posY=0.; posZ=0.;
			int ZCutFlag[100000];//1 for above cathode, below gate, 2 for below cathode.
		// Loop through the record of each entry to make sure that at least some energy is deposited in the main xenon volume between the cathode and the gate.
			Energy0 = 0.; Energy1 = 0.; Energy2= 0.;
			for(int j = 0; j < iRecordSize; j++) { 
				if((iParticleID[j] == 22 || iParticleID[j] == 11 || iParticleID[j] == -11)){
					if (fPositionZ_cm[j] < ZMin) {ZMin=fPositionZ_cm[j];}
					if (fPositionZ_cm[j] > ZMax) {ZMax=fPositionZ_cm[j];}
					if (fPositionZ_cm[j] > 5.6 && fPositionZ_cm[j] < 53.92 ){
						ZCutFlag[j] = 1;Energy0 += fEnergyDep_keV[j]; Energy1 += fEnergyDep_keV[j]; 
						if (fPositionZ_cm[j] < ZMinAboveCat) {ZMinAboveCat=fPositionZ_cm[j];}
						if (fPositionZ_cm[j] > ZMaxAboveCat) {ZMaxAboveCat=fPositionZ_cm[j];}
					}
					else if (fPositionZ_cm[j] > 0 && fPositionZ_cm[j] <= 5.6 ){ZCutFlag[j] = 2;Energy0 += fEnergyDep_keV[j]; Energy2 += fEnergyDep_keV[j];}	
				}
			}
			//Single scatter above cathode.
			if ( ((ZMaxAboveCat-ZMinAboveCat)<0.65) && ((ZMaxAboveCat-ZMinAboveCat)>=0.)){
				posZ = ZMinAboveCat;
				for (int j = 0; j < iRecordSize; j++) {
					if((iParticleID[j] == 22 || iParticleID[j] == 11 || iParticleID[j] == -11)){
						posX +=  fPositionX_cm[j];
						posY +=  fPositionY_cm[j];
					}
				}
				posX = posX/double(iRecordSize);
				posY = posY/double(iRecordSize);
				newtree->Fill();
			}
			// not single scatter above cathode.
		}
	}


//newtree->Draw("log10(Energy1*10*18.):Energy2*10.*.1+Energy1*10.*.1", "Energy1>0 &&Energy2 >0 && Energy1<100" && CatGrid_cut,"col");

//newtree->Draw("log10(Energy1*10*18.):Energy2*10.*.1+Energy1*10.*.1", "Energy1>0 &&Energy2 >0 && Energy1<100" && CatGrid_cut && "posZ>0","col");

//newtree->Draw("posZ: sqrt(posX*posX+posY*posY)", "Energy1>0 &&Energy2 >0 && Energy1<100" && CatGrid_cut,"col");

 	TFile* newfile = TFile::Open(fOutName.Data(),"recreate");
	newfile->cd();
	newtree->Write();
	newfile->Close();
	return 1;
}
