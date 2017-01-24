#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TSpectrum.h>
#include <TGraph.h>
#include <TF1.h>
#include <fstream>
#include <iostream>

void Calibration()
{
  TFile *fin = TFile::Open("UncalibratedHistos_manual.root");
  //fin->ls();

  TH2F *ADCFronts2D = (TH2F*)fin->FindObjectAny("ADCFronts2D");
  ADCFronts2D->Draw("col");

  TH2F *ADCBacks2D = (TH2F*)fin->FindObjectAny("ADCBacks2D");
  ADCBacks2D->Draw("col");
  
  double FrontGains[96], FrontOffsets[96], BackGains[96], BackOffsets[96];
  for(int i=0;i<96;i++)
    {
      FrontGains[i] = 0;
      FrontOffsets[i] = 0;
      BackGains[i] = 0;
      BackOffsets[i] = 0;
    }
  
  for(int i=0;i<96;i++)
    {
      TH1D *hProj = ADCFronts2D->ProjectionX("_px",i+1,i+1,"");
      hProj->Draw();

      hProj->GetXaxis()->SetRangeUser(500,2500);

      TSpectrum *sp = new TSpectrum();
      sp->Search(hProj,2,"",0.35);

      if(sp->GetNPeaks()==3)
	{
	  float *chan = sp->GetPositionX();
	  for(int n=0;n<2;n++)
	    {
	      for(int nn=n+1;nn<3;nn++)
		{
		  if(chan[nn]<chan[n])
		    {
		      float chan_tmp = chan[n];
		      chan[n] = chan[nn];
		      chan[nn] = chan_tmp;
		    }
		}
	    }
	  TGraph *g = new TGraph();
	  g->SetName("fitting");
	  double Ealpha[3] = {5.155,5.486,5.805};
	  for(int n=0;n<3;n++)
	    {
	      g->SetPoint(n,chan[n],Ealpha[n]);
	    }
	  TF1 *fit1 = new TF1("fit1","[0]+[1]*x",0,5000);
	  g->Fit("fit1","Q");
	  g->Draw("AL*");
	  fit1->Draw("same");

	  FrontOffsets[i] = fit1->GetParameter(0);
	  FrontGains[i] = fit1->GetParameter(1);
	}
      else
	{
	  FrontOffsets[i] = 0;
	  FrontGains[i] = 1;
	  cout << "front fuck" << endl;
	}
    }

   for(int i=0;i<96;i++)
    {
      TH1D *hProj = ADCBacks2D->ProjectionX("_px",i+1,i+1,"");
      hProj->Draw();

      hProj->GetXaxis()->SetRangeUser(500,2500);

      TSpectrum *sp = new TSpectrum();
      sp->Search(hProj,2,"",0.35);

      if(sp->GetNPeaks()==3)
	{
	  float *chan = sp->GetPositionX();
	  for(int n=0;n<2;n++)
	    {
	      for(int nn=n+1;nn<3;nn++)
		{
		  if(chan[nn]<chan[n])
		    {
		      float chan_tmp = chan[n];
		      chan[n] = chan[nn];
		      chan[nn] = chan_tmp;
		    }
		}
	    }
	  TGraph *g = new TGraph();
	  g->SetName("fitting");
	  double Ealpha[3] = {5.155,5.486,5.805};
	  for(int n=0;n<3;n++)
	    {
	      g->SetPoint(n,chan[n],Ealpha[n]);
	    }
	  TF1 *fit1 = new TF1("fit1","[0]+[1]*x",0,5000);
	  g->Fit("fit1","Q");
	  g->Draw("AL*");
	  fit1->Draw("same");

	  BackOffsets[i] = fit1->GetParameter(0);
	  BackGains[i] = fit1->GetParameter(1);
	}
      else
	{
	  BackOffsets[i] = 0;
	  BackGains[i] = 1;
	  cout << "back fuck" << endl;
	}
    }

   ofstream output;
   output.open("/home/padsley/codes/nptool/Projects/SPcoincW1_19Fpp/calibs/W1Calibs.txt");

   for(int i=0;i<96;i++)
     {
       cout << "W1_D" << (i-i%16)/16+1 << "_FRONT" << i%16 << "_E\t" << FrontOffsets[i] << "\t" << FrontGains[i] << endl;
       output << "W1_D" << (i-i%16)/16+1 << "_FRONT" << i%16 << "_E\t" << FrontOffsets[i] << "\t" << FrontGains[i] << endl;
     }

   for(int i=0;i<96;i++)
     {
       cout << "W1_D" << (i-i%16)/16+1 << "_BACK" << i%16 << "_E\t" << BackOffsets[i] << "\t" << BackGains[i] << endl;
       output << "W1_D" << (i-i%16)/16+1 << "_BACK" << i%16 << "_E\t" << BackOffsets[i] << "\t" << BackGains[i] << endl;
     }

     output.close();
     output.open("/home/padsley/data/19Fpp/W1Calibs.h");
   
   output << "double CalibPars[192][2] = { ";
   
   double Gains[192], Offsets[192];
   
   for(int i=0;i<6;i++)
   {
       for(int j=0;j<16;j++)
       {
           cout << "32*(i-1)+j = " << 32*(i)+j << endl;
            Offsets[32*(i)+j] = FrontOffsets[16*i+j];
            Gains[32*(i)+j] = FrontGains[16*i+j];
            
            Offsets[32*(i)+j+16] = BackOffsets[16*i+j];
            Gains[32*(i)+j+16] = BackGains[16*i+j];
       }
   }
   
   for(int i=0;i<191;i++)
   {
       output << "{" << Offsets[i] << "," << Gains[i] << "},\n";
   }
   output << "{" << Offsets[191] << "," << Gains[191] << "}};";
}


