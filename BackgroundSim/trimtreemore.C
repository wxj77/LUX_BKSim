
// how to run
//bash: root -q -l -b "trimtreemore.C(\"oldFileName\", \"newFileName\")"
//ex: root -q -l -b "trimtree.C(\"/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root\", \"test.root\")"

#ifndef trimtreemore_C 
#define trimtreemore_C

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>


void trimtreemore(string infile, string outfile) {
// take output of LUXSim result trim "header" tree. trim "tree" tree.
   
   TFile *oldfile = new TFile(infile.c_str(), "read");
   TTree *oldtree = (TTree*)oldfile->Get("tree");
   oldtree->SetBranchStatus("*",0);

//general info
//   oldtree->SetBranchStatus("iEvtN",1);

//primary particle info to keep
   oldtree->SetBranchStatus("cPrimaryParName",1);
   oldtree->SetBranchStatus("fPrimaryParPosX_mm",1);
   oldtree->SetBranchStatus("fPrimaryParPosY_mm",1);
   oldtree->SetBranchStatus("fPrimaryParPosZ_mm",1);
   oldtree->SetBranchStatus("fPrimaryParEnergy_keV",1);

//other particle info to keep
   oldtree->SetBranchStatus("iRecordSize",1);
   oldtree->SetBranchStatus("iParticleID",1);
//   oldtree->SetBranchStatus("iTrackID",1);
//   oldtree->SetBranchStatus("iParentID",1);
   oldtree->SetBranchStatus("fPositionX_cm",1); 
   oldtree->SetBranchStatus("fPositionY_cm",1);
   oldtree->SetBranchStatus("fPositionZ_cm",1);
   oldtree->SetBranchStatus("fStepTime",1);
   oldtree->SetBranchStatus("fEnergyDep_keV",1);
   oldtree->SetBranchStatus("fTotEDep",1);


   TTree *oldtree2 = (TTree*)oldfile->Get("header");
   oldtree2->SetBranchStatus("*",0);
   oldtree2->SetBranchStatus("iNumRecords",1);

   //Create a new file + a clone of old tree in new file
   TFile *newfile = new TFile(outfile.c_str(),"recreate");
   TTree *newtree = oldtree->CloneTree();

 //  newtree->Print();
   newfile->Write();

   TTree *newtree2 = oldtree2->CloneTree();

 //  newtree2->Print();
   newfile->Write();

   delete oldfile;
   delete newfile;
}

#endif
