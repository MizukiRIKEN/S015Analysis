//void compRScsMX()
{
  TString arun = gSystem -> Getenv("RUN");
  TString sVer = gSystem -> Getenv("VER");

  if( arun=="" || sVer=="") {
    cout << "Plase type " << endl;
    cout << "$ RUN=#### VER=#.# root compRDvsMXplot.C" << endl;
    exit(0);
  }


  
  Int_t nrun = (arun.Length()-1)/5;
  cout << arun << "-> nrun " << nrun << endl;;

  vector<Int_t> lrun;
  Int_t ist = 1;
  while(ist < arun.Length() - 4) {
    TString prun = arun(ist,4);
    lrun.push_back( atoi(prun) );
    ist+=5;
  }

  for(Int_t i = 0; i < nrun; i++)
    cout << " lrun = " << lrun.at(i) << endl;



  TChain *rChain[2];
  rChain[0] = new TChain("rflw");
  rChain[1] = new TChain("mflw");
  

  TString fname[2];

  for(Int_t i = 0; i < (Int_t)lrun.size(); i++){
    fname[0] = Form("../data/run%d_rdflwv"+sVer+".root",lrun.at(i));
    fname[1] = Form("../data/run%d_mxflwv"+sVer+".root",lrun.at(i));

    cout << fname[1] << endl;
    cout << fname[0] << endl;
    
    rChain[0]->Add(fname[0]);
    rChain[1]->Add(fname[1]);
    
  }



  // TFile *aFile[2];
  // TTree *aTree[2];


  // aFile[0] = TFile::Open(fname[0]);
  // aTree[0] = (TTree*)gROOT->FindObject("rflw");
  // if( !aTree[0] ) return;
  // aTree[0]->ls();

  // aFile[1] = TFile::Open(fname[1]);
  // aTree[1] = (TTree*)gROOT->FindObject("mflw");
  // if( !aTree[1] ) return;
  // aTree[1]->ls();


  auto aLeg = new TLegend(0.1,0.8,0.3,0.9);
  TString fleg[2];
  fleg[0] = " Real ";
  fleg[1] = " Mixed";

  Int_t iColor[2] = {2, 4};

  TCanvas *cc[2];
  TH1D    *hp[2];
  TH1D    *hk[2];
  TH1D   *dvd_01;

  for(Int_t i = 0; i < 2; i++) {
    hp[i] = new TH1D();
  }


  //  rChain[0]->cd();
}



void plot1D(TString param = "deltphi", TString cut="",  TString opt="")
{
  cc[0] = new TCanvas("cc0", "comparison 1",700, 600);
  cc[1] = new TCanvas("cc1", "comparison 2",700, 600);


  Int_t    nbin;
  Double_t range[2];

  SetRange(param, &nbin, range);

  cout << "nbin " << nbin << " range[0]: " << range[0] << " range[1]: "<<range[1] << endl;


  if( cut == "" && (param == "mtrack" || param == "unitP.Phi()") )
    cut = "mtrack>0";

  gROOT->cd();
  for(Int_t i = 0; i < 2; i++) {
    hp[i]->SetName(Form("hp_%d",i));
    hp[i]->SetTitle(param+fleg[i]);
    hp[i]->SetBins(nbin, range[0], range[1]);


    hp[i]->Print();

  }
    

  cc[0]->Divide(1,2);

  for(Int_t i = 0; i < 2; i++){
    cc[0]->cd(i+1);
    //    rChain[i]->cd();
    TString hdraw = Form(param+">>hp_%d",i);
    gROOT->cd();
    hp[i]->SetDirectory(gROOT);
    rChain[i]->Draw(hdraw,cut);


  }



  
  aLeg->Clear();
  aLeg->SetHeader(fname[0]+" : "+param);

  for(Int_t i = 0; i < 2; i++){
    Double_t nrmf = hp[i]->Integral();
    hk[i] = (TH1D*)hp[i]->Clone();
    hk[i]->SetName(Form("hk_%d",i));

    hk[i] -> Scale(1./nrmf);
    aLeg  -> AddEntry(hk[i], fleg[i], "l");

    hp[i]->SetLineColor(iColor[i]);
    hk[i]->SetLineColor(iColor[i]);  
  }


  gROOT->ls();

  cc[1]->Update();
  cc[1]->Divide(1,2);

  cc[1]->cd(1);
  Double_t hrng = std::max( hk[1] -> GetMaximum(), hk[0] -> GetMaximum() );



  hk[1] -> SetMaximum(hrng*1.1);
  hk[1] -> Draw();
  hk[0] -> Draw("same");
  aLeg  -> Draw();

  cc[1]->cd(2);


  TH1D *hk0 = (TH1D*)gROOT->FindObject("hp_0");
  TH1D *hk1 = (TH1D*)gROOT->FindObject("hp_1");
  //  TH1D *dvd_01 = new TH1D((*hk0)/(*hk1));
  //  TH1D *dvd_01 = new TH1D((*hk0)-(*hk1));
  dvd_01 = new TH1D((*hk0)-(*hk1));
  dvd_01->SetName("dvd_01");

  Double_t nrmf = hk0->Integral();
  dvd_01->Scale(1./nrmf);

  dvd_01 -> Print();

  dvd_01 -> Draw(opt);
  cc[1]  -> Update();


}


void plot2D(TString paramy = "deltphi", TString paramx = "rapid" , TString cut="")
{
  cc[0] = new TCanvas("cc0", "comparison 1",700, 600);
  cc[1] = new TCanvas("cc1", "comparison 2",700, 600);


  Int_t    nbin[2] = {200, 200};
  Double_t range[2][2] = {{-500,500},{-500,500}};

  SetRange(paramx, &nbin[0], range[0]);
  cout<<paramx  << " nbin " << nbin[0] << " range[0]:" << range[0][0] << " range[1]:"<<range[0][1] << endl;

  
  SetRange(paramy, &nbin[1], range[1]);
  cout <<paramy << " nbin " << nbin[1] << " range[1]:" << range[1][0] << " range[1]:"<<range[1][1] << endl;


  TH2D *pxy[4];
  pxy[0] = new TH2D("hpxy_0",paramx+":"+paramy+"REAL",nbin[0],range[0][0],range[0][1],nbin[1],range[1][0],range[1][1]);
  pxy[1] = new TH2D("hpxy_1",paramx+":"+paramy+"Mix ",nbin[0],range[0][0],range[0][1],nbin[1],range[1][0],range[1][1]);

  for(Int_t i = 0; i < 2; i++){
    cc[i]->cd();
    //    rChain[i]->cd();
    
    pxy[i]->SetDirectory(gROOT);
    gROOT->cd();
    rChain[i] -> Draw(Form(paramy+":"+paramx+">>hpxy_%d",i),cut,"colz");
  }

  auto cc3 = new TCanvas("cc3","Flow analysis ",700,600);
  TH2D *dvd_02 = new TH2D((*hpxy_0)/(*hpxy_1));
  dvd_02->SetName("dvd_02");
  dvd_02->SetTitle("Real/Mix");
  dvd_02->Draw("colz");
  cc3->SetLogz();
}

void dphibt(TString cut="", TString opt="")
{
  if(cut !="") cut = "&&"+cut;
  //  plot1D("TVector2::Phi_mpi_pi(unitP_b.Phi()-unitP_t.Phi())","mtrack_b>0&&mtrack_t>0"+cut);
  plot1D("TVector2::Phi_0_2pi(unitP_b.Phi()-unitP_t.Phi())","mtrack_b>0&&mtrack_t>0"+cut,opt);

  auto ccp = new TCanvas("ccp", "comparison 2",700, 600);
  ccp->cd();
  dvd_01 -> Draw(opt);
  
}


void AMDcomp()
{
  plot1D("unitP.Phi()");

  TFile *afile2 = new TFile("/cache/scr/spirit/mizuki/AMD/AMDhard.g4.root");

  
  TH1I *h1 = (TH1I*)gROOT->FindObject("h1");
  Double_t nrmf = h1->Integral();
  h1 -> Scale(1./nrmf);
  aLeg -> AddEntry(h1, "AMD","1");

  cc1->cd(1);
  h1->SetLineColor(6);
  h1->Draw("same");

}

void SetRange(TString param, Int_t *nbin, Double_t *range)
{


  if(param == "pz") {
    *nbin = 200; range[0] = 0.;   range[1] = 1500.;
  }
  else if(param == "px" || param == "py" ) {
    *nbin = 200; range[0] = -500.;range[1] = 500;
    cout << " param " << param << endl;
  }
  else if(param == "mtrack" || param == "mtrack_t" || param== "mtrack_b") {
    *nbin = 50;  range[0] = 0.;   range[1] = 50;
    cout << " param " << param << endl;
  }
  else if(param == "rapid") {
    *nbin = 100; range[0] = 0.;   range[1] = 1.;
  }
  else if(param(0,19) == "TVector2::Phi_0_2pi") {
    *nbin = 60;  range[0] = 0; range[1] = 6.4;
  }
  else {
    *nbin = 60;  range[0] = -3.2; range[1] = 3.2;
    cout << " param " << param << endl;
  }
  
}


void Multiplicity(TString stitle = "")
{
  auto ccp = new TCanvas("ccp", "comparison 2",700, 600);

  hk[0]->GetXaxis()->SetTitle(stitle);
  hk[0]->Draw("");
  hk[1]->Draw("same");
  aLeg->SetHeader("");
  aLeg->Draw();
  
  //  cc1->SaveAs("FlwRUN2900_RealMix_Multiplicity.png");
}
