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

void v1()
{
  auto cc0 = new TCanvas("cc0", "cc0",700, 600);
  auto cc1 = new TCanvas("cc1", "cc1",700, 600);
  auto cc3 = new TCanvas("cc3", "cc3",700, 600);
  auto cc4 = new TCanvas("cc4", "cc4",700, 600);

  TDatime dtime;
  TDatime btime(dtime);

  Double_t yrang[2] = {0.,1.};

  TH2D *hdphi[2]; 
  TH2D *hdcos[2]; 

  hdphi[0] = new TH2D("hdphi0","dphi "    ,nbinx,yrang[0],yrang[1],60,-3.2,3.2);
  hdcos[0] = new TH2D("hdcos0","cos(phi) ",nbinx,yrang[0],yrang[1],60,-1.,1.);
  hdphi[1] = new TH2D("hdphi1","dphi "    ,nbinx,yrang[0],yrang[1],60,-3.2,3.2);
  hdcos[1] = new TH2D("hdcos1","cos(phi) ",nbinx,yrang[0],yrang[1],60,-1.,1.);


  Int_t   mtrack_b;

  vector<Int_t>    *pid=0;
  vector<Double_t> *deltphi_b=0;
  vector<Double_t> *rapid=0;
  TBranch *brapid;
  TBranch *bdeltphi_b;
  TBranch *bpid;

  Int_t    nproton;
  Double_t aveY[nbinx];
  Double_t avePhi[nbinx];
  Double_t aveCos[nbinx];


  for(Int_t m = 0; m < 2; m++){

    rChain[m]->SetBranchAddress("mtrack_b",&mtrack_b);
    rChain[m]->SetBranchAddress("deltphi_b",&deltphi_b,&bdeltphi_b);
    rChain[m]->SetBranchAddress("rapid",&rapid,&brapid);
    rChain[m]->SetBranchAddress("pid",&pid,&bpid);

    Int_t nevt = rChain[m]->GetEntries();  

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);


      if(mtrack_b == 0) continue;
      bdeltphi_b->GetEntry(i);    
      brapid->GetEntry(i);    
      bpid->GetEntry(i);    
     
      vector< vector<Double_t> > vecY(nbinx);
      vector< vector<Double_t> > vecPhi(nbinx);
      vector< vector<Double_t> > vecCos(nbinx);

      Int_t nproton = 0;
      for(Int_t j = 0; j < deltphi_b->size() ; j++){

	//@@@@@
	if( pid->at(j) == 12212 && rapid->at(j) < 1.0 ){
	
	  Int_t ybin = (Int_t)(rapid->at(j)/dxbin);

	  vecY[ybin]  .push_back(rapid->at(j));
	  vecPhi[ybin].push_back(deltphi_b->at(j));
	  vecCos[ybin].push_back(cos(deltphi_b->at(j)));

	  nproton++;
	

	}
      }
  
      for(Int_t j = 0; j < nbinx; j++){

	aveY[j]   = 0.;
	avePhi[j] = 0.;
	aveCos[j] = 0.;

	if( vecY[j].size() != 0 ){		
	
	  for(Int_t k = 0; k < vecY[j].size(); k++ ) {
	    aveY[j]   += vecY[j].at(k);
	    avePhi[j] += vecPhi[j].at(k);
	    aveCos[j] += vecCos[j].at(k);
	  }

	  avePhi[j] /= (Double_t)vecY[j].size();
	  aveCos[j] /= (Double_t)vecY[j].size();

	}
    

	if(aveY[j] != 0 && avePhi[j] !=0 && aveCos[j] != 0){
	  hdphi[m]->Fill(aveY[j], avePhi[j]);
	  hdcos[m]->Fill(aveY[j], aveCos[j]);
	}
      }

    }
  }
  cc0->Divide(1,2);
  cc0->cd(1);
  hdphi[0]->Draw("colz");
  cc0->cd(2);
  hdphi[1]->Draw("colz");
  

  cc1->Divide(1,2);
  cc1->cd(1);
  hdcos[0]->Draw("colz");
  cc1->cd(2);
  hdcos[1]->Draw("colz");
  
  cc3->Divide(1,2);
  cc4->Divide(1,2);

  TH2D *hdphi_rm = new TH2D( (*hdphi[0])/(*hdphi[1]) );
  hdphi_rm->SetName("hdphi_rm");
  cc3->cd(1);  hdphi_rm->Draw("colz");
  cc4->cd(1);  hdphi_rm->ProfileX("hdphi_rm_pfx",1,-1,"d");
  


  TH2D *hdcos_rm = new TH2D( (*hdcos[0])/(*hdcos[1]) );
  hdcos_rm->SetName("hdcos_rm");
  cc3->cd(2);  hdcos_rm->Draw("colz");
  cc4->cd(2);  hdcos_rm->ProfileX("hdcos_rm_pfx",1,-1,"d");
}

void vn(Double_t vn = 2)
{
  auto cc0 = new TCanvas("cc0", "cc0",700, 600);
  auto cc1 = new TCanvas("cc1", "cc1",700, 600);
  auto cc3 = new TCanvas("cc3", "cc3",700, 600);
  auto cc4 = new TCanvas("cc4", "cc4",700, 600);

  TDatime dtime;
  TDatime btime(dtime);

  Double_t yrang[2] = {0.,1.};

  TH2D *hdphi[2]; 
  TH2D *hdcos[2]; 

  hdphi[0] = new TH2D("hdphi0","2dphi "    ,nbinx,yrang[0],yrang[1],60,0.,3.2);
  hdcos[0] = new TH2D("hdcos0","cos(2phi) ",nbinx,yrang[0],yrang[1],60,-1.,1.);
  hdphi[1] = new TH2D("hdphi1","2dphi "    ,nbinx,yrang[0],yrang[1],60,0.,3.2);
  hdcos[1] = new TH2D("hdcos1","cos(2phi) ",nbinx,yrang[0],yrang[1],60,-1.,1.);


  Int_t   mtrack_b;

  vector<Int_t>    *pid=0;
  vector<Double_t> *deltphi_b=0;
  vector<Double_t> *rapid=0;
  TBranch *brapid;
  TBranch *bdeltphi_b;
  TBranch *bpid;

  Int_t    nproton;
  Double_t aveY[nbinx];
  Double_t avePhi[nbinx];
  Double_t aveCos[nbinx];

  for(Int_t m = 0; m < 2; m++){

    rChain[m]->SetBranchAddress("mtrack_b",&mtrack_b);
    rChain[m]->SetBranchAddress("deltphi_b",&deltphi_b,&bdeltphi_b);
    rChain[m]->SetBranchAddress("rapid",&rapid,&brapid);
    rChain[m]->SetBranchAddress("pid",&pid,&bpid);

    Int_t nevt = rChain[m]->GetEntries();  

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack_b == 0) continue;
      bdeltphi_b->GetEntry(i);    
      brapid->GetEntry(i);    
      bpid->GetEntry(i);    
     
      vector< vector<Double_t> > vecY(nbinx);
      vector< vector<Double_t> > vecPhi(nbinx);
      vector< vector<Double_t> > vecCos(nbinx);

      Int_t nproton = 0;
      for(Int_t j = 0; j < deltphi_b->size() ; j++){

	//@@@@@
	if( pid->at(j) == 12212 && rapid->at(j) < 1.0 ){
	
	  Int_t ybin = (Int_t)(rapid->at(j)/dxbin);

	  vecY[ybin]  .push_back(rapid->at(j));

	  Double_t dblphi = abs(TVector2::Phi_mpi_pi(vn*deltphi_b->at(j)));
	  vecPhi[ybin].push_back(dblphi);
	  vecCos[ybin].push_back(cos(dblphi));

	  nproton++;
	

	}
      }
  
      for(Int_t j = 0; j < nbinx; j++){

	aveY[j]   = 0.;
	avePhi[j] = 0.;
	aveCos[j] = 0.;

	if( vecY[j].size() != 0 ){		
	
	  for(Int_t k = 0; k < vecY[j].size(); k++ ) {
	    aveY[j]   += vecY[j].at(k);
	    avePhi[j] += vecPhi[j].at(k);
	    aveCos[j] += vecCos[j].at(k);
	  }

	  avePhi[j] /= (Double_t)vecY[j].size();
	  aveCos[j] /= (Double_t)vecY[j].size();
	}
    
	if(aveY[j] != 0 && avePhi[j] !=0 && aveCos[j] != 0){
	  hdphi[m]->Fill(aveY[j], avePhi[j]);
	  hdcos[m]->Fill(aveY[j], aveCos[j]);

	  //	  cout << m <<  " th:  phi["<< j << "] " << avePhi[j] << " " << aveCos[j] << endl;

	}
      }
    }
  }
  cc0->Divide(1,2);
  cc0->cd(1);
  hdphi[0]->Draw("colz");
  cc0->cd(2);
  hdphi[1]->Draw("colz");
  

  cc1->Divide(1,2);
  cc1->cd(1);
  hdcos[0]->Draw("colz");
  cc1->cd(2);
  hdcos[1]->Draw("colz");
  
  cc3->Divide(1,2);
  cc4->Divide(1,2);

  TH2D *hdphi_rm = new TH2D( (*hdphi[0])/(*hdphi[1]) );
  hdphi_rm->SetName("hdphi_rm");

  cc3->cd(1);  hdphi_rm->Draw("colz");
  cc4->cd(1);  hdphi_rm->ProfileX("hdphi_rm_pfx",1,-1,"d");
  


  TH2D *hdcos_rm = new TH2D( (*hdcos[0])/(*hdcos[1]) );
  hdcos_rm->SetName("hdcos_rm");
  
  cc3->cd(2);  hdcos_rm->Draw("colz");
  cc4->cd(2);  hdcos_rm->ProfileX("hdcos_rm_pfx",1,-1,"d");
}

void flatten(Double_t vn)
{

  Int_t   mtrack_b;

  vector<Int_t>    *pid=0;
  vector<Double_t> *deltphi_b=0;
  vector<Double_t> *rapid=0;
  TBranch *brapid;
  TBranch *bdeltphi_b;
  TBranch *bpid;

  Int_t    nproton;
  Double_t aveY[nbinx];
  Double_t avePhi[nbinx];
  Double_t aveCos[nbinx];


  for(Int_t m = 0; m < 2; m++){

    rChain[m]->SetBranchAddress("mtrack_b",&mtrack_b);
    rChain[m]->SetBranchAddress("deltphi_b",&deltphi_b,&bdeltphi_b);
    rChain[m]->SetBranchAddress("rapid",&rapid,&brapid);
    rChain[m]->SetBranchAddress("pid",&pid,&bpid);

    Int_t nevt = rChain[m]->GetEntries();  

    if(mtrack_b == 0) continue;
    bdeltphi_b->GetEntry(i);    
    brapid->GetEntry(i);    
    bpid->GetEntry(i);    
     
    vector< vector<Double_t> > vecY(nbinx);
    vector< vector<Double_t> > vecPhi(nbinx);
    vector< vector<Double_t> > vecCos(nbinx);
    
    //@@@@@@@@@@@@@@@@@@@@2

    



}




void SaveCanvas()
{
  if(cc0 != NULL) cc0->SaveAs(printHeader+"_0.png");
  if(cc1 != NULL) cc1->SaveAs(printHeader+"_1.png");
  if(cc3 != NULL) cc3->SaveAs(printHeader+"_3.png");
  if(cc4 != NULL) cc4->SaveAs(printHeader+"_4.png");

}
