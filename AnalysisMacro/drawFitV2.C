{
  //  TString fname = "FlwRUN2900m9_phi.root";
  TString fname = "FlwRUN2900m9_prtnphi"; 
  //  TString fname = "FlwRUN2331m4_prtnphi"; 


  TFile *fin = new TFile(fname+".root");

  TH2D *hphip_r = (TH2D*)gROOT->FindObject("hphip_r");
  TH1D *p1 = hphip_r->ProjectionX();


  auto cc3 = new TCanvas("cc3", "cc3 Slice",1400, 1200);
  cc3->Divide(6,5);

  TF1  *f1[30];
  TH1D *phi_rm[30];

  Double_t rapid[30];
  Double_t v1[30] = {0.,0.,-2.0e-03,-0.001,-0.00041293,-0.000733738,-0.000612936,-0.000501535,-0.000197325,-0.000480613,
		     -0.000347789,-0.000326825,-0.000230786,7.61107e-05,-0.000142901,5.61907e-05,0.000125451,
		     0.000149089,0.000314133,0.000230173,0.000351465,0.000287752,-0.000226025,0.000210337,
		     5.48187e-05,-8.66898e-05,1.70578e-05,-0.000192373,-0.000156358,-0.000200014};

  Double_t v2[30] = {0.,0.,-1.5e-03,-0.001,-0.000435117,-0.000765773,-0.000675709,-0.000515314,-0.00031302,-0.000505055,
		     -0.000381801,-0.000390262,-0.000425944,-0.000403428,-0.000433422,-0.000197422,9.94755e-05,
		     1.71343e-06,-7.97936e-06,1.13789e-05,-7.44617e-05,-8.87522e-05,-0.00019023,-0.000159975,
		     6.34506e-05,-1.22038e-05,1.77018e-05,-5.66731e-05,-7.57708e-05,2.74097e-06};

  Double_t xoff[30];

  Int_t m = 0;
  Int_t k = 0;
  while( k < 30){

    
    cc3->cd(k+1);

    f1[k] = new TF1(Form("f1%d",k),"2*[0]*cos((x-[2]))+2*[1]*cos(2*(x-[2]))",-3.2,3.2);
    f1[k]->SetParameter(0,v1[k]);
    f1[k]->SetParameter(1,v2[k]);  
    f1[k]->SetParameter(2,0.2);  

    fin->cd();
    phi_rm[k] = (TH1D*)gROOT->FindObject(Form("h1phi_rm%d",k));
    if(phi_rm[k] != NULL) {
      phi_rm[k]->Fit(Form("f1%d",k));
    
      phi_rm[k]->Draw();

      if( k > 2) {
	rapid[m] = p1->GetBinCenter(k+1);
	v1[m]   = f1[k]->GetParameter(0);
	v2[m]   = f1[k]->GetParameter(1);
	xoff[m] = f1[k]->GetParameter(2);
	m++;
      }
    }
    k++;
  }

  TGraph *gv1 = new TGraph(m, rapid, v1);
  TGraph *gv2 = new TGraph(m, rapid, v2);

  TCanvas *cc1 = new TCanvas("cc1","v1 and v2",700,500);
  //  cc1->Divide(1,2);
  
  //  cc1->cd(1);
  gv1->SetMarkerStyle(20);
  gv1->SetLineColor(2);
  gv1->SetMarkerColor(2);
  gv1->SetTitle(";rapidity;v1");  
  gv1->Draw("");


  //  cc1->cd(2);
  gv2->SetMarkerStyle(20);
  gv2->SetLineColor(4);
  gv2->SetMarkerColor(4);
  gv2->SetTitle(";rapidity;v2");
  gv2->Draw("pl");
  
  auto aLeg = new TLegend(0.1,0.8,0.3,0.9);
  TString fleg[2];
  fleg[0] = " v1 ";
  fleg[1] = " v2 ";
  aLeg->AddEntry(gv1,fleg[0],"lp");
  aLeg->AddEntry(gv2,fleg[1],"lp");
  aLeg->Draw();


  cc1->SaveAs(fname+"FS.png");
  cc3->SaveAs(fname+"v1v1.png");

}
