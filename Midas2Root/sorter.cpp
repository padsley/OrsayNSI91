{
  gROOT->ProcessLine(".L Midas2Root.C+");
	  for(int i=95;i<=104;i++)
    {
//	gROOT->ProcessLine("Midas2Root(\"../raw\",\"../sorted/\",i,0,false)");
	gROOT->ProcessLine("Midas2Root(\"../raw/\",\"../sorted/\",i,0,true)");
    }
  gROOT->ProcessLine(".q");
}
