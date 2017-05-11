{
  TString arun = gSystem -> Getenv("RUN");
  TString sVer = gSystem -> Getenv("VER");

  TString ssVer = "";
  TString tVer  = sVer+".";

  for(Int_t i = 0; i < 3; i++){
    Ssiz_t idx = tVer.Index(".");
    ssVer += tVer(0 , (Int_t)idx);
    tVer.Replace(0,(Int_t)(idx+1),"");
  }
  
  Int_t fVer = atoi(ssVer);
  

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

  const Int_t nbinx = 30;
  TH1D *h2s_r[nbinx];
  TH1D *h2s_m[nbinx];
  TH1D *h2s_rm[nbinx];
  Double_t dxbin = 1./(Double_t)nbinx; 

  TH2D *h2_r;
  TH2D *h2_m;
  TH2D *h2_rm;

  TH1D *h1phimid_r;
  TH1D *h1phimid_m;
  TH1D *h1phimid_rm;

  TF1 *f1;
}

void Slice2D()
{
  auto cc3 = new TCanvas("cc3", "cc3 Slice",1200, 1000);

  if(nbinx == 30) cc3->Divide(6,5);
  else cc3->Divide(5,4);

  auto cc4 = new TCanvas("cc4", "cc4 Slice sub",1200, 1000);

  if(nbinx == 30) cc4->Divide(6,5);
  else cc4->Divide(5,4);

  TFile fout(printName+".root","recreate");

  Double_t avrg_r = h2_r->GetEntries();
  h2_r->Scale(1./avrg_r);
  Double_t avrg_m = h2_m->GetEntries();
  h2_m->Scale(1./avrg_m);

  Int_t k = 0;
  while(k < nbinx){
    cc3->cd(k+1);
    h2s_r[k] = new TH1D(*(h2_r->ProjectionY("",k,k)) );
    h2s_m[k] = new TH1D(*(h2_m->ProjectionY("",k,k)) );
    
    h2s_r[k]->SetName(Form("h2s_r%d",k));
    h2s_m[k]->SetName(Form("h2s_m%d",k));
    


    h2s_r[k]->SetDirectory(gROOT);
    h2s_m[k]->SetDirectory(gROOT);

    h2s_r[k]->SetLineColor(4);
    h2s_m[k]->SetLineColor(2);
  
    Float_t nrm_r = h2s_r[k]->Integral();
    //    h2s_r[k]->Scale(1./avrg_r);
    Float_t nrm_m = h2s_m[k]->Integral();
    //    h2s_m[k]->Scale(1./avrg_r);

    //    h2s_m[k]->Scale(1./nrm_m * nrm_r);

    Double_t hrng = std::max( h2s_r[k] -> GetMaximum(), h2s_m[k] -> GetMaximum() );
  
    h2s_r[k]->SetMaximum(hrng);
    h2s_r[k]->Draw();
    h2s_m[k]->Draw("same");

    cc4->cd(k+1);

    if(h2s_r[k]->GetEntries()>0 && h2s_m[k]->GetEntries()>0) {
      h2s_rm[k] = new TH1D( (*h2s_r[k])-(*h2s_m[k]));
      //    h2s_rm[k]->SetMaximum(0.01);
      //    h2s_rm[k]->SetMinimum(-0.01);



      h2s_rm[k]->Draw();
      //      h2s_rm[k]->Draw("e");

      h2s_rm[k]->SetName(Form("h2s_rm%d",k));
      h2s_rm[k]->SetTitle(Form("Rapidity %f",dxbin*(k+0.5)));
			    
      h2s_rm[k]->SetDirectory(gROOT);
      h2s_rm[k]->Write();
    }

    k++;
  }
  h2_r->Write();
  h2_m->Write();
  fout.Write();


  cout << printName << " is created " << endl;
  //  fout.ls();

  fout.Close();  
}


void phi()
{
  gROOT->cd();

  //plot <phi> pseudorapidity 

  h2_r = new TH2D("h2_r","Real ; pseudorapidity; #Delta#varphi",nbinx,0,3.5,60,-3.2,3.2);
  h2_m = new TH2D("h2_m","Mixed; pseudorapidity; #Delta#varphi",nbinx,0,3.5,60,-3.2,3.2);


  cc0->cd();
  rChain[0] -> Draw("deltphi_b:prapid>>h2_r","mtrack_b>0","colz");

  cc1->cd();
  rChain[1] -> Draw("deltphi_b:prapid>>h2_m","mtrack_b>0","colz");

  cc2->cd();
  TH2D *h2_rm = new TH2D( (*h2_r)/(*h2_m) );
  h2_rm->SetName("h2_rm");
  h2_rm->Draw("colz");
  cc2->SetLogz();

  printName = printHeader+"_phi";

  cout << " print name " << printName << endl;

  Slice2D();
}



void px()
{
  gROOT->cd();

  //plot <px> pseudorapidity 

  h2_r = new TH2D("hpxp_r","Real ; pseudorapidity; <px>",nbinx,0,3.5,100,-500,500);
  h2_m = new TH2D("hpxp_m","Mixed; pseudorapidity; <px>",nbinx,0,3.5,100,-500,500);

  cc0->cd();
  rChain[0] -> Draw("px:rapid>>h2_r","mtrack_b>0&&pid==12212","colz");

  cc1->cd();
  rChain[1] -> Draw("px:rapid>>h2_m","mtrack_b>0&&pid==12212","colz");

  cc2->cd();
  TH2D *h2_rm = new TH2D( (*h2_r)/(*h2_m) );
  h2_rm->SetName("h2_rm");
  h2_rm->Draw("colz");
  cc2->SetLogz();

  printName = printHeader+"_px";

  Slice2D();
}

void SaveCanvas(TString ext = ".png")
{
  if(cc0 != NULL)  cc0->SaveAs(printName+"RL"+ext);
  if(cc1 != NULL)  cc1->SaveAs(printName+"MX"+ext);
  if(cc2 != NULL)  cc2->SaveAs(printName+"RM"+ext);
  if(cc3 != NULL)  cc3->SaveAs(printName+"FT"+ext);
  if(cc4 != NULL)  cc4->SaveAs(printName+"SB"+ext);
  if(cca1!= NULL)  cca1->SaveAs(printName+"FS.png");

}

void protonphi()
{
  gROOT->cd();

  h2_r = new TH2D("h2_r","Real ; rapidity; #Delta#varphi",nbinx,0,1.,60,-3.2,3.2);
  h2_m = new TH2D("h2_m","Mixed; rapidity; #Delta#varphi",nbinx,0,1.,60,-3.2,3.2);

  Double_t ddphi = 60./6.4*30;

  cc0->cd();
  TString aPID="";
  if(fVer >= 207)
    aPID = "&&pid==12212";
  else
    aPID = "&&pid==2212";

  rChain[0] -> Draw("deltphi_b:rapid>>h2_r","mtrack_b>0"+aPID,"colz");
  h2_r->Scale(ddphi);

  cc1->cd();
  rChain[1] -> Draw("deltphi_b:rapid>>h2_m","mtrack_b>0"+aPID,"colz");
  h2_m->Scale(ddphi);

  cc2->cd();
  TH2D *h2_rm = new TH2D( (*h2_r)/(*h2_m) );
  h2_rm->SetName("h2_rm");
  h2_rm->Draw("colz");
  cc2->SetLogz();

  printName = printHeader+"_prtnphi";
  
  cout << " print name " << printName << endl;

  Slice2D();  

  fitv();
}

void protonPt()
{
  gROOT->cd();

  h2_r = new TH2D("h2_r","Real ; rapidity; pt",nbinx,0,1.,100,0,1000.);
  h2_m = new TH2D("h2_m","Mixed; rapidity; pt",nbinx,0,1.,100,0,1000.);


  cc0->cd();
  rChain[0] -> Draw("p_rot.Pt():rapid>>h2_r","mtrack_b>0&&pid==12212","colz");

  cc1->cd();
  rChain[1] -> Draw("p_rot.Pt():rapid>>h2_m","mtrack_b>0&&pid==12212","colz");

  cc2->cd();
  TH2D *h2_rm = new TH2D( (*h2_r)/(*h2_m) );
  h2_rm->SetName("h2_rm");
  h2_rm->Draw("colz");
  cc2->SetLogz();

  printName = printHeader+"_prtnpt";
  
  Slice2D();  
}


void midproton()
{
  gStyle->SetOptFit(1111);
  
  gROOT->cd();

  auto hphip_r = new TH2D("hphip_r","Real ; rapidity; #Delta#varphi",nbinx,0,1.,60,-3.2,3.2);
  auto hphip_m = new TH2D("hphip_m","Mixed; rapidity; #Delta#varphi",nbinx,0,1.,60,-3.2,3.2);


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
  
  Float_t nrm_r = h1phimid_r->Integral();
  //  h1phimid_r->Scale(1./nrmf);

  Float_t nrm_m = h1phimid_m->Integral();
  h1phimid_m->Scale(1./nrm_m * nrm_r);

  cc2->cd();
  h1phimid_rm = new TH1D( (*h1phimid_r)-(*h1phimid_m));
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

  h2_r = new TH2D("h2_r","Real ; dPhi_beam; d(Phi_t - Phi_b)",nbinx,0.,6.3,60,0.,6.3);
  h2_m = new TH2D("h2_m","Mixed; dPhi_beam; d(Phi_t - Phi_b)",nbinx,0.,6.3,60,0.,6.3);
  
  cc0->cd();
  rChain[0]->Draw("TVector2::Phi_0_2pi(unitP_t.Phi()-unitP_b.Phi()):unitP_b.Phi()>>h2_r","mtrack_b>0&&mtrack_t>0&&pid==2212","colz");

  cc1->cd();
  rChain[1]->Draw("TVector2::Phi_0_2pi(unitP_t.Phi()-unitP_b.Phi()):unitP_b.Phi()>>h2_m","mtrack_b>0&&mtrack_t>0&&pid==2212","colz");

  cc2->cd();
  TH2D *h2_rm = new TH2D( (*h2_r)/(*h2_m) );
  h2_rm->SetName("h2_rm");
  h2_rm->SetTitle("REAL/MiXed");
  h2_rm->Draw("colz");
  cc2->SetLogz();

  printName = printHeader+"_phiphi";

  Slice2D();

}


void v1()
{

  TDatime dtime;
  TDatime btime(dtime);


  TH1I *h1m[2];
  h1m[0] = new TH1I("h1m0","mult real ",20,0,20);
  h1m[1] = new TH1I("h1m1","mult mixed",20,0,20);

  TH1D *h1v1[2];
  h1v1[0] = new TH1D("h1v10","cos(dphi) real ",120,-1.,1.);
  h1v1[1] = new TH1D("h1v11","cos(dphi) real ",120,-1.,1.);

  TH1D *h1mv1[2];
  h1mv1[0] = new TH1D("h1mv10","<cos(dphi)> real ",120,-1.,1.);
  h1mv1[1] = new TH1D("h1mv11","<cos(dphi)> mixed ",120,-1.,1.);

  TH2D *h2mv1y[2];
  h2mv1y[0] = new TH2D("h2mv1y0","<cos(dphi)> rapidity real ",nbinx,0.,1.,120,-1.,1.);
  h2mv1y[1] = new TH2D("h2mv1y1","<cos(dphi)> rapidity mixed",nbinx,0.,1.,120,-1.,1.);

  auto h2dphiy = new TH2D("h2dphiy","dphi vs rapidity",nbinx,0.,1.,120,-1.,1.);

  for(Int_t irm = 0; irm < 2; irm++){
    
    cout << " irm ------------> " << irm << endl;

    Int_t Entry = rChain[irm]->GetEntries();

    //  Entry = 2;
    for(Int_t i = 0; i < Entry; i++){
      
      if(i%10000==0) {
	dtime.Set();
	cout << " processed " << i << " dtime --- " << dtime.AsString() << endl; 
      }

    
      rChain[irm]->Draw("cos(deltphi_b):rapid>>h2dphiy","pid==12212&&mtrack_b>0&&mtrack>1","",1,i);
      if(h2dphiy->GetEntries() == 0) continue;

      Int_t k = 0;
      while(k < nbinx){
	TH1D h2s(*(h2dphiy->ProjectionY("",k,k)) );
	h2s.SetName("h2s");

	cc0->cd();
	h2s.Draw("box");

	Int_t    multv1 = h2s.GetEntries();
	if(multv1 > 0){
	  Double_t meanv1 = h2s.GetMean();
	  
	  //	cout << "event " << i <<  " k : "<< k << " mult " << multv1 << " meanv1 " << meanv1 << endl;
	  
	  h1mv1[irm]->Fill(meanv1);
	  h1m[irm]  ->Fill(multv1);
	
	  h2mv1y[irm]->Fill(dxbin*((Double_t)k+0.5),meanv1);
	
	}
	k++;
	
      }
    }
  }

  h2_r = h2mv1y[0];
  h2_m = h2mv1y[1];
  
  Slice2D();

}

void v2()
{

  TDatime dtime;
  TDatime btime(dtime);

  TH1I *h1m[2];
  h1m[0] = new TH1I("h1m0","mult real ",20,0,20);
  h1m[1] = new TH1I("h1m1","mult mixed",20,0,20);

  TH1D *h1v1[2];
  h1v1[0] = new TH1D("h1v10","cos(dphi) real ",120,-1.,1.);
  h1v1[1] = new TH1D("h1v11","cos(dphi) real ",120,-1.,1.);

  TH1D *h1mv1[2];
  h1mv1[0] = new TH1D("h1mv10","<cos(dphi)> real ",120,-1.,1.);
  h1mv1[1] = new TH1D("h1mv11","<cos(dphi)> mixed ",120,-1.,1.);

  TH2D *h2mv1y[2];
  h2mv1y[0] = new TH2D("h2mv1y0","<cos(dphi)> rapidity real ",nbinx,0.,1.,120,-1.,1.);
  h2mv1y[1] = new TH2D("h2mv1y1","<cos(dphi)> rapidity mixed",nbinx,0.,1.,120,-1.,1.);

  auto h2dphiy = new TH2D("h2dphiy","dphi vs rapidity",nbinx,0.,1.,120,-1.,1.);

  for(Int_t irm = 0; irm < 2; irm++){
    
    cout << " irm ------------> " << irm << endl;

    Int_t Entry = rChain[irm]->GetEntries();

    //  Entry = 2;
    for(Int_t i = 0; i < Entry; i++){
      
      if(i%10000==0) {
	dtime.Set();
	cout << " processed " << i << " dtime --- " << dtime.AsString() << endl; 
      }

    
      rChain[irm]->Draw("cos(2*deltphi_b):rapid>>h2dphiy","pid==12212&&mtrack_b>0&&mtrack>1","",1,i);
      if(h2dphiy->GetEntries() == 0) continue;

      Int_t k = 0;
      while(k < nbinx){
	TH1D h2s(*(h2dphiy->ProjectionY("",k,k)) );
	h2s.SetName("h2s");

	cc0->cd();
	h2s.Draw("box");

	Int_t    multv1 = h2s.GetEntries();
	if(multv1 > 0){
       	  Double_t meanv1 = h2s.GetMean();
	  
	  //	cout << "event " << i <<  " k : "<< k << " mult " << multv1 << " meanv1 " << meanv1 << endl;
	  
	  h1mv1[irm]->Fill(meanv1);
	  h1m[irm]  ->Fill(multv1);
	
	  h2mv1y[irm]->Fill(dxbin*((Double_t)k+0.5),meanv1);
	
	}
	k++;
	
      }
    }
  }

  h2_r = h2mv1y[0];
  h2_m = h2mv1y[1];
  
  Slice2D();

}


void fitv()
{

  TH2D *h2_r = (TH2D*)gROOT->FindObject("h2_r");
  if(!h2_r) {
    h2_r = (TH2D*)gROOT->FindObject("hphip_r");
    if(!h2_r) exit(0);
  }

  TH1D *p1 = h2_r->ProjectionX();


  TF1  *f1[nbinx];

  Double_t rapid[nbinx];
  Double_t v1[30] = {0.,0.,-2.0e-03,-0.001,-0.00041293,-0.000733738,-0.000612936,-0.000501535,-0.000197325,-0.000480613,
		     -0.000347789,-0.000326825,-0.000230786,7.61107e-05,-0.000142901,5.61907e-05,0.000125451,
		     0.000149089,0.000314133,0.000230173,0.000351465,0.000287752,-0.000226025,0.000210337,
		     5.48187e-05,-8.66898e-05,1.70578e-05,-0.000192373,-0.000156358,-0.000200014};

  Double_t v2[30] = {0.,0.,-1.5e-03,-0.001,-0.000435117,-0.000765773,-0.000675709,-0.000515314,-0.00031302,-0.000505055,
		     -0.000381801,-0.000390262,-0.000425944,-0.000403428,-0.000433422,-0.000197422,9.94755e-05,
		     1.71343e-06,-7.97936e-06,1.13789e-05,-7.44617e-05,-8.87522e-05,-0.00019023,-0.000159975,
		     6.34506e-05,-1.22038e-05,1.77018e-05,-5.66731e-05,-7.57708e-05,2.74097e-06};

  Double_t xoff[nbinx];
  Double_t v1e[nbinx];
  Double_t v2e[nbinx];

  Int_t m = 0;
  Int_t k = 0;
  while( k < nbinx){

    if(h2s_rm[k] != NULL){

      f1[k] = new TF1(Form("f1%d",k),"2*[0]*cos((x-[2]))+2*[1]*cos(2*(x-[2]))",-3.2,3.2);
      f1[k]->SetParameter(0,v1[k]);
      f1[k]->SetParameter(1,v2[k]);  
      f1[k]->SetParameter(2,0.2);  
    
      if(h2s_rm[k]->GetEntries() != 0) {
	h2s_rm[k]->Fit(f1[k]);
	
	rapid[m] = p1->GetBinCenter(k);
	v1[m]   = f1[k]->GetParameter(0);
	v1e[m]  = f1[k]->GetParError(0);
	v2[m]   = f1[k]->GetParameter(1);
	v2e[m]  = f1[k]->GetParError(1);
	xoff[m] = f1[k]->GetParameter(2);
	m++;

      }
    }
     
    k++;
    
  }

  TGraph *gv1 = new TGraph(m, rapid, v1);
  TGraph *gv2 = new TGraph(m, rapid, v2);
  //  TGraphErrors *gv1 = new TGraphErrors(m, rapid, v1, 0, v1e);
  //  TGraphErrors *gv2 = new TGraphErrors(m, rapid, v2, 0, v1e);

  TCanvas *cca1 = new TCanvas("cca1","cca1 v1 and v2",700,500);
  cca1->Divide(1,2);
  
  cca1->cd(1);
  gv1->SetMarkerStyle(20);
  gv1->SetLineColor(2);
  gv1->SetMarkerColor(2);
  gv1->SetTitle(";rapidity;v1");  
  // gv1->SetMaximum(0.002);
  // gv1->SetMinimum(-0.002);
  gv1->Draw("APL");


  cca1->cd(2);
  gv2->SetMarkerStyle(20);
  gv2->SetLineColor(4);
  gv2->SetMarkerColor(4);
  gv2->SetTitle(";rapidity;v2");
  // gv2->SetMaximum(0.0005);
  // gv2->SetMinimum(-0.0005);
  gv2->Draw("APL");
  
  // auto aLeg = new TLegend(0.1,0.8,0.3,0.9);
  // TString fleg[2];
  // fleg[0] = " v1 ";
  // fleg[1] = " v2 ";
  // aLeg->AddEntry(gv1,fleg[0],"lp");
  // aLeg->AddEntry(gv2,fleg[1],"lp");
  // aLeg->Draw();



}
