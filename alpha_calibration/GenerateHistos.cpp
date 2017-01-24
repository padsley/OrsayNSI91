#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <iostream>
#include <TH2.h>
#include <TChain.h>


void GenerateHistos()
{
  TFile *f0 = TFile::Open("/home/splitpole/nsi91/sorted/R41_0_old.root");

  TTree *t0 = (TTree*)f0->FindObjectAny("EGTree");
  t0->Print();

  TChain *ch = new TChain("EGTree");
  ch->Add("/home/splitpole/nsi91/sorted/R41_0_old.root");

  TH2F *ADCValues2D = new TH2F("ADCValues2D","Two-dimensional plot of ADC values from front strips",4096,0,4096,32*6,0,32*6);

  t0->Draw("adcList:adcData>>ADCValues2D","","col");
  

  TFile *fout = new TFile("UncalibratedHistos.root","RECREATE");
  ADCValues2D->Write();
  fout->Close();
}
	  
