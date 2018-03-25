//test run
//ls ../XENON_Kr85/R4Bkg_XENON_Kr85_100000* >txt.txt 
//root -b -q -l "./WeiLookForGammaX.C+(\"./txt.txt\", \"test1.root\", \"test2.root\", \"test3.root\",\"test4.root\")"


//c lib
#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <cstdlib>

//root lib
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TFile.h"
#include "TStopwatch.h"

//custom header
#include "../BkgSimulationTools/FieldMap.h"
#include "../BkgSimulationTools/modelBinDef.h"



using namespace std;

//functions in this file.

const Int_t kMaxTrack=1000;
struct MySimEvent;
struct MyClusterEvent;
void load_chain(TString txtFileList, TChain* chain);
void SaveChain(TString fOutName, TChain* chain);

void load_tree(TTree* chain, MySimEvent & e);
void set_newtree(TTree* events, MyClusterEvent & e);
void load_newtree(TTree* events, MyClusterEvent & e);
void eventLoop(TChain* events, TTree* newtree);
void Cluster(MySimEvent & e, MyClusterEvent & cl);
void Map(MyClusterEvent & cl, FieldMap m);
void eventLoop2(TTree* newtree, TTree* newtree2, int timeBin);

//int WeiLookForGammaX(TString fName, TString fOutName1, TString fOutName2);
int WeiLookForGammaX(TString fName, TString fOutName1, TString fOutName2, TString fOutName3, TString fOutName4);


//------------------------------------------------------------------------------
// Simple struct to hold the variables that we extract from ROOT files.
// To add a variable:
// 1. Add it to this struct
// 2. Set the appropriate branch address in load_tree()
enum Type { S1=1, S2=2, SPE=3, SE=4, OTHER=5 };

struct MySimEvent
{ 
  int iEvtN;
  int iParticleID[100000];
  int iRecordSize;
  double fTotEDep;
  double fEnergyDep_keV[100000];
  double fPositionX_cm[100000];
  double fPositionY_cm[100000];
  double fPositionZ_cm[100000];
  double fPrimaryParPosX_mm;
  double fPrimaryParPosY_mm; 
  double fPrimaryParPosZ_mm;
};


struct MyClusterEvent
{
  Float_t fTotEDep;
  Int_t iClusterNum;
  Int_t iParticleID[kMaxTrack];//iParticleID[iClusterNum];
  Float_t fEnergyDep_keV[kMaxTrack];//fEnergyDep_keV[iClusterNum];
  Float_t fPositionX_cm[kMaxTrack];//fPositionX_cm[iClusterNum];
  Float_t fPositionY_cm[kMaxTrack];//fPositionY_cm[iClusterNum];
  Float_t fPositionZ_cm[kMaxTrack];//fPositionZ_cm[iClusterNum];
  Float_t S2X_cm[kMaxTrack];//fPositionX_cm[iClusterNum];
  Float_t S2Y_cm[kMaxTrack];//fPositionY_cm[iClusterNum];
  Float_t Drift_us[kMaxTrack];//fPositionZ_cm[iClusterNum];
  Float_t fPrimaryParPosX_mm;
  Float_t fPrimaryParPosY_mm; 
  Float_t fPrimaryParPosZ_mm;

  Int_t ZCutFlag[kMaxTrack];//flag for record cluster location, 0 for total, 1 for above cathode, below gate, 2 for below cathode.
  Float_t Energy0, Energy1,Energy2; //0 for total, 1 for above cathode, below gate, 2 for below cathode.
  Float_t posX; 
  Float_t posY; 
  Float_t posZ; 
  Float_t deltaZ;
  
};

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
}//load_chain(TString txtFileList, TChain* chain)

void SaveChain(TString fOutName, TChain* chain) {
//save chain to a root file.
  printf("start copytree to merging big chain.");
  printf("%s", fOutName.Data());
  TFile *fout = TFile::Open(fOutName.Data(),"RECREATE");
  chain->CloneTree(-1,"fast");
  fout->cd();
  fout->Write();
  fout->Close();
  delete fout;
}//SaveChain(TString fOutName, TChain* chain)

//------------------------------------------------------------------------------
// Set the branch address for the variables in MyEvent.
// To increase speed, we disable all branches by default, and turn on only the
// ones we want to use.
void load_tree(TTree* events, MySimEvent & e){
  events->SetBranchStatus("*",0); //disable all

  events->SetBranchStatus("iEvtN", 1);
  events->SetBranchAddress("iEvtN", &e.iEvtN);

  events->SetBranchStatus("iParticleID", 1);
  events->SetBranchAddress("iParticleID", &e.iParticleID);

  events->SetBranchStatus("iRecordSize", 1);
  events->SetBranchAddress("iRecordSize", &e.iRecordSize);

  events->SetBranchStatus("fTotEDep", 1);
  events->SetBranchAddress("fTotEDep", &e.fTotEDep);

  events->SetBranchStatus("fEnergyDep_keV", 1);
  events->SetBranchAddress("fEnergyDep_keV", &e.fEnergyDep_keV);

  events->SetBranchStatus("fPositionX_cm", 1);
  events->SetBranchAddress("fPositionX_cm", &e.fPositionX_cm);

  events->SetBranchStatus("fPositionY_cm", 1);
  events->SetBranchAddress("fPositionY_cm", &e.fPositionY_cm);

  events->SetBranchStatus("fPositionZ_cm", 1);
  events->SetBranchAddress("fPositionZ_cm", &e.fPositionZ_cm);

  events->SetBranchStatus("fPrimaryParPosX_mm", 1);
  events->SetBranchAddress("fPrimaryParPosX_mm", &e.fPrimaryParPosX_mm);

  events->SetBranchStatus("fPrimaryParPosY_mm", 1);
  events->SetBranchAddress("fPrimaryParPosY_mm", &e.fPrimaryParPosY_mm);

  events->SetBranchStatus("fPrimaryParPosZ_mm", 1);
  events->SetBranchAddress("fPrimaryParPosZ_mm", &e.fPrimaryParPosZ_mm);  
}//load_tree()

void set_newtree(TTree* events, MyClusterEvent & e){

  events->Branch("iClusterNum", &e.iClusterNum, "iClusterNum/I");

  events->Branch("fTotEDep", &e.fTotEDep, "fTotEDep/F");

  events->Branch("iParticleID", &e.iParticleID, "iParticleID[iClusterNum]/I");

  events->Branch("fEnergyDep_keV", &e.fEnergyDep_keV, "fEnergyDep_keV[iClusterNum]/F");

  events->Branch("fPositionX_cm", &e.fPositionX_cm, "fPositionX_cm[iClusterNum]/F");

  events->Branch("fPositionY_cm", &e.fPositionY_cm, "fPositionY_cm[iClusterNum]/F");

  events->Branch("fPositionZ_cm", &e.fPositionZ_cm, "fPositionZ_cm[iClusterNum]/F");

  events->Branch("S2X_cm", &e.S2X_cm, "S2X_cm[iClusterNum]/F");

  events->Branch("S2Y_cm", &e.S2Y_cm, "S2Y_cm[iClusterNum]/F");

  events->Branch("Drift_us", &e.Drift_us, "Drift_us[iClusterNum]/F");

  events->Branch("fPrimaryParPosX_mm", &e.fPrimaryParPosX_mm, "fPrimaryParPosX_mm/F");

  events->Branch("fPrimaryParPosY_mm", &e.fPrimaryParPosY_mm, "fPrimaryParPosY_mm/F");

  events->Branch("fPrimaryParPosZ_mm", &e.fPrimaryParPosZ_mm, "fPrimaryParPosZ_mm/F");  

  events->Branch("posX", &e.posX, "posX/F");
  events->Branch("posY", &e.posY, "posY/F");
  events->Branch("posZ", &e.posZ, "posZ/F");

  events->Branch("Energy0",&e.Energy0,"Energy0/F");
  events->Branch("Energy1",&e.Energy1,"Energy1/F");
  events->Branch("Energy2",&e.Energy2,"Energy2/F");

  events->Branch("deltaZ", &e.deltaZ, "deltaZ/F");
}//set_newtree()


void load_newtree(TTree* events, MyClusterEvent & e){
  events->SetBranchStatus("*",0); //disable all

  events->SetBranchStatus("iClusterNum", 1);
  events->SetBranchAddress("iClusterNum", &e.iClusterNum);

  events->SetBranchStatus("fTotEDep", 1);
  events->SetBranchAddress("fTotEDep", &e.fTotEDep);

  events->SetBranchStatus("iParticleID", 1);
  events->SetBranchAddress("iParticleID", &e.iParticleID);

  events->SetBranchStatus("fEnergyDep_keV", 1);
  events->SetBranchAddress("fEnergyDep_keV", &e.fEnergyDep_keV);

  events->SetBranchStatus("fPositionX_cm", 1);
  events->SetBranchAddress("fPositionX_cm", &e.fPositionX_cm);

  events->SetBranchStatus("fPositionY_cm", 1);
  events->SetBranchAddress("fPositionY_cm", &e.fPositionY_cm);

  events->SetBranchStatus("fPositionZ_cm", 1);
  events->SetBranchAddress("fPositionZ_cm", &e.fPositionZ_cm);

  events->SetBranchStatus("S2X_cm", 1);
  events->SetBranchAddress("S2X_cm", &e.S2X_cm);

  events->SetBranchStatus("S2Y_cm", 1);
  events->SetBranchAddress("S2Y_cm", &e.S2Y_cm);

  events->SetBranchStatus("Drift_us", 1);
  events->SetBranchAddress("Drift_us", &e.Drift_us);

  events->SetBranchStatus("fPrimaryParPosX_mm", 1);
  events->SetBranchAddress("fPrimaryParPosX_mm", &e.fPrimaryParPosX_mm);

  events->SetBranchStatus("fPrimaryParPosY_mm", 1);
  events->SetBranchAddress("fPrimaryParPosY_mm", &e.fPrimaryParPosY_mm);

  events->SetBranchStatus("fPrimaryParPosZ_mm", 1);
  events->SetBranchAddress("fPrimaryParPosZ_mm", &e.fPrimaryParPosZ_mm);  

  events->SetBranchStatus("posX", 1);
  events->SetBranchAddress("posX", &e.posX);

  events->SetBranchStatus("posY", 1);
  events->SetBranchAddress("posY", &e.posY);

  events->SetBranchStatus("posZ", 1);
  events->SetBranchAddress("posZ", &e.posZ);

  events->SetBranchStatus("Energy0", 1);
  events->SetBranchAddress("Energy0",&e.Energy0);

  events->SetBranchStatus("Energy1", 1);
  events->SetBranchAddress("Energy1",&e.Energy1);

  events->SetBranchStatus("Energy2", 1);
  events->SetBranchAddress("Energy2",&e.Energy2);

  events->SetBranchStatus("deltaZ", 1);
  events->SetBranchAddress("deltaZ", &e.deltaZ);
}//load_newtree()


void Cluster(MySimEvent & e, MyClusterEvent & cl){
  double* x = e.fPositionX_cm;
  double* y = e.fPositionY_cm;
  double* z = e.fPositionZ_cm;
  double* energy = e.fEnergyDep_keV; 
  int* iParticleID = e.iParticleID;
  int iRecordSize = e.iRecordSize;
   
  Float_t* x2 = cl.fPositionX_cm;
  Float_t* y2 = cl.fPositionY_cm;
  Float_t* z2 = cl.fPositionZ_cm;
  Float_t* energy2 = cl.fEnergyDep_keV;
  Int_t* iParticleID2 = cl.iParticleID;
  Int_t iRecordSize2=0;

  int counts=0;
  int k = 0;
// cluster electron(11) positron (11), gamma(22) events.
  k = iRecordSize2;
  counts=0;
        for(int j = 0; j < e.iRecordSize; j++) {
            if(j==0){
              x2[k] = x[j];
              y2[k] = y[j];
              z2[k] = z[j];
              energy2[k] = 0.;
              iRecordSize2 = k + 1;
              iParticleID2[k] = 11;
            }
            for(int m = 0; m < k + 1; m++){
		if (( (iParticleID[j]==11) || (iParticleID[j]==-11) || (iParticleID[j]==22))){
			counts++;
		        if( ((x[j]-x2[m]) * (x[j]-x2[m]) + (y[j]-y2[m]) * (y[j]-y2[m]) + (z[j]-z2[m]) * (z[j]-z2[m]) < (0.04) * (0.04))  ){
		            energy2[m] = energy2[m] + energy[j];
		            break;
		        }
		        else if (m == k){
		            k++;
		            x2[k] = x[j];
		            y2[k] = y[j];
		            z2[k] = z[j];
		            energy2[k] = energy[j];
		            iRecordSize2 = k + 1;
              		    iParticleID2[k] = 11;
		            break;
		        }
		}
            }
        }  
  if (counts==0){
    iRecordSize2-=1;
  }
//cluster nuclear events.
  k = iRecordSize2;
  counts=0;
        for(int j = 0; j < e.iRecordSize; j++) {
            if(j==0){
              x2[k] = x[j];
              y2[k] = y[j];
              z2[k] = z[j];
              energy2[k] = 0.;
              iRecordSize2 = k + 1;
              iParticleID2[k] = 120000000;
            }
            for(int m = 0; m < k + 1; m++){
		if (( (iParticleID[j]>1.e8))){
			counts++;
		        if( ((x[j]-x2[m]) * (x[j]-x2[m]) + (y[j]-y2[m]) * (y[j]-y2[m]) + (z[j]-z2[m]) * (z[j]-z2[m]) < (0.04) * (0.04))  ){
		            energy2[m] = energy2[m] + energy[j];
		            break;
		        }
		        else if (m == k){
				std::cout<<m<<", "<< j<<", "<< x[j]<<std::endl;
				printf("%f", float(x[j]));
		            k++;
		            x2[k] = x[j];
		            y2[k] = y[j];
		            z2[k] = z[j];
		            energy2[k] = energy[j];
		            iRecordSize2 = k + 1;
              		    iParticleID2[k] = 120000000;
		            break;
		        }
		}
            }
        }
  if (counts==0){
    iRecordSize2-=1;
    k = iRecordSize2;
  }

//update cluster number.
  cl.iClusterNum=iRecordSize2;
}

//map cluster to s2x, s2y, drift time.
void Map(MyClusterEvent & cl, FieldMap m) {
   for (int j=0; j< cl.iClusterNum; j++){
            m.SetCurrentPoint(cl.fPositionX_cm[j],cl.fPositionX_cm[j],cl.fPositionX_cm[j]);
            double params[4];
            m.InterpolateTrilinear(params); // params = {S2x, S2y, dt, E}
            cl.S2X_cm[j]    = params[0]; // cm
            cl.S2Y_cm[j]     = params[1]; // cm
            cl.Drift_us[j]    = params[2]; // us
   }
}//Map(float dat_x, float dat_y, float dat_z, int timeBin = 1)



//------------------------------------------------------------------------------
// Main event loop is contained here.
void eventLoop(TChain* events, TTree* newtree){
  //Write privious results to a new tree.
  // Set the branch addresses.
  
  MySimEvent e;
  MyClusterEvent cl;
  load_tree(events, e);
  set_newtree(newtree, cl);


  //-------------------------//
  //     MAIN EVENT LOOP     //
  //-------------------------//


// We loop over all these entries and keep those with the enrgy below 100 keV.
  int numEntries = events->GetEntries();
  for(int i = 0; i < numEntries; i++) {
    events->GetEntry(i);
  //    if(fTotEDep < 100.) 
    if(1) {
      //clear value from previous loop.
      double ZMin = 999999999.;
      double ZMax = -999999999.;
      double ZMinAboveCat = 999999999.;
      double ZMaxAboveCat = -999999999.;
 //     double deltaZ = 0.;
      cl.posX=0.; cl.posY=0.; cl.posZ=0.;
      cl.Energy0 = 0.; cl.Energy1 = 0.; cl.Energy2= 0.;
      cl.fPrimaryParPosX_mm = e.fPrimaryParPosX_mm; cl.fPrimaryParPosY_mm = e.fPrimaryParPosY_mm;cl.fPrimaryParPosZ_mm = e.fPrimaryParPosZ_mm;
      cl.fTotEDep=e.fTotEDep;

      //1 for above cathode, below gate, 2 for below cathode.
      // Loop through the record of each entry to make sure that at least some energy is deposited in the main xenon volume between the cathode and the gate.
      Cluster(e, cl);
      for(int j = 0; j < e.iRecordSize; j++) { 
        if((e.iParticleID[j] == 22 || e.iParticleID[j] == 11 || e.iParticleID[j] == -11)){
          if (e.fPositionZ_cm[j] < ZMin) {ZMin=e.fPositionZ_cm[j];}
          if (e.fPositionZ_cm[j] > ZMax) {ZMax=e.fPositionZ_cm[j];}
          if (e.fPositionZ_cm[j] > 5.6 && e.fPositionZ_cm[j] < 53.92 ){
            cl.ZCutFlag[j] = 1; cl.Energy0 += e.fEnergyDep_keV[j]; cl.Energy1 += e.fEnergyDep_keV[j]; 
            if (e.fPositionZ_cm[j] < ZMinAboveCat) {ZMinAboveCat=e.fPositionZ_cm[j];}
            if (e.fPositionZ_cm[j] > ZMaxAboveCat) {ZMaxAboveCat=e.fPositionZ_cm[j];}
          }
          else if (e.fPositionZ_cm[j] > 0 && e.fPositionZ_cm[j] <= 5.6 ){
            cl.ZCutFlag[j] = 2; cl.Energy0 += e.fEnergyDep_keV[j]; cl.Energy2 += e.fEnergyDep_keV[j];}  
        }
      }
      cl.deltaZ = ZMaxAboveCat-ZMinAboveCat;
      //Single scatter above cathode.
      //if ( ((ZMaxAboveCat-ZMinAboveCat)<0.65) && ((ZMaxAboveCat-ZMinAboveCat)>=0.))
      if ((e.iRecordSize)>0){
        cl.posZ = ZMinAboveCat;
        for (int j = 0; j < e.iRecordSize; j++) {
          if((e.iParticleID[j] == 22 || e.iParticleID[j] == 11 || e.iParticleID[j] == -11)){
            cl.posX +=  e.fPositionX_cm[j];
            cl.posY +=  e.fPositionY_cm[j];
          }
        }
        cl.posX = cl.posX/double(e.iRecordSize);
        cl.posY = cl.posY/double(e.iRecordSize);
        newtree->Fill();
      }
      // not single scatter above cathode.
    }//if(1)
  }//for(int i = 0; i < numEntries; i++)

}//eventLoop(TChain* events, TTree* newtree)


void eventLoop2(TTree* newtree, TTree* newtree2, int timeBin){
//a newtree2 to recorde mapped and nested results. 
  int ii=0;
//define field map;
  TString fieldMapName = timeBinDef(timeBin);
  TString TimeBinStr=TString::Format("%d",timeBin);
  char* env_p = std::getenv("BKSimTools");
  fieldMapName = TString::Format("%s/BkgSimulationTools/Run4FieldMaps/%s", env_p,fieldMapName.Data());
  // Declare a FieldMap object that reads in the desired Lucie field map
  FieldMap m(fieldMapName.Data());

  MyClusterEvent cl1;
//  MyClusterEvent cl2;

  load_newtree(newtree, cl1);
  set_newtree(newtree2, cl1);

  int numEntries = newtree->GetEntries();
  for(int i = 0; i < numEntries; i++) {
    newtree->GetEntry(i);
    Map(cl1, m); //time bin mapping
    newtree2->Fill();
  }
}
/*
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
}*/


//////////////////////////////////////////
////////////////////////////////////Main Function//////////////////////////////////////////

int WeiLookForGammaX(TString fName, TString fOutName1, TString fOutName2, TString fOutName3, TString fOutName4) {

// This root script pares down the output of the energy deposition only sims so that the optical sims don't have to run as many unneccessary simulations.
// fName: File name for a txt that contains the directory of the root results for this simulation.
// fOutName1: A File contain a TChain that has the simulation results.
// fOutName2: A File that contain the counts and the file name for the simulation results. 
// fOutName3: A File that contain the clustered, mapped, nested result for the simulation results.
// fOutName4: A File that contain the clustered, mapped, nested result for the simulation results, for different timeBin.

  TStopwatch* clock = new TStopwatch();
  clock->Start();

  printf("start merging big tree.");
  TString txtFileList = fName.Data();//TString txtFileList = "txtFileList.txt";

// write header infos, has the counts for all simulations.
  TChain *chain2 = new TChain("header");
  load_chain(txtFileList, chain2);
  SaveChain(fOutName2, chain2);
  delete chain2;
  std::cout << "Done write header!"<<" "<<clock->RealTime()<<" s."<<std::endl;

// write tree info, has all simulation results/
  TChain *chain = new TChain("tree");
  load_chain(txtFileList, chain);
  SaveChain(fOutName1, chain);
  std::cout << "Done write tree!"<<" "<<clock->RealTime()<<" s."<<std::endl;

// write newtree info, has output for cluster events.
  TFile* outfile = new TFile(fOutName3.Data(), "recreate");
  TTree* newtree = new TTree("tree","LUXSim Cluster result.");
  eventLoop( chain, newtree);
  outfile->cd();
  newtree->Write();
  outfile->Close();
  delete chain; 
  std::cout << "Done write newtree!"<<" "<<clock->RealTime()<<" s."<<std::endl;

  TFile* outfile2 = new TFile(fOutName4.Data(), "recreate");
  TTree* newtree2 = new TTree("tree","LUXSim Cluster result.");
  eventLoop2( newtree, newtree2, 1);
  outfile2->cd();
  newtree2->Write();
  outfile2->Close();
  std::cout << "Done write newtree2!"<<" "<<clock->RealTime()<<" s."<<std::endl;

  std::cout << "Done!"<<" "<<clock->RealTime()<<" s."<<std::endl;
  
  delete newtree;
//  delete clock;
  return 1;




  

}//WeiLookForGammaX(TString fName, TString fOutName1, TString fOutName2)

