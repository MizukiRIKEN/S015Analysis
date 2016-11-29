{

  Int_t iRun = 2331;

  TFile *_file0 = TFile::Open(Form("../data/run%d_asm5.v1.03.root",iRun));
  TString pRun = Form("%d",iRun);

  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);

  //--------------------start
  //
  //  Int_t nEntry = tpc->GetEntries();

  TLatex tRun(0,-450,"RUN_"+pRun);



  auto c1 = new TCanvas("c1","c1",1000,0,800,600);
 
  c1->Divide(1,2);
  c1->cd(1);
  tpc->Draw("KatanaMult:trknKATANA>>hm(20,0,20,13,0,13)",sn,"colz");
  hm->GetXaxis()->SetTitle("Number of Trakcs in TPC");
  hm->GetYaxis()->SetTitle("Multiplicity on KATANA");
  hm->Draw("colz");
  c1->cd(2);
  tpc->Draw("KatanaMult:foundKATANA>>hm1(20,0,20,13,0,13)",sn,"colz");



  c1->Update();

  auto c2 = new TCanvas("c2","c2",0,0,800,600);
  c2->Divide(1,2);
  c2->cd(1);
  tpc->Draw("foundKATANA:trknKATANA>>hm2(20,0,20,20,0,20)",sn,"colz");

  c2->cd(2);
  tpc->Draw("foundKATANA/trknKATANA>>hm3(20,0,1)",sn);
  hm3->GetXaxis()->SetTitle("Associated Track/All Track");
  hm3->Draw();
  c2->Update();



  auto c3 = new TCanvas("c3","c3",0,1000,800,600);
  
  c3->Divide(1,2);
  c3->cd(1);
  tpc->Draw("fKatanay:fKatanax>>h3(200,-1000,400,200,-500,50",sn&&"fndKATANA","colz");
  tRun.Draw();

  c3->cd(2);
  tpc->Draw("fKatanay:fKatanax>>h4(200,-1000,400,200,-500,50",sn&&"fndKATANA==0&&hitKATANAX<999","colz");
  
}


void backup()
{
 
  c2->Divide(1,2);

  c2->cd(1);
  c2->GetPad(1)->SetLeftMargin(0.058f);
  c2->GetPad(1)->SetRightMargin(0.070f);

  tpc->Draw("fKatanay:fKatanax>>h2(200,-1000,600,200,-500,50",InKatana&&sn,"colz");
  tRun.Draw();

  c2->GetPad(2)->Divide(2,1);
  c2->GetPad(2)->cd(1);
  c2->cd(3);
  tpc->Draw("fKatanax>>hx1(200,-1000,600)",InKatana&&sn,"colz");
  c2->GetPad(2)->cd(2);
  c2->cd(4);
  tpc->Draw("fKatanay>>hx2(200,-500,50)",InKatana&&sn,"colz");

}
