{
  TFile *_file0 = TFile::Open("../data/run2900_flw_v4.root");
  //  cout << " execute macro " << endl;
  gROOT->Reset();
}

void pxvsrapid()
{
  flw->Draw("fPx:0.5*log(((sqrt(1876^2 + fP^2)+fPz)/(sqrt(1876^2 + fP^2)-fPz)))>>(200,-0.01,1,200,-500,500)","flnPID==10020&&fgoodtrack","colz");
}

void pypz()
{
  auto hp1 = new TH2D("hp1","Deuteron",200,-10.,1500.,200.,-500,500);
  flw->Draw("fPy:fPz>>hp1","flnPID==10020&&fgoodtrack","colz");
}

void pxpz()
{
  auto hp1 = new TH2D("hp2","Deuteron",200,-10.,1500.,200.,-500,500);
  flw->Draw("fPx:fPz>>hp2","flnPID==10020&&fgoodtrack","colz");
}
