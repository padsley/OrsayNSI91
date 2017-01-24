#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <iostream>
#include <TH2.h>
#include <TChain.h>


void GenerateHistos()
{
  TFile *f0 = TFile::Open("/home/padsley/data/19Fpp/sorted/R225_0.root");
  TFile *f1 = TFile::Open("/home/padsley/data/19Fpp/sorted/R225_1.root");
  TFile *f2 = TFile::Open("/home/padsley/data/19Fpp/sorted/R225_2.root");

  TTree *t0 = (TTree*)f1->FindObjectAny("EGTree");
  TTree *t1 = (TTree*)f1->FindObjectAny("EGTree");
  TTree *t2 = (TTree*)f1->FindObjectAny("EGTree");

  TChain *ch = new TChain("EGTree");
  ch->Add("/home/padsley/data/19Fpp/sorted/R225_0.root");
  ch->Add("/home/padsley/data/19Fpp/sorted/R225_1.root");
  ch->Add("/home/padsley/data/19Fpp/sorted/R225_2.root");

  TH1F **ADCFronts = new TH1F*[16*6];
  TH1F **ADCBacks  = new TH1F*[16*6];

  TH2F *ADCFronts2D = new TH2F("ADCFronts2D","Two-dimensional plot of ADC values from front strips",4096,0,4096,16*6,0,16*6);
  TH2F *ADCBacks2D = new TH2F("ADCBacks2D","Two-dimensional plot of ADC values from back strips",4096,0,4096,16*6,0,16*6);
  
  for(int i=0;i<16*6;i++)
    {
      char buffer[256], buffer2[256];
      sprintf(buffer,"FrontStrip%d",i);
      sprintf(buffer2,"ADC spectrum for Front Strip %d",i);
      //cout << buffer << endl;
      ADCFronts[i] = new TH1F(buffer,buffer2,4096,0,4096);
      sprintf(buffer,"BackStrip%d",i);
      sprintf(buffer2,"ADC spectrum for Back Strip %d",i);
      ADCBacks[i] = new TH1F(buffer,buffer2,4096,0,4096);
    }


  TFile *fout = new TFile("UncalibratedHistos.root","RECREATE");

  char buffer[256], buffer2[256];
  sprintf(buffer,"16*(fW1_FrontE_DetectorNbr-1)+fW1_FrontE_StripNbr:fW1_FrontE_Energy>>ADCFronts2D(4096,0,4096,96,0,96)");
  ch->Draw(buffer,"","col");

  sprintf(buffer,"16*(fW1_BackE_DetectorNbr-1)+fW1_BackE_StripNbr:fW1_BackE_Energy>>ADCBacks2D(4096,0,4096,96,0,96)");
  ch->Draw(buffer,"","col");
  
  for(int i=0;i<6;i++)
    {
      for(int j=0;j<16;j++)
	{
	  //cout << "i: " << i << "\t j: " << j << endl;
	  //char buffer[256], buffer2[256];
	  //sprintf(buffer,"fW1_FrontE_Energy>>ADCFronts[%d]",16*i+j);
	  //sprintf(buffer2,"fW1_FrontE_DetectorNbr==%d && fW1_FrontE_StripNbr==%d",i,j);
	  //t1->Draw(buffer,buffer2,"");
	  //ADCFronts[16*i+j]->Write();

	  //sprintf(buffer,"fW1_BackE_Energy>>ADCBacks[%d]",16*i+j);
	  //sprintf(buffer2,"fW1_BackE_DetectorNbr==%d && fW1_BackE_StripNbr==%d",i,j);
	  //t1->Draw(buffer,buffer2,"");
	  //ADCBacks[16*i+j]->Write();
	}
    }

  ADCFronts2D->Write();
  ADCBacks2D->Write();
  
  fout->Close();
}
	  
