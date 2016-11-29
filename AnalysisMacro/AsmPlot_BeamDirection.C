{
  gROOT->Macro("AsmPlot_TCut.C");

  //  TString fname = "run0000_ts.v1.03.6s4sb4";
  //  TString fname = "run0000_ts.v1.03.boff";
  TString fname = "run0000_asm.v1.03";

  //Int_t iRun[] = {2333};
  //  Int_t iRun[] = {2331,2332,2333,2334};
  //  Int_t iRun[] = {2181,2182,2183,2184,2186,2189};
  Int_t iRun[] = {2903};

  auto fChain = new TChain("tpc");
  
  for(Int_t i = 0; i < sizeof(iRun)/sizeof(Int_t); i++){
    TString tmfile = fname;
    TString sRun = Form("%d",iRun[i]);
    tmfile.ReplaceAll("0000",sRun);
    
    fChain->Add("../data/"+tmfile+".root");
  }

  gStyle->SetOptStat(0);
  
  TString twait ;
  auto c1 = new TCanvas("c1","c1",40,50,800,600);

  //gStyle->SetOptFit(1);

  //--------------------start
  c1->Divide(2,2);

  c1->cd(1);
  tpc->Draw("tx>>htn6(200,-100,100)",sn&&"bevt[2]&&bevt[4]&&gTrack");
  htn6->GetXaxis()->SetTitle("Tracks extrapolated onto the target in X[mm]");
  c1->Update();

  c1->cd(2);
  tpc->Draw("asin(tx/770)>>htn1(200,-0.1,0.1)",sn&&"bevt[2]&&bevt[4]&&gTrack");
  htn1->GetXaxis()->SetTitle("Tracks extrapolated onto the target in X[mm]");
  c1->Update();


  
  c1->cd(3);
  tpc->Draw("STTrack.fBeamVy:ty>>htv12(200,-50,50,200,-260,-200)",sn&&"bevt[2]&&bevt[4]&&gTrack","colz");
  // htv12->GetXaxis()->SetTitle("Beam Position [mm]");			
  // htv12->GetYaxis()->SetTitle("Tracks extrapolated onto the target [mm]");
  c1->Update();




  return 0;

  tpc->Draw("STTrack.fBeamVy>>htv2(200,-260,-200)",sn&&"bevt[2]&&bevt[4]&&gTrack","colz");
  htv2->GetXaxis()->SetTitle("Tracks extrapolated onto the target in Y[mm]");
  c1->Update();


  return 0;



  tpc->Draw("STTrack.fBeamVx:tx>>htv1(200,-10,50,200,-80,80)",sn&&"bevt[2]&&bevt[4]","colz");
  // htv1->GetXaxis()->SetTitle("Beam Position [mm]");			
  // htv1->GetYaxis()->SetTitle("Tracks extrapolated onto the target [mm]");
  c1->Update();

  tpc->Draw("STTrack.fBeamVx:tx>>htv1(200,-10,50,200,-80,80)",sn&&"bevt[2]&&bevt[4]&&gTrack","colz");
  c1->Update();

 
  tpc->Draw("STTrack.fBeamVx:tx>>htn5(200,-30,60,200,-100,100",sn&&"bevt[2]&&bevt[4]&&gTrack","colz");
  c1->Update();

}
