{
  TString arun = gSystem -> Getenv("RUN");
  TString sVer = gSystem -> Getenv("VER");

  Float_t fVer = atof(sVer);

  if( arun=="" || sVer=="") {
    cout << "Plase type " << endl;
    cout << "$ RUN=#### VER=#.# root compRDvsMXplot.C" << endl;
    exit(0);
  }


  Int_t nrun = (arun.Length()-1)/5;
  cout << arun << "-> nrun " << nrun << endl;;

  TString printHeader = "FlwRUN"+arun(1,4)+ Form("m%d",nrun); 
  TString printName ;
  vector<Int_t> lrun;
  Int_t ist = 1;
  while(ist < arun.Length() - 4) {
    TString prun = arun(ist,4);
    lrun.push_back( atoi(prun) );

    ist+=5;
  }

  for(Int_t i = 0; i < nrun; i++)
    cout << " lrun = " << lrun.at(i) << endl;

 
  TChain *rChain[2];
  rChain[0] = new TChain("rflw");
  rChain[1] = new TChain("mflw");


  TString fname[2];

  for(Int_t i = 0; i < (Int_t)lrun.size(); i++){
    fname[0] = Form("../data/run%d_rdflwv"+sVer+".root",lrun.at(i));
    fname[1] = Form("../data/run%d_mxflwv"+sVer+".root",lrun.at(i));

    cout << fname[1] << endl;
    cout << fname[0] << endl;

    rChain[0]->Add(fname[0]);
    rChain[1]->Add(fname[1]);
  }


  auto aLeg = new TLegend(0.1,0.8,0.3,0.9);
  TString fleg[2];
  fleg[0] = " Real ";
  fleg[1] = " Mixed";
  Int_t iColor[2] = {2, 4};

  auto cc0 = new TCanvas("cc0", "cc0 real",700, 600);
  auto cc1 = new TCanvas("cc1", "cc1 mixed",700, 600);
  auto cc2 = new TCanvas("cc2", "cc2 real-mixed",700, 600);

  TH1D *h1phi_r[30];
  TH1D *h1phi_m[30];
  TH1D *h1phi_rm[30];

  TH1D *h1px_r[30];
  TH1D *h1px_m[30];

}


void phi()
{
  Int_t nbinx = 30; 
  gROOT->cd();

  //plot <phi> pseudorapidity 

  auto hphip_r = new TH2D("hphip_r","Real ; pseudorapidity; <phi>",nbinx,0,3.5,60,-3.2,3.2);
  auto hphip_m = new TH2D("hphip_m","Mixed; pseudorapidity; <phi>",nbinx,0,3.5,60,-3.2,3.2);


  cc0->cd();
  rChain[0] -> Draw("deltphi_b:prapid>>hphip_r","mtrack_b>0","colz");

  cc1->cd();
  rChain[1] -> Draw("deltphi_b:prapid>>hphip_m","mtrack_b>0","colz");

  cc2->cd();
  TH2D *hphip_rm = new TH2D( (*hphip_r)/(*hphip_m) );
  hphip_rm->SetName("hphip_rm");
  hphip_rm->Draw("colz");
  cc2->SetLogz();


  auto cc3 = new TCanvas("cc3", "cc3 Slice",1400, 1200);
  cc3->Divide(6,5);
  auto cc4 = new TCanvas("cc4", "cc4 Slice sub",1400, 1200);
  cc4->Divide(6,5);


  printName = printHeader+"_phi.root";
  TFile fout(printName,"recreate");

  Int_t k = 0;
  while(k <= 29){
    cc3->cd(k+1);
    h1phi_r[k] = new TH1D(*(hphip_r->ProjectionY("",k,k+1)) );
    h1phi_m[k] = new TH1D(*(hphip_m->ProjectionY("",k,k+1)) );
    
    h1phi_r[k]->SetName(Form("h1phi_r%d",k));
    h1phi_m[k]->SetName(Form("h1phi_m%d",k));


    h1phi_r[k]->SetDirectory(gROOT);
    h1phi_m[k]->SetDirectory(gROOT);

    h1phi_r[k]->SetLineColor(4);
    h1phi_m[k]->SetLineColor(2);
  
    Float_t nrmf = h1phi_r[k]->Integral();
    h1phi_r[k]->Scale(1./nrmf);

    nrmf = h1phi_m[k]->Integral();
    h1phi_m[k]->Scale(1./nrmf);

    Double_t hrng = std::max( h1phi_r[k] -> GetMaximum(), h1phi_m[k] -> GetMaximum() );
  
    h1phi_r[k]->SetMaximum(hrng);
    h1phi_r[k]->Draw();
    h1phi_m[k]->Draw("same");

    cc4->cd(k+1);
    h1phi_rm[k] = new TH1D( (*h1phi_r[k])-(*h1phi_m[k]));
    h1phi_rm[k]->SetMaximum(0.01);
    h1phi_rm[k]->SetMinimum(-0.01);
    h1phi_rm[k]->Draw("e");

    h1phi_rm[k]->SetName(Form("h1phi_rm%d",k));
    h1phi_rm[k]->SetDirectory(gROOT);
    h1phi_rm[k]->Write();

    k++;
  }
  hphip_r->Write();
  hphip_m->Write();
  fout.Write();
  fout.Close();
}


void px()
{
  Int_t nbinx = 30; 
  gROOT->cd();

  //plot <px> pseudorapidity 

  auto hpxp_r = new TH2D("hpxp_r","Real ; pseudorapidity; <px>",nbinx,0,3.5,100,-500,500);
  auto hpxp_m = new TH2D("hpxp_m","Mixed; pseudorapidity; <px>",nbinx,0,3.5,100,-500,500);


  cc0->cd();
  rChain[0] -> Draw("rpxb:prapid>>hpxp_r","mtrack_b>0","colz");

  cc1->cd();
  rChain[1] -> Draw("rpxb:prapid>>hpxp_m","mtrack_b>0","colz");

  cc2->cd();
  TH2D *hpxp_rm = new TH2D( (*hpxp_r)/(*hpxp_m) );
  hpxp_rm->SetName("hpxp_rm");
  hpxp_rm->Draw("colz");
  cc2->SetLogz();


  auto cc3 = new TCanvas("cc3", "cc3 Slice",1400, 1200);
  cc3->Divide(6,5);

  //  for(Int_t k = 0; k < nbin-1; k++){

  Int_t k = 0;
  while(k <= 29){
    cc3->cd(k+1);
    h1px_r[k] = new TH1D(*(hpxp_r->ProjectionY("",k,k+1)) ) ;
    h1px_m[k] = new TH1D(*(hpxp_m->ProjectionY("",k,k+1)) );

    Double_t bincenter = hpxp_r->GetBinCenter(k);
    h1px_r[k]->SetTitle(Form("REAL Rapidity %f",bincenter));
    h1px_m[k]->SetTitle(Form("MIX  Rapidity %f",bincenter));


    h1px_r[k]->SetLineColor(4);
    h1px_m[k]->SetLineColor(2);
  
    Double_t hrng = std::max( h1px_r[k] -> GetMaximum(), h1px_m[k] -> GetMaximum() );
  
    h1px_r[k]->SetMaximum(hrng);
    h1px_r[k]->Draw();
    h1px_m[k]->Draw("same");

    k++;
  }

}


void plot2D()
{
  hpxp_rm->FitSlicesY();
  TH1D *hpxp_rm_1 = (TH1D*)gROOT->FindObject("hpxp_rm_1");
  hpxp_rm_1->SetMaximum(800);
  hpxp_rm_1->SetMinimum(-800);
  hpxp_rm_1->Draw();
  

  printName = printHeader+"px_sy";
  //cout << " Output Name " << printName << endl;

}

void SaveCanvas(TString ext = ".png")
{
  cc0->SaveAs(printName+"RL"+ext);
  cc1->SaveAs(printName+"MX"+ext);
  cc2->SaveAs(printName+"RM"+ext);
  cc3->SaveAs(printName+"FT"+ext);
  cc4->SaveAs(printName+"SB"+ext);
}


void protonphi()
{
  Int_t nbinx = 30; 
  gROOT->cd();

  auto hphip_r = new TH2D("hphip_r","Real ; rapidity; <phi>",nbinx,0,1.,60,-3.2,3.2);
  auto hphip_m = new TH2D("hphip_m","Mixed; rapidity; <phi>",nbinx,0,1.,60,-3.2,3.2);


  cc0->cd();
  rChain[0] -> Draw("deltphi_b:rapid>>hphip_r","mtrack_b>0&&pid==2212","colz");

  cc1->cd();
  rChain[1] -> Draw("deltphi_b:rapid>>hphip_m","mtrack_b>0&&pid==2212","colz");

  cc2->cd();
  TH2D *hphip_rm = new TH2D( (*hphip_r)/(*hphip_m) );
  hphip_rm->SetName("hphip_rm");
  hphip_rm->Draw("colz");
  cc2->SetLogz();


  auto cc3 = new TCanvas("cc3", "cc3 Slice",1400, 1200);
  cc3->Divide(6,5);
  auto cc4 = new TCanvas("cc4", "cc4 Slice sub",1400, 1200);
  cc4->Divide(6,5);

  printName = printHeader+"_prtnphi";
  TFile fout(printName+".root","recreate");

  Int_t k = 0;
  while(k <= 29){
    cc3->cd(k+1);
    h1phi_r[k] = new TH1D(*(hphip_r->ProjectionY("",k,k+1)) );
    h1phi_m[k] = new TH1D(*(hphip_m->ProjectionY("",k,k+1)) );
    
    h1phi_r[k]->SetName(Form("h1phi_r%d",k));
    h1phi_m[k]->SetName(Form("h1phi_m%d",k));

    h1phi_r[k]->SetDirectory(gROOT);
    h1phi_m[k]->SetDirectory(gROOT);

    h1phi_r[k]->SetLineColor(4);
    h1phi_m[k]->SetLineColor(2);
  
    Float_t nrmf = h1phi_r[k]->Integral();
    h1phi_r[k]->Scale(1./nrmf);

    nrmf = h1phi_m[k]->Integral();
    h1phi_m[k]->Scale(1./nrmf);

    Double_t hrng = std::max( h1phi_r[k] -> GetMaximum(), h1phi_m[k] -> GetMaximum() );
  
    h1phi_r[k]->SetMaximum(hrng);
    h1phi_r[k]->Draw();
    h1phi_m[k]->Draw("same");

    cc4->cd(k+1);
    h1phi_rm[k] = new TH1D( (*h1phi_r[k])-(*h1phi_m[k]));
    h1phi_rm[k]->SetMaximum(0.01);
    h1phi_rm[k]->SetMinimum(-0.01);
    h1phi_rm[k]->Draw("e");

    h1phi_rm[k]->SetName(Form("h1phi_rm%d",k));
    h1phi_rm[k]->SetDirectory(gROOT);
    h1phi_rm[k]->Write();

    k++;
  }
  hphip_r->Write();
  hphip_m->Write();
  fout.Write();
  fout.Close();

}
