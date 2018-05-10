//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun May  6 19:55:36 2018 by ROOT version 5.34/26
// from TTree header/header
// found on file: /nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root
//////////////////////////////////////////////////////////

#ifndef header_h
#define header_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class header {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           iNumRecords;
   Char_t          productionTime[31];
   Char_t          geant4Version[22];
   Char_t          svnVersion[48];
   Char_t          uName[10];
   Char_t          InputCommands[1037];
   Char_t          diffs[24778246];
   Char_t          DetCompo[91591];

   // List of branches
   TBranch        *b_iNumRecords;   //!
   TBranch        *b_sPT;   //!
   TBranch        *b_sG4V;   //!
   TBranch        *b_sSV;   //!
   TBranch        *b_sUN;   //!
   TBranch        *b_sIC;   //!
   TBranch        *b_sDI;   //!
   TBranch        *b_sDC;   //!

   header(TTree *tree=0);
   virtual ~header();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef header_cxx
header::header(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root");
      }
      f->GetObject("header",tree);

   }
   Init(tree);
}

header::~header()
{
//   if (!fChain) return;
//   delete fChain->GetCurrentFile();
}

Int_t header::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t header::LoadTree(Long64_t entry)
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

void header::Init(TTree *tree)
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

   fChain->SetBranchAddress("iNumRecords", &iNumRecords, &b_iNumRecords);
   fChain->SetBranchAddress("productionTime", productionTime, &b_sPT);
   fChain->SetBranchAddress("geant4Version", geant4Version, &b_sG4V);
   fChain->SetBranchAddress("svnVersion", svnVersion, &b_sSV);
   fChain->SetBranchAddress("uName", uName, &b_sUN);
   fChain->SetBranchAddress("InputCommands", InputCommands, &b_sIC);
   fChain->SetBranchAddress("diffs", diffs, &b_sDI);
   fChain->SetBranchAddress("DetCompo", DetCompo, &b_sDC);
   Notify();
}

Bool_t header::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void header::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t header::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef header_cxx
