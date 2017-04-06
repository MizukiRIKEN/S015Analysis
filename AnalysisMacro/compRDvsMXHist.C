//void compRScsMX()
{
  TString srun = gSystem -> Getenv("RUN");
  TString sVer = gSystem -> Getenv("VER");

  if( srun=="" || sVer=="") {
    cout << "Plase type " << endl;
    cout << "$ RUN=#### VER=#.# root compRDvsMXplot.C" << endl;
    exit(0);
  }


  TString fname[2];

  //--------
  fname[0] = "../data/run"+srun+"_rdflwv"+sVer+".root";
  fname[1] = "../data/run"+srun+"_mxflwv"+sVer+".root";
  //--------

  cout << fname[1] << endl;
  cout << fname[0] << endl;


  TFile *aFile[2];
  TTree *aTree[2];


  aFile[0] = TFile::Open(fname[0]);
  aTree[0] = (TTree*)gROOT->FindObject("rflw");
  if( !aTree[0] ) return;
  aTree[0]->ls();

  aFile[1] = TFile::Open(fname[1]);
  aTree[1] = (TTree*)gROOT->FindObject("mflw");
  if( !aTree[1] ) return;
  aTree[1]->ls();


  auto aLeg = new TLegend(0.1,0.8,0.3,0.9);
  TString fleg[2];
  fleg[0] = " Real ";
  fleg[1] = " Mixed";

  Int_t iColor[2] = {2, 4};

  TCanvas *cc[2];

  TH1D    *hp[2];
  TH1D    *hk[2];

  for(Int_t i = 0; i < 2; i++) {
    hp[i] = new TH1D();
  }


  aFile[0]->cd();
}


void plotHist2(TString param = "hRPpxsr_b")
{
  cc[0] = new TCanvas("cc0", "comparison 1",700, 600);
  cc[1] = new TCanvas("cc1", "comparison 2",700, 600);
  
  aFile[0]->cd();
  cc[0]->cd();

  TH2D *h2_0 = (TH2D*)gROOT->FindObject(param);
  h2_0 -> SetName("REAL");
  h2_0 -> Draw("colz");

  aFile[1]->cd();
  cc[1]->cd();

  TH2D *h2_1 = (TH2D*)gROOT->FindObject(param);
  h2_1 -> SetName("MIXed");
  h2_1 -> Draw("colz");

  auto cc3 = new TCanvas("cc3","Flow analysis ",700,600);
  cc3->SetLogz();
  TH2D *dvd_02 = new TH2D((*h2_0)/(*h2_1));
  dvd_02->SetName("dvd_02");
  dvd_02->SetTitle("Real/Mix");
  dvd_02->Draw("colz");
  
  
}

