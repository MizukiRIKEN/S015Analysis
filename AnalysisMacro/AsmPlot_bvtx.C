{
  TString fname = "run0000_asm3.v1.03";
  Int_t iRun ={2903};//{2477};
    
  TString pRun = Form("_%d",iRun);   

  TFile *_file0 = TFile::Open("../data/run2903_asm3.v1.03.root");
  // cut
  // bevt[0]
  // bevt[1] Vertex is reconstructed
  // bevt[2] Veretx.Z in -6 to -14
  // bevt[3] Vertex.XY correlated with BDC
  // bevt[4] Track.ZY correlated with BDC

  
  TCut sn108 = "bmpid == 10";
  TCut sn132 = "bmpid == 20";
  TCut sn    = sn108;//||sn132;
  TCut bv2   = "bevt[2]";
  TCut bv4   = "bevt[4]";
  TCut ntrk  = "ntrack[1]>1";
  TCut bestcut  = sn&&bv2&&bv4;

  TCut InKyotoL = "fKyotoLy<=-51&&fKyotoLy>=-501 && fKyotoLz>=168&&fKyotoLz<=1685";
  TCut InKyotoR = "fKyotoRy<=-51&&fKyotoRy>=-501 && fKyotoRz>=168&&fKyotoRz<=1685";
  TCut InKatana = "fKatanax>=-800 && fKatanax <=500 && fKatanay <=0 && fKatana >= -400";

  TCut FindKyotoL = "dzKyotoL<40&&dzKyotoL>-40";
  TCut FindKyotoR = "dzKyotoR<40&&dzKyotoR>-40";


  gStyle->SetOptFit(1111);

  //--------------------start
  //

  auto c2 = new TCanvas("c2","c2",0,0,1000,800);
  c2->Divide(2,2);
  c2->cd(1);
  TCut gtL = sn&&InKyotoL&&"gTgTrack";
  //  TCut gtL = sn&&InKyotoL&&"gTrack&&gTgTrack";
  tpc->Draw("ty:tx>>hxy1(100,0,40,100,-20,20)",sn,"colz");
  TLatex tRun(5,15,"RUN_"+pRun);
  tRun.Draw();
  
  c2->cd(3);
  tpc->Draw("tx>>hx1(100,0,40)",sn);
  hx1->Fit("gaus","","",10.,30.);


  c2->cd(4);
  tpc->Draw("ty>>hy1(100,-20,20)",sn,"colz");
  hy1->Fit("gaus","","",-10.,10.)

}
