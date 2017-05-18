#include "FourierCorrection.h"

TCanvas *cc0 = NULL;
TCanvas *cc1 = NULL;
TCanvas *cc2 = NULL;
TCanvas *cc3 = NULL;
TCanvas *cc4 = NULL;
const Int_t nbinx = 30;
TString printHeader;
TChain *rChain[2];
Double_t dxbin;


void calcRDvsMX()
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

  printHeader = "FlwRUN"+arun(1,4)+ Form("m%d",nrun) + "_v" + sVer; 
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



  TH1D *h2s_r[nbinx];
  TH1D *h2s_m[nbinx];
  TH1D *h2s_rm[nbinx];
  dxbin = 1./(Double_t)nbinx; 

  TH2D *h2_r;
  TH2D *h2_m;
  TH2D *h2_rm;

  TH1D *h1phimid_r;
  TH1D *h1phimid_m;
  TH1D *h1phimid_rm;

  TF1 *f1;




}


void SaveCanvas(TString fopt = "")
{
  if(cc0 != NULL) cc0->SaveAs(printHeader+fopt+"_0.png");
  if(cc1 != NULL) cc1->SaveAs(printHeader+fopt+"_1.png");
  if(cc3 != NULL) cc3->SaveAs(printHeader+fopt+"_3.png");
  if(cc4 != NULL) cc4->SaveAs(printHeader+fopt+"_4.png");

}

void vn(Double_t vn = 2)
{
  cc0 = new TCanvas("cc0", "cc0",700, 600);
  cc1 = new TCanvas("cc1", "cc1",700, 600);
  cc3 = new TCanvas("cc3", "cc3",700, 600);
  cc4 = new TCanvas("cc4", "cc4",700, 600);

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


void flatten_data(Int_t val = 4)
{
  const Int_t harm = val;

  Int_t    mtrack;
  TVector2 *unitP_lang=NULL;
  TBranch  *bunitP_lang;

  vector<Double_t> phi_lang;

 
  TH1D *hphi[2];
  TH1D *hcphi[2];
  TH2D *hccphi[2];
  TH2D *hbpxpy[2];
  TH2D *hapxpy[2];

  for(Int_t m = 0; m < 2; m++){
    
    hphi[m]  = new TH1D(Form("hphi%d",m),   "original Phi"   ,60.,-4.,4.);
    hcphi[m] = new TH1D(Form("hcphi%d",m),  "corrected Phi"  ,60.,-4.,4.);
    hccphi[m]= new TH2D(Form("hccphi%d",m), "bef aft correct",60.,-4.,4.,60.,-4.,4.);
    hbpxpy[m] = new TH2D(Form("hbpxpy%d",m),"bef px py       ",100,-1.2,1.2,100,-1.2,1.2);
    hapxpy[m] = new TH2D(Form("hapxpy%d",m),"afte px py      ",100,-1.2,1.2,100,-1.2,1.2);


    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("unitP_lang",&unitP_lang,&bunitP_lang);

    phi_lang.clear();
 
    //@@@@@@@@@@@@@@@@@@@@2
    Int_t nevt = rChain[m]->GetEntries();  

    cout << " Number of events " << nevt << endl;

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;

      Double_t pphi = TVector2::Phi_mpi_pi(unitP_lang->Phi());
      phi_lang.push_back(pphi);

      hbpxpy[m]->Fill(unitP_lang->Unit().X(),unitP_lang->Unit().Y());
    }
 
    vector<Double_t> cpphi = phi_lang;
 
    cout << " m " << m << " size " << phi_lang.size() << " / " << cpphi.size() << endl;;

    FourierCorrection(harm, cpphi);

    for(Int_t i = 0; i < phi_lang.size(); i++){
      hphi[m]->Fill(phi_lang.at(i));
      hcphi[m]->Fill(cpphi.at(i));
      
      hccphi[m]->Fill(phi_lang.at(i),cpphi.at(i));

      TVector2 avct(0.5*cos(cpphi.at(i)), 0.5*sin(cpphi.at(i)));
      hapxpy[m]->Fill(avct.X(), avct.Y());
      
      //      pcphi[m]->TH1D::Fill(cpphi.at(i));

    }

  }


  cc0 = new TCanvas("cc0", "cc0",1000, 700);
  cc1 = new TCanvas("cc1", "cc1",1000, 700);

  cc0->Divide(2,2);
  cc0->cd(1);  hphi[0]->Draw();
  cc0->cd(2);  
  hbpxpy[0]->Draw("colz"); 
  hapxpy[0]->Draw("samescolz");
  cc0->cd(3);  hcphi[0]->Draw();  
  cc0->cd(4);  hccphi[0]->Draw("colz");

  cc1->Divide(2,2);
  cc1->cd(1);  hphi[1]->Draw();
  cc1->cd(2);  
  hbpxpy[1]->Draw("colz"); 
  hapxpy[1]->Draw("samecolz");
  cc1->cd(3);  hcphi[1]->Draw();  
  cc1->cd(4);  hccphi[1]->Draw("colz");

}



void SubeventCorrelation()
{
  const Int_t harm = 4;

  Int_t    mtrack;
  Int_t    mtrack_1;
  Int_t    mtrack_2;
  TVector2 *unitP_lang=NULL;
  TBranch  *bunitP_lang;
  TVector2 *unitP_1=NULL;
  TBranch  *bunitP_1;
  TVector2 *unitP_2=NULL;
  TBranch  *bunitP_2;

  vector<Double_t> phi_lang;
  vector<Double_t> phi_1;
  vector<Double_t> phi_2;

  TH1D *hbphi[2];
  TH1D *haphi[2];
  TH2D *h2bphi[2];
  TH2D *h2aphi[2];


  for(Int_t m = 0; m < 2; m++){
    
    hbphi[m]  = new TH1D(Form("hbphi%d",m), "before correction; #Delta(#Phi_1 - #Phi_2) [rad] "   ,60.,-4.,4.);
    haphi[m]  = new TH1D(Form("haphi%d",m), "after correction; #Delta(#Phi_1 - #Phi_2) [rad]"   ,60.,-4.,4.);
    h2bphi[m] = new TH2D(Form("h2bphi%d",m),"before correction; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);
    h2aphi[m] = new TH2D(Form("h2aphi%d",m),"after correcttion; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);


    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("mtrack_1",&mtrack_1);
    rChain[m]->SetBranchAddress("mtrack_2",&mtrack_2);
    rChain[m]->SetBranchAddress("unitP_lang",&unitP_lang,&bunitP_lang);
    rChain[m]->SetBranchAddress("unitP_1"   ,&unitP_1,&bunitP_1);
    rChain[m]->SetBranchAddress("unitP_2"   ,&unitP_2,&bunitP_2);

    phi_lang.clear();
    phi_1.clear();
    phi_2.clear();
 
    //@@@@@@@@@@@@@@@@@@@@2
    Int_t nevt = rChain[m]->GetEntries();  

    cout << " Number of events " << nevt << endl;

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      //      cout << mtrack_1 << " " << mtrack_2 << endl;
      if(mtrack_1 == 0 || mtrack_2 == 0) continue;

      Double_t pphi1 = TVector2::Phi_mpi_pi(unitP_1->Phi());
      phi_1.push_back(pphi1);

      Double_t pphi2 = TVector2::Phi_mpi_pi(unitP_2->Phi());
      phi_2.push_back(pphi2);

    }

    if(phi_1.size() == 0) return;


    vector<Double_t> bphi_1 = phi_1;
    vector<Double_t> bphi_2 = phi_2;

    cout << "Fourier correction  " << m << endl; 
    FourierCorrection(harm, phi_1);

    FourierCorrection(harm, phi_2);
      
    
    for(Int_t i = 0; i < phi_1.size(); i++){
      h2bphi[m]->Fill(bphi_1.at(i),TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      h2aphi[m]->Fill( phi_1.at(i),TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));
      hbphi[m]->Fill(TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      haphi[m]->Fill(TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));
    }
  }

  cc0 = new TCanvas("cc0", "cc0 REAL",1000, 700);

  cc0->Divide(2,2);
  cc0->cd(1);
  h2bphi[0]->Draw("colz");
  cc0->cd(2);
  hbphi[0]->Draw();

  cc0->cd(3);
  h2aphi[0]->Draw("colz");
  cc0->cd(4);
  haphi[0]->Draw();


  cc1 = new TCanvas("cc1", "cc1 MIXed",1000, 700);

  cc1->Divide(2,2);
  cc1->cd(1);
  h2bphi[1]->Draw("colz");
  cc1->cd(2);
  hbphi[1]->Draw();

  cc1->cd(3);
  h2aphi[1]->Draw("colz");
  cc1->cd(4);
  haphi[1]->Draw();
}

