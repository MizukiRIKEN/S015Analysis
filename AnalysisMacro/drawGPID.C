{
  //  gROOT->Macro("openASM.C");
  gROOT->Macro("openRComp.C");
  auto rChain = (TChain*)gROOT->FindObject("rChain0");
 
  gStyle -> SetOptStat(0);
  gStyle -> SetPadBottomMargin(0.10);
  gStyle -> SetPadLeftMargin(0.11);
  gStyle -> SetPadRightMargin(0.12);
  gStyle -> SetTitleFontSize(0.06);


  gROOT->Macro("load_BeamCut.C");
  gROOT->Macro("load_PIDcut.C");


  TCut GenieSigm = "sigma20 & (z < 50.536)";
  TCut GenieCut0 = "ftrackatTarget.Z()<-9.49569&&ftrackatTarget.Z()>-12.80121";
  TCut GenieCut1 = "ftrackatTarget.X()>-15&&ftrackatTarget.X()<15&&ftrackatTarget.Y()<-206.06&&ftrackatTarget.Y()>-246.06";
  TCut GenieCut2 = "rVertexID>=0&&rDist<5";
  TCut GenieCut3 = "rNDF>30";
  TCut GenieCut  = GenieSigm && GenieCut0 && GenieCut1 &&GenieCut2 && GenieCut3;

  TCut MizukiCut = "fgoodtrackf";

  TCut ccCut     = MizukiCut; //&&"ftheta<0.8";
  
  auto histPID = new TH2D("histPID", ";p/Z (MeV/c); dE/dx (ADC/mm)", 500, -500, 2500, 800, 0, 800);
  //  rChain -> Draw("fdEdx:fP/fChar>>histPID", GenieCut,"colz");
  rChain -> Project("histPID","fdEdx:fP/fChar",ccCut);  //> ver 4.0 
  cout << histPID->GetEntries() << endl;

  auto histPIDc = new TH2D("histPIDc", ";p/Z (MeV/c); dE/dx (ADC/mm)", 500, -500, 2500, 800, 0, 800);
  rChain -> Project("histPIDc","fdEdx:fP/fChar",ccCut&&"ftheta<0.8");  //> ver 4.0 
  cout << histPIDc->GetEntries() << endl;


  

  auto gcutFile1 = new TFile("gcutPID132Sn.root");
  auto gProton   = (TCutG*)gcutFile1->Get("gcutProton132Sn2");
  auto gDeuteron = (TCutG*)gcutFile1->Get("gcutDeutron132Sn");
  auto gTriton   = (TCutG*)gcutFile1->Get("gcutTriton132Sn");
  auto gPip      = (TCutG*)gcutFile1->Get("gcutPip132Sn");
  auto gPim      = (TCutG*)gcutFile1->Get("gcutPim132Sn");
  gcutFile1->Close();



  auto cc0 = new TCanvas("cc0","cc0 " ,700,500);
  histPID->Draw("colz");
  gProton->Draw("same");
  gDeuteron->Draw("same");
  gTriton->Draw("same");
  gPip->Draw("same");
  gPim->Draw("same");
  cc0->SetLogz();


  auto cc1 = new TCanvas("cc1","cc1 " ,700,500);
  histPIDc->Draw("colz");
  gProton->Draw("same");
  gDeuteron->Draw("same");
  gTriton->Draw("same");
  gPip->Draw("same");
  gPim->Draw("same");
  cc1->SetLogz();

}
 
