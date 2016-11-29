{

  Int_t iRun = 2903;
  TFile *_file0 = TFile::Open(Form("../data/run%d_asm2.v1.03.root",iRun));
  TString pRun = Form("%d",iRun);

  // cut
  // bevt[0]
  // bevt[1] Vertex is reconstructed
  // bevt[2] Veretx.Z in -6 to -14
  // bevt[3] Vertex.XY correlated with BDC
  // bevt[4] Track.ZY correlated with BDC


  
  TCut sn108 = "bmpid == 10";
  TCut sn132 = "bmpid == 20";
  TCut sn    = sn108||sn132;
  TCut bv2   = "bevt[2]";
  TCut bv4   = "bevt[4]";
  TCut ntrk  = "ntrack[1]>1";
  TCut bestcut  = sn&&bv2&&bv4;

  TCut InKyotoL = "fKyotoLy<=-51&&fKyotoLy>=-501 && fKyotoLz>=168&&fKyotoLz<=1685";
  TCut InKyotoR = "fKyotoRy<=-51&&fKyotoRy>=-501 && fKyotoRz>=168&&fKyotoRz<=1685";

  TCut FindKyotoL = "dzKyotoL<40&&dzKyotoL>-40";
  TCut FindKyotoR = "dzKyotoR<40&&dzKyotoR>-40";

  TCut InKatana = "fKatanax>=-800 && fKatanax <=500 && fKatanay <=0 && fKatanay >= -400";

  gStyle->SetOptFit(1111);

  //--------------------start
  //
  Int_t nEntry = tpc->GetEntries();

  // TLatex tRun(0,-450,"RUN_"+pRun);
  // tRun.Draw();


  auto c2 = new TCanvas("c2","c2",0,0,800,600);
  
  TCut kyotoMult = "kyotom==4";
  tpc->Draw("ntrack[0]>>hn1(100,0,70)",sn&&kyotoMult);
  Int_t nevt = hn1->GetEntries();

  c2->Clear();
  c2->Divide(2,2);

  c2->cd(1);
  TCut gtL = sn&&InKyotoL&&kyotoMult&&"gTrack&&gTgTrack";
  tpc->Draw("fKyotoLz>>hsl(31,168,1735)",gtL&&"fKyotoLz>0");

  c2->cd(2);
  TH1F *hsl1 = new TH1F((Float_t)1./nevt*(*hsl));
  hsl1->Draw();

  c2->cd(3);
  TCut gtR = sn&&InKyotoR&&kyotoMult&&"gTrack&&gTgTrack";
  tpc->Draw("fKyotoRz>>hsr(31,168,1735)",gtR&&"fKyotoRz>0");

  c2->cd(4);
  TH1F *hsr1 = new TH1F((Float_t)1./nevt*(*hsr));
  hsr1->Draw();
  

  cout << "Run_" << pRun << " Entries " << nevt << endl;
  kyotoMult.Print();
}
