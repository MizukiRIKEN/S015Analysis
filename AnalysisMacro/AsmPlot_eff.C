{

  Int_t iRun = 2903;
  //TFile *_file0 = TFile::Open(Form("../data/run%d_asm3.v1.03.root",iRun));
  TFile *_file0 = TFile::Open(Form("../data/run%d_kym3.v1.03.root",iRun));
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

  //  TCut InKyotoL = "fKyotoLy<=-51&&fKyotoLy>=-501 && fKyotoLz>=168&&fKyotoLz<=1685";
  //  TCut InKyotoR = "fKyotoRy<=-51&&fKyotoRy>=-501 && fKyotoRz>=168&&fKyotoRz<=1685";
  TCut InKyotoL = "KytTrack == 1";
  TCut InKyotoR = "KytTrack == 2";

  TCut FindKyotoL = "dzKyotoL<40&&dzKyotoL>-40";
  TCut FindKyotoR = "dzKyotoR<40&&dzKyotoR>-40";

  TCut InKatana = "fKatanax>=-800 && fKatanax <=500 && fKatanay <=0 && fKatanay >= -400";


  gStyle->SetOptFit(1111);

  //--------------------start
  //

  auto c2 = new TCanvas("c2","c2",0,0,1000,800);
  c2->Divide(2,2);
  c2->cd(1);
  TCut gtL = sn&&InKyotoL&&"gTgTrack";
  //  TCut gtL = sn&&InKyotoL&&"gTrack&&gTgTrack";
  tpc->Draw("fKyotoLy:fKyotoLz>>hsl2(200,0,2000,200,-700,100",gtL&&"fKyotoLz>0","colz");

  
  c2->cd(2);
  tpc->Draw("dzKyotoL>>hsl1(100,-250,250",gtL&&"fKyotoLz>0");
  tpc->Draw("dzKyotoL>>hsl1c(100,-250,250",gtL&&FindKyotoL&&"fKyotoLz>0","same");
  
  cout << "L Found in Kyoto / Track " << hsl1c->GetEntries() << " / " << hsl1->GetEntries() 
       << " = " << hsl1c->GetEntries()/hsl1->GetEntries() << endl;



  c2->cd(3);

  TCut gtR = sn&&InKyotoR&&"gTgTrack";
  //  TCut gtR = sn&&InKyotoR&&"gTrack&&gTgTrack";
  tpc->Draw("fKyotoRy:fKyotoRz>>hsr2(200,0,2000,200,-700,100",gtR&&"fKyotoRz>0","colz");

  c2->cd(4);
  tpc->Draw("dzKyotoR>>hsr1(100,-250,250",gtR&&"fKyotoRz>0");
  tpc->Draw("dzKyotoR>>hsr1c(100,-250,250",gtR&&FindKyotoR&&"fKyotoRz>0","same");

  cout << "R Found in Kyoto / Track " << hsr1c->GetEntries() << " / " << hsr1->GetEntries()
       << " = " << hsr1c->GetEntries()/hsr1->GetEntries() << endl;




  auto c1 = new TCanvas("c1","c1",0,0,1000,800);
  c1->Divide(2,2);
  c1->cd(1);
  tpc->Draw("fKyotoLy:fKyotoLz>>hhl2(200,0,2000,200,-700,100",sn&&InKyotoL&&"fKyotoLz>0","colz");

  
  c1->cd(2);
  tpc->Draw("dzKyotoL>>hhl1(100,-250,250",sn&&InKyotoL&&"fKyotoLz>0");
  tpc->Draw("dzKyotoL>>hhl1c(100,-250,250",sn&&InKyotoL&&FindKyotoL&&"fKyotoLz>0","same");
  
  cout << "L Found in Kyoto / Track " << hhl1c->GetEntries() << " / " << hhl1->GetEntries() 
       << " = " << hhl1c->GetEntries()/hhl1->GetEntries() << endl;



  c1->cd(3);
  tpc->Draw("fKyotoRy:fKyotoRz>>hhr2(200,0,2000,200,-700,100",sn&&InKyotoR&&"fKyotoRz>0","colz");

  c1->cd(4);
  tpc->Draw("dzKyotoR>>hhr1(100,-250,250",sn&&InKyotoR&&"fKyotoRz>0");
  tpc->Draw("dzKyotoR>>hhr1c(100,-250,250",sn&&InKyotoR&&FindKyotoR&&"fKyotoRz>0","same");

  cout << "R Found in Kyoto / Track " << hhr1c->GetEntries() << " / " << hhr1->GetEntries()
       << " = " << hhr1c->GetEntries()/hhr1->GetEntries() << endl;
  return;



  // tpc->Draw("STVertex.fPos.X()-tx>>htn11(200,-40,10",sn108&&bv2,"");
  // htn11->GetXaxis()->SetTitle("Vertex.X - Beam X [mm]"); 
  // c1->Update();

  tpc->Draw("STVertex.fPos.Y()-ty>>htn12(200,-250,-210",sn108&&bv2,"");
  htn12->GetXaxis()->SetTitle("Vertex.Y - Beam Y [mm]"); 
  c1->Update();


  // tpc->Draw("ty>>htv22(200,-40,40)",bestcut);
  // htv22->GetXaxis()->SetTitle("Beam Y [mm]");
  // c1->Update();

  // tpc->Draw("tx>>htv21(200,-20,60)",bestcut);
  // htv21->GetXaxis()->SetTitle("Beam X [mm]");
  // c1->Update();

  // tpc->Draw("ty:tx>>htv11(200,-20,60,200,-40,40)",bestcut,"colz");
  // htv11->GetXaxis()->SetTitle("Beam X [mm]");
  // htv11->GetYaxis()->SetTitle("Beam Y [mm]");
  //c1->Update();


  // number of tracks
  // tpc->Draw("ntrack[1]>>h1(65,0,65)",bestcut);
  // h1->GetXaxis()->SetTitle("Number of Tracks");

  // tpc->Draw("ntrack[1]>>h11(65,0,65)",bestcut&&"max_veto==0","same");
  // h11->GetXaxis()->SetTitle("Number of Tracks");
  // h11->SetLineColor(2);
  // h11->Draw("same");

  // ntrack vs KatanaV
  // tpc->Draw("max_veto:ntrack[1]>>h2(65,0,65,200,0,1000)",bestcut&&"max_veto>0&&ntrack[1]>0","colz");
  // h2->GetXaxis()->SetTitle("Number of Tracks");
  // h2->GetYaxis()->SetTitle("Katana_V(max)");



  //  tpc->Draw("dzKyotoL:hitKyotoLZ>>hl(31,168,1735,100,-100,50)","hitKyotoLZ>0","colz");
  // tpc->Draw("dzKyotoR:hitKyotoRZ>>hr(31,168,1735,100,-100,50)","hitKyotoRZ>0","colz");


}
