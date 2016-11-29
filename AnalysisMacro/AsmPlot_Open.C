{
  //TString fname = "run0000_asm.v1.03";
  TString fname = "run0000_asm2.v1.03";

  //Int_t iRun[] = {2333};
  Int_t iRun[] = {2903};
  //  Int_t iRun[] = {3023};

  auto fChain = new TChain("tpc");
  
  for(Int_t i = 0; i < sizeof(iRun)/sizeof(Int_t); i++){
    TString tmfile = fname;
    TString sRun = Form("%d",iRun[i]);
    tmfile.ReplaceAll("0000",sRun);
    
    fChain->Add("../data/"+tmfile+".root");
  }

  // cut
  // bevt[0]
  // bevt[1] Vertex is reconstructed
  // bevt[2] Veretx.Z in -6 to -14
  // bevt[3] Vertex.XY correlated with BDC
  // bevt[4] Track.ZY correlated with BDC


  gStyle->SetOptStat(0);
  
  TCut sn108 = "bmpid == 10";
  TCut sn132 = "bmpid == 20";
  TCut sn    = sn108||sn132;
  TCut bv2   = "bevt[2]";
  TCut bv4   = "bevt[4]";
  TCut ntrk  = "ntrack[1]>1";
  TCut bestcut  = sn&&bv2&&bv4;

  TCut InKyotoL = "fKyotoLy<=-51&&fKyotoLy>=-501 && fKyotoLz>=168&&fKyotoLz<=1685";
  TCut InKyotoR = "fKyotoRy<=-51&&fKyotoRy>=-501 && fKyotoRz>=168&&fKyotoRz<=1685";

  TCut InKatana = "fKatanax>=-800 && fKatanax <=500 && fKatanay <=0 && fKatana >= -400";
}

