{
  gROOT->ProcessLine(".L Midas2Root.C+");
  for(int i=1;i<=5;i++)
    {
//       gROOT->ProcessLine("Midas2Root(\"../data\",\"../sorted/\",i,0,false)");
      gROOT->ProcessLine("Midas2Root(\"../data/\",\"../sorted/\",i,0,true)");
    }
  gROOT->ProcessLine(".q");
}
