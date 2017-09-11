{
  gROOT->Macro("openASM.C");
  auto rChain = (TChain*)gROOT->FindObject("rChain");
 
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

  TCut ccCut     = MizukiCut;
  
  auto histPID = new TH2D("histPID", ";p/Z (MeV/c); dE/dx (ADC/mm)", 500, -500, 2500, 800, 0, 800);
  auto histAcc = new TH2D("histAcc", "; #Theta ; #phi"             , 100,   0.,  1.6, 100, -3.2, 3.2);



  
  auto cc0 = new TCanvas("cc0","cc0 " ,700,500);
  //  rChain -> Draw("fdEdx:fP/fChar>>histPID", GenieCut,"colz");
  rChain -> Draw("fdEdx:fP/fChar>>histPID",ccCut,"colz");  //> ver 4.0 
  //  histPID -> Draw("colz");
  histPID->GetEntries();


  auto cc1 = new TCanvas("cc1","cc1 " ,700,500);
  rChain -> Draw("frphi:fRotatedP3.Theta()>>histAcc", ccCut,"colz");
  //  histAcc -> Draw("colz");

}
 
