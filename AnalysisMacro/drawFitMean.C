{
  TString fname = "Flw2v2900m30_phiphi";

  TFile *fin = new TFile(fname+".root");

  TH2D *hphiphi_r = (TH2D*)gROOT->FindObject("hphiphi_r");
  TH1D *p1 = hphiphi_r->ProjectionX();


  auto cc3 = new TCanvas("cc3", "cc3 Slice",1400, 1200);
  cc3->Divide(6,5);

  TF1  *f1[30];
  TH1D *phiphi_rm[30];

  Double_t dphi[30];
  Double_t wphi[30];
  Double_t mean[30];
  Double_t sigm[30];
  Double_t amp[30];

  for(Int_t iu = 0; iu < 30; iu++){
    mean[iu] = 2.7;
    sigm[iu] = 0.6;
    amp[iu]  = 0.01;
  } 
  mean[1] = 2.7; sigm[1]=0.54; amp[1]=0.01;


  Int_t m = 0;
  Int_t k = 0;
  while( k < 30){

    
    cc3->cd(k+1);

    f1[k] = new TF1(Form("f1%d",k),"gaus",0.,6.);
    f1[k]->SetParameter(0,amp[k]);
    f1[k]->SetParameter(1,mean[k]);  
    f1[k]->SetParameter(2,sigm[k]);  

    fin->cd();
    phiphi_rm[k] = (TH1D*)gROOT->FindObject(Form("h1phiphi_rm%d",k));
    if(phiphi_rm[k] != NULL) {
      phiphi_rm[k]->Fit(Form("f1%d",k));
    
      phiphi_rm[k]->Draw();

      dphi[m]   = p1->GetBinCenter(k+1);
      wphi[m]   = p1->GetBinWidth(k+1);
      amp[m]    = f1[k]->GetParameter(0);
      mean[m]   = f1[k]->GetParameter(1);
      sigm[m]   = f1[k]->GetParameter(2);
      
      m++;

    }
    k++;
  }

  TGraphErrors *g1 = new TGraphErrors(m, dphi, mean, wphi, sigm);

   TCanvas *cc1 = new TCanvas("cc1","v1 and v2",700,500);
  
  // g1->SetMarkerStyle(20);
  // g1->SetLineColor(2);
  // g1->SetMarkerColor(2);
  // g1->SetTitle(";rapidity;v1");  
  // g1->SetMarkerColor(4);
  // g1->SetMarkerStyle(21);
  // g1->Draw("ALP");
  
  // cc1->SaveAs(fname+"FS.png");
  //  cc3->SaveAs(fname+"v1v1.png");

}
