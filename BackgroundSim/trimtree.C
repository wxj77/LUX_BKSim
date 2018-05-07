
// how to run
//bash: root -q -l -b "trimtree.C(\"oldFileName\", \"newFileName\")"
//ex: root -q -l -b "trimtree.C(\"/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newnew/XENON/Kr85/R4Bkg_XENON_Kr85_1000000.root\", \"test.root\")"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>


void trimtree(string infile, string outfile) {
// take output of LUXSim result trim "header" tree. trim "tree" tree.
   
   TFile *oldfile = new TFile(infile.c_str(), "read");
   TTree *oldtree = (TTree*)oldfile->Get("tree");
   oldtree->SetBranchStatus("*",1);
   oldtree->SetBranchStatus("cVolumeName",0);
   oldtree->SetBranchStatus("cParticleName",0);
   oldtree->SetBranchStatus("cCreatorProcess",0);
   oldtree->SetBranchStatus("cStepProcess",0);

   TTree *oldtree2 = (TTree*)oldfile->Get("header");
   oldtree2->SetBranchStatus("*",0);
   oldtree2->SetBranchStatus("iNumRecords",1);
   oldtree2->SetBranchStatus("svnVersion",0);
   oldtree2->SetBranchStatus("geant4Version",0);

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
