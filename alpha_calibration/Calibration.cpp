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
  TFile *fin = TFile::Open("UncalibratedHistos.root");
  //fin->ls();

  TH2F *ADCValues2D = (TH2F*)fin->FindObjectAny("ADCValues2D");
  ADCValues2D->Draw("col");
  
  double Gains[192], Offsets[192];
  for(int i=0;i<192;i++)
    {
      Gains[i] = 0;
      Offsets[i] = 0;
    }
  
  for(int i=0;i<192;i++)
    {
      TH1D *hProj = ADCValues2D->ProjectionX("_px",i+1,i+1,"");
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

	  Offsets[i] = fit1->GetParameter(0);
	  Gains[i] = fit1->GetParameter(1);
	}
      else
	{
	  Offsets[i] = 0;
	  Gains[i] = 1;
	  cout << "fuck" << endl;
	}
    }

    ofstream output;
    output.open("W1Calibs.h");
   
   output << "double CalibPars[192][2] = { ";
   
   for(int i=0;i<191;i++)
   {
       output << "{" << Offsets[i] << "," << Gains[i] << "},\n";
   }
   output << "{" << Offsets[191] << "," << Gains[191] << "}};";
}


