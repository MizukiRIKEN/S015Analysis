{
  TFile *_file0 = TFile::Open("../data/run2900_flw_v4.root");
  //  cout << " execute macro " << endl;
  gROOT->Reset();
  
  Int_t pid[6]    = {2212,     10020,       10030,  20030,  20040,      3000};
  TString spid[6] = {"Proton", "Deuteron", "Triton", "3He", "4He", "Fragments"}; 
  TString sscut[6]= {"",     "&&fP>450", "", "", "", ""};

  auto hp1 = new TH2D("hp1","init;pz;py",200,-10.,1500.,200.,-500,500);
  auto hp2 = new TH2D("hp2","init;pz;px",200,-10.,1500.,200.,-500,500);

}

void pxvsrapid()
{
  flw->Draw("fPx:0.5*log(((sqrt(1876^2 + fP^2)+fPz)/(sqrt(1876^2 + fP^2)-fPz)))>>(200,-0.01,1,200,-500,500)","flnPID==10020&&fgoodtrack","colz");
}

void pypz(Int_t ipid = 1)
{
  if (ipid > 5) return;

  gROOT->Reset();
  hp1->SetTitle(spid[ipid]);
  TString scut = Form("fgoodtrack&&flnPID==%d",pid[ipid]);
  flw->Draw("fPy:fPz>>hp1",scut+sscut[ipid],"colz");
}

void pxpz(Int_t ipid = 1)
{
  if (ipid > 5) return;

  gROOT->Reset();
  hp2->SetTitle(spid[ipid]);
  TString scut = Form("fgoodtrack&&flnPID==%d",pid[ipid]);
  flw->Draw("fPx:fPz>>hp2",scut+sscut[ipid],"colz");
}

void dedxp()
{
  flw->Draw("fdEdx:fP>>(200,0,3000,200,0,800)","fgoodtrack","colz");
  if( c1 != NULL)
    c1->SetLogz();
}
