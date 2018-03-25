
void LoadChain(TString txtFileList, TChain* chain, TString fOutName2)
{
  std::cout << "Loading file names from "<<txtFileList << std::endl;
  ifstream fileList(txtFileList);
  string file;
  TFile *fout = new TFile::Open(fOutName.Data(), "recreate");
  TTree *header = new TTree("tree", "tree");
  int NumOfSim;
  char FileName[200];
  TBranch *br_en0 = newtree->Branch("NumOfSim",&NumOfSim,"NumOfSim/I");
  TBranch *br_en1 = newtree->Branch("FileName",FileName,"FileName[]/C");
  if (fileList.is_open()) {
    while ( getline(fileList, file) ) {
      chain->AddFile(file.c_str());
      //std::cout << "Added to TChain: "<< file << std::endl;
      header->Fill();
    }
    fileList.close();
  }
  fout->cd();
  header->Write();
  fout->Write();
  fout->Close();
}
