//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun May  6 19:55:44 2018 by ROOT version 5.34/26
// from TTree tree/tree
// found on file: /nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           iPrimaryParNum;
   Char_t          cPrimaryParName[11];
   Double_t        fPrimaryParEnergy_keV[1];   //[iPrimaryParNum]
   Double_t        fPrimaryParTime_ns[1];   //[iPrimaryParNum]
   Double_t        fPrimaryParPosX_mm[1];   //[iPrimaryParNum]
   Double_t        fPrimaryParPosY_mm[1];   //[iPrimaryParNum]
   Double_t        fPrimaryParPosZ_mm[1];   //[iPrimaryParNum]
   Double_t        fPrimaryParDirX[1];   //[iPrimaryParNum]
   Double_t        fPrimaryParDirY[1];   //[iPrimaryParNum]
   Double_t        fPrimaryParDirZ[1];   //[iPrimaryParNum]
   Int_t           iRecordSize;
   Int_t           iEvtN;
   Int_t           iV_name;
   Char_t          cVolumeName[12];
   Int_t           iStepNum[117];   //[iRecordSize]
   Int_t           iParticleID[117];   //[iRecordSize]
   Char_t          cParticleName[362];
   Int_t           iTrackID[117];   //[iRecordSize]
   Int_t           iParentID[117];   //[iRecordSize]
   Char_t          cCreatorProcess[1470];
   Char_t          cStepProcess[528];
   Double_t        fKEnergy_keV[117];   //[iRecordSize]
   Double_t        fOptPhoWaveLength_nm[117];   //[iRecordSize]
   Double_t        fDirectionX[117];   //[iRecordSize]
   Double_t        fDirectionY[117];   //[iRecordSize]
   Double_t        fDirectionZ[117];   //[iRecordSize]
   Double_t        fEnergyDep_keV[117];   //[iRecordSize]
   Double_t        fPositionX_cm[117];   //[iRecordSize]
   Double_t        fPositionY_cm[117];   //[iRecordSize]
   Double_t        fPositionZ_cm[117];   //[iRecordSize]
   Double_t        fStepTime[117];   //[iRecordSize]
   Double_t        fTotEDep;
   Int_t           iTotOptNum;
   Int_t           iTotThermElecNum;

   // List of branches
   TBranch        *b_iPrimaryParNum;   //!
   TBranch        *b_cPrimaryParName;   //!
   TBranch        *b_fPrimaryParEnergy_keV;   //!
   TBranch        *b_fPrimaryParTime_ns;   //!
   TBranch        *b_fPrimaryParPosX_mm;   //!
   TBranch        *b_fPrimaryParPosY_mm;   //!
   TBranch        *b_fPrimaryParPosZ_mm;   //!
   TBranch        *b_fPrimaryParDirX;   //!
   TBranch        *b_fPrimaryParDirY;   //!
   TBranch        *b_fPrimaryParDirZ;   //!
   TBranch        *b_iRecordSize;   //!
   TBranch        *b_EvtN;   //!
   TBranch        *b_V_name;   //!
   TBranch        *b_cVolumeName;   //!
   TBranch        *b_iStepNum;   //!
   TBranch        *b_iParticleID;   //!
   TBranch        *b_cParticleName;   //!
   TBranch        *b_iTrackID;   //!
   TBranch        *b_iParentID;   //!
   TBranch        *b_cCreatorProcess;   //!
   TBranch        *b_cStepProcess;   //!
   TBranch        *b_fKEnergy_keV;   //!
   TBranch        *b_fOptPhoWaveLength_nm;   //!
   TBranch        *b_fDirectionX;   //!
   TBranch        *b_fDirectionY;   //!
   TBranch        *b_fDirectionZ;   //!
   TBranch        *b_fEnergyDep_keV;   //!
   TBranch        *b_fPositionX_cm;   //!
   TBranch        *b_fPositionY_cm;   //!
   TBranch        *b_fPositionZ_cm;   //!
   TBranch        *b_fStepTime;   //!
   TBranch        *b_TotEDep;   //!
   TBranch        *b_TotOptNum;   //!
   TBranch        *b_TotThermElecNum;   //!

   tree(TTree *tree=0);
   virtual ~tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

tree::~tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("iPrimaryParNum", &iPrimaryParNum, &b_iPrimaryParNum);
   fChain->SetBranchAddress("cPrimaryParName", cPrimaryParName, &b_cPrimaryParName);
   fChain->SetBranchAddress("fPrimaryParEnergy_keV", fPrimaryParEnergy_keV, &b_fPrimaryParEnergy_keV);
   fChain->SetBranchAddress("fPrimaryParTime_ns", fPrimaryParTime_ns, &b_fPrimaryParTime_ns);
   fChain->SetBranchAddress("fPrimaryParPosX_mm", fPrimaryParPosX_mm, &b_fPrimaryParPosX_mm);
   fChain->SetBranchAddress("fPrimaryParPosY_mm", fPrimaryParPosY_mm, &b_fPrimaryParPosY_mm);
   fChain->SetBranchAddress("fPrimaryParPosZ_mm", fPrimaryParPosZ_mm, &b_fPrimaryParPosZ_mm);
   fChain->SetBranchAddress("fPrimaryParDirX", fPrimaryParDirX, &b_fPrimaryParDirX);
   fChain->SetBranchAddress("fPrimaryParDirY", fPrimaryParDirY, &b_fPrimaryParDirY);
   fChain->SetBranchAddress("fPrimaryParDirZ", fPrimaryParDirZ, &b_fPrimaryParDirZ);
   fChain->SetBranchAddress("iRecordSize", &iRecordSize, &b_iRecordSize);
   fChain->SetBranchAddress("iEvtN", &iEvtN, &b_EvtN);
   fChain->SetBranchAddress("iV_name", &iV_name, &b_V_name);
   fChain->SetBranchAddress("cVolumeName", cVolumeName, &b_cVolumeName);
   fChain->SetBranchAddress("iStepNum", iStepNum, &b_iStepNum);
   fChain->SetBranchAddress("iParticleID", iParticleID, &b_iParticleID);
   fChain->SetBranchAddress("cParticleName", cParticleName, &b_cParticleName);
   fChain->SetBranchAddress("iTrackID", iTrackID, &b_iTrackID);
   fChain->SetBranchAddress("iParentID", iParentID, &b_iParentID);
   fChain->SetBranchAddress("cCreatorProcess", cCreatorProcess, &b_cCreatorProcess);
   fChain->SetBranchAddress("cStepProcess", cStepProcess, &b_cStepProcess);
   fChain->SetBranchAddress("fKEnergy_keV", fKEnergy_keV, &b_fKEnergy_keV);
   fChain->SetBranchAddress("fOptPhoWaveLength_nm", fOptPhoWaveLength_nm, &b_fOptPhoWaveLength_nm);
   fChain->SetBranchAddress("fDirectionX", fDirectionX, &b_fDirectionX);
   fChain->SetBranchAddress("fDirectionY", fDirectionY, &b_fDirectionY);
   fChain->SetBranchAddress("fDirectionZ", fDirectionZ, &b_fDirectionZ);
   fChain->SetBranchAddress("fEnergyDep_keV", fEnergyDep_keV, &b_fEnergyDep_keV);
   fChain->SetBranchAddress("fPositionX_cm", fPositionX_cm, &b_fPositionX_cm);
   fChain->SetBranchAddress("fPositionY_cm", fPositionY_cm, &b_fPositionY_cm);
   fChain->SetBranchAddress("fPositionZ_cm", fPositionZ_cm, &b_fPositionZ_cm);
   fChain->SetBranchAddress("fStepTime", fStepTime, &b_fStepTime);
   fChain->SetBranchAddress("fTotEDep", &fTotEDep, &b_TotEDep);
   fChain->SetBranchAddress("iTotOptNum", &iTotOptNum, &b_TotOptNum);
   fChain->SetBranchAddress("iTotThermElecNum", &iTotThermElecNum, &b_TotThermElecNum);
   Notify();
}

Bool_t tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tree_cxx
