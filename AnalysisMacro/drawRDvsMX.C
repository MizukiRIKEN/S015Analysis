{
  TString arun = gSystem -> Getenv("RUN");
  TString sVer = gSystem -> Getenv("VER");

  Float_t fVer = atof(sVer);


  if( arun=="" || sVer=="" || arun.Length() == 4) {
    cout << "Plase type " << endl;
    cout << "RUN={####} VER=#.#.# root compRDvsMXplot.C" << endl;
    exit(0);
  }


  Int_t nrun = (arun.Length()-1)/5;
  cout << arun << "-> nrun " << nrun << endl;;

  TString printHeader = "FlwRUN"+arun(1,4)+ Form("m%d",nrun) + "_v" + sVer; 
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
    TString rootdir = "../data"; 
    fname[0] = Form("run%d_rdflw_v"+sVer+".root",lrun.at(i));
    fname[1] = Form("run%d_mxflw_v"+sVer+".root",lrun.at(i));

    cout << fname[1] << endl;
    cout << fname[0] << endl;

    if(gSystem->FindFile(rootdir,fname[0]))
      rChain[0]->Add(fname[0]);
    else
      cout << " File is not found " << fname[0] << endl;

    if(gSystem->FindFile(rootdir,fname[1]))
      rChain[1]->Add(fname[1]);
    else
      cout << " File is not found " << fname[1] << endl;

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

  TH1D *h1phiphi_r[30];
  TH1D *h1phiphi_m[30];
  TH1D *h1phiphi_rm[30];

  TH1D *h1phimid_r;
  TH1D *h1phimid_m;
  TH1D *h1phimid_rm;

  TF1 *f1;
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
  if(cc0 != NULL)  cc0->SaveAs(printName+"RL"+ext);
  if(cc1 != NULL)  cc1->SaveAs(printName+"MX"+ext);
  if(cc2 != NULL)  cc2->SaveAs(printName+"RM"+ext);
  if(cc3 != NULL)  cc3->SaveAs(printName+"FT"+ext);
  if(cc4 != NULL)  cc4->SaveAs(printName+"SB"+ext);
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


void midproton()
{
  gStyle->SetOptFit(1111);
  
  Int_t nbinx = 30; 
  gROOT->cd();

  auto hphip_r = new TH2D("hphip_r","Real ; rapidity; <phi>",nbinx,0,1.,60,-3.2,3.2);
  auto hphip_m = new TH2D("hphip_m","Mixed; rapidity; <phi>",nbinx,0,1.,60,-3.2,3.2);


  cc0->cd();
  rChain[0] -> Draw("deltphi_b:rapid>>hphip_r","mtrack_b>0&&pid==2212","colz");

  cc1->cd();
  rChain[1] -> Draw("deltphi_b:rapid>>hphip_m","mtrack_b>0&&pid==2212","colz");

  h1phimid_r = new TH1D(*(hphip_r->ProjectionY("",10,14)) );
  h1phimid_m = new TH1D(*(hphip_m->ProjectionY("",10,14)) );
  
  h1phimid_r->SetName("h1phimid_r");
  h1phimid_m->SetName("h1phimid_m");

  h1phimid_r->SetDirectory(gROOT);
  h1phimid_m->SetDirectory(gROOT);

  h1phimid_r->SetLineColor(4);
  h1phimid_m->SetLineColor(2);
  
  Float_t nrmf = h1phimid_r->Integral();
  h1phimid_r->Scale(1./nrmf);

  nrmf = h1phimid_m->Integral();
  h1phimid_m->Scale(1./nrmf);

  cc2->cd();
  h1phimid_rm = new TH1D( (*h1phimid_r)-(*h1phimid_m));
  h1phimid_rm->SetMaximum(0.002);
  h1phimid_rm->SetMinimum(-0.002);
  h1phimid_rm->Draw("");

  Double_t v1 = -0.0002;
  Double_t v2 = -0.0001;
  Double_t xoff = 0.;

  f1 = new TF1("f1","[3]+2*[0]*cos((x-[2]))+2*[1]*cos(2*(x-[2]))",-3.2,3.2);
  f1->SetParameter(0,v1);
  f1->SetParameter(1,v2);
  f1->SetParameter(2,xoff);
  f1->SetParameter(3,100);

  h1phimid_rm->Fit("f1");
  h1phimid_rm->Fit("f1");

}


void phiphi()
{
  Int_t nbinx = 30;

  auto hphiphi_r = new TH2D("hphiphi_r","Real ; dPhi_beam; d(Phi_t - Phi_b)",nbinx,0.,6.3,60,0.,6.3);
  auto hphiphi_m = new TH2D("hphiphi_m","Mixed; dPhi_beam; d(Phi_t - Phi_b)",nbinx,0.,6.3,60,0.,6.3);
  

  cc0->cd();
  rChain[0]->Draw("TVector2::Phi_0_2pi(unitP_t.Phi()-unitP_b.Phi()):unitP_b.Phi()>>hphiphi_r","mtrack_b>0&&mtrack_t>0&&pid==2212","colz");

  cc1->cd();
  rChain[1]->Draw("TVector2::Phi_0_2pi(unitP_t.Phi()-unitP_b.Phi()):unitP_b.Phi()>>hphiphi_m","mtrack_b>0&&mtrack_t>0&&pid==2212","colz");

  cc2->cd();
  TH2D *hphiphi_rm = new TH2D( (*hphiphi_r)/(*hphiphi_m) );
  hphiphi_rm->SetName("hphiphi_rm");
  hphiphi_rm->SetTitle("REAL/MiXed");
  hphiphi_rm->Draw("colz");
  cc2->SetLogz();

  auto cc3 = new TCanvas("cc3", "cc3 Slice",1400, 1200);
  cc3->Divide(6,5);
  auto cc4 = new TCanvas("cc4", "cc4 Slice sub",1400, 1200);
  cc4->Divide(6,5);

  printName = printHeader+"_phiphi";
  TFile fout(printName+".root","recreate");

  Int_t k = 0;
  while(k <= 29){
    cc3->cd(k+1);
    h1phiphi_r[k] = new TH1D(*(hphiphi_r->ProjectionY("",k,k+1)) );
    h1phiphi_m[k] = new TH1D(*(hphiphi_m->ProjectionY("",k,k+1)) );
    
    h1phiphi_r[k]->SetName(Form("h1phiphi_r%d",k));
    h1phiphi_m[k]->SetName(Form("h1phiphi_m%d",k));

    h1phiphi_r[k]->SetDirectory(gROOT);
    h1phiphi_m[k]->SetDirectory(gROOT);

    h1phiphi_r[k]->SetLineColor(4);
    h1phiphi_m[k]->SetLineColor(2);
  
    Float_t nrmf = h1phiphi_r[k]->Integral();
    h1phiphi_r[k]->Scale(1./nrmf);

    nrmf = h1phiphi_m[k]->Integral();
    h1phiphi_m[k]->Scale(1./nrmf);

    Double_t hrng = std::max( h1phiphi_r[k] -> GetMaximum(), h1phiphi_m[k] -> GetMaximum() );
  
    h1phiphi_r[k]->SetMaximum(hrng);
    h1phiphi_r[k]->Draw();
    h1phiphi_m[k]->Draw("same");

    cc4->cd(k+1);
    h1phiphi_rm[k] = new TH1D( (*h1phiphi_r[k])-(*h1phiphi_m[k]));
    h1phiphi_rm[k]->SetMaximum(0.01);
    h1phiphi_rm[k]->SetMinimum(-0.01);
    h1phiphi_rm[k]->Draw("e");

    h1phiphi_rm[k]->SetName(Form("h1phiphi_rm%d",k));
    h1phiphi_rm[k]->SetDirectory(gROOT);
    h1phiphi_rm[k]->Write();

    k++;
  }
  hphiphi_r->Write();
  hphiphi_m->Write();
  fout.Write();
  fout.Close();

}
