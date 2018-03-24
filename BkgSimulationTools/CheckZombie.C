#include "TFile.h"
#include <iostream>

void CheckZombie(TString filename){
  TFile* file = TFile::Open(filename.Data());
  std::cout<<"Processing file. "<<std::endl;
  if (!file->IsOpen()){
    std::cout<<"File did not Open: "<<filename.Data()<<std::endl;
  }
  if (file->IsZombie()){
    std::cout<<"Zombie file: "<<filename.Data()<<std::endl;
  file->Close();
  }
}
