//#include "FourierCorrection.h"
#include "STFlowCorrection.hh"

TCanvas *cc0 = NULL;
TCanvas *cc1 = NULL;
TCanvas *cc2 = NULL;
TCanvas *cc3 = NULL;
TCanvas *cc4 = NULL;
TCanvas *cc5 = NULL;
TCanvas *cc6 = NULL;

const Int_t nbinx = 30;
UInt_t  fVer = 0;

TString printHeader;
TChain *rChain[2];
Double_t dxbin;

TH2D *h2_r;
TH2D *h2_m;
TH1D *h2s_r[nbinx];
TH1D *h2s_m[nbinx];

TH2D *h2cos_r;
TH2D *h2cos_m;
TH1D *h2coss_r[nbinx];
TH1D *h2coss_m[nbinx];

TH2D *h2cos2_r;
TH2D *h2cos2_m;
TH1D *h2cos2s_r[nbinx];
TH1D *h2cos2s_m[nbinx];

Int_t    mtrack;
Int_t    mtrack_1;
Int_t    mtrack_2;

vector<Double_t> *iphi=0;
vector<Double_t> *rpphi=0;
vector<Double_t> *rapid=0;
vector<Double_t> *prapid=0;
vector<Int_t>    *pid=0;
vector<Double_t> *pz=0;
vector<Double_t> *px=0;
vector<Double_t> *py=0;
vector<Double_t> *deltphi=0;

TVector2 *unitP  =NULL;
TVector2 *unitP_1=NULL;
TVector2 *unitP_2=NULL;

TBranch  *bunitP;
TBranch  *bunitP_1;
TBranch  *bunitP_2;
TBranch *brpphi=0;
TBranch *biphi=0;
TBranch *brapid=0;
TBranch *bprapid=0;
TBranch *bpid=0;
TBranch *bpz=0;
TBranch *bpx=0;
TBranch *bpy=0;
TBranch *bdeltphi=0;


void calcRDvsMX();
void Slice2D(Int_t nval=30);
void v1(Int_t nval=30);
void v2(Int_t nval=30);

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

  fVer = atoi(ssVer);

  cout << "fVer " << fVer << " ssVer " << ssVer << " tVer " << tVer << endl;


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



  dxbin = 1./(Double_t)nbinx; 

  TH1D *h1phimid_r;
  TH1D *h1phimid_m;
  TH1D *h1phimid_rm;

  TF1 *f1;



}


void SaveCanvas(TString fopt = "")
{
  Int_t iCanvas = gROOT->GetListOfCanvases()->GetEntries();  
  for(Int_t i = 0; i < iCanvas; i++)
    gROOT->GetListOfCanvases()->At(i)->SaveAs(printHeader+fopt+Form("_%d",i)+".png");
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

  hdphi[0] = new TH2D("hdphi0","ndphi "    ,nbinx,yrang[0],yrang[1],60,0.,3.2);
  hdcos[0] = new TH2D("hdcos0","cos(nphi) ",nbinx,yrang[0],yrang[1],60,-1.,1.);
  hdphi[1] = new TH2D("hdphi1","ndphi "    ,nbinx,yrang[0],yrang[1],60,0.,3.2);
  hdcos[1] = new TH2D("hdcos1","cos(nphi) ",nbinx,yrang[0],yrang[1],60,-1.,1.);

  
  Int_t    nproton;
  Double_t aveY[nbinx];
  Double_t avePhi[nbinx];
  Double_t aveCos[nbinx];



  for(Int_t m = 0; m < 2; m++){

    rChain[m]->SetBranchAddress("mtrack"    ,&mtrack);
    rChain[m]->SetBranchAddress("rapid"     ,&rapid,&brapid);
    rChain[m]->SetBranchAddress("pid"       ,&pid,&bpid);
    rChain[m]->SetBranchAddress("unitP"     ,&unitP,&bunitP);
    rChain[m]->SetBranchAddress("rpphi"     ,&rpphi ,&brpphi);
    rChain[m]->SetBranchAddress("prapid"    ,&prapid,&bprapid);
    rChain[m]->SetBranchAddress("deltphi"   ,&deltphi,&bdeltphi);
    

    Int_t nevt = rChain[m]->GetEntries();  

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;
  
      vector< vector<Double_t> > vecY(nbinx);
      vector< vector<Double_t> > vecPhi(nbinx);
      vector< vector<Double_t> > vecCos(nbinx);

      Int_t nproton = 0;
      for(Int_t j = 0; j < deltphi->size() ; j++){

	//@@@@@
	if( pid->at(j) == 12212 && rapid->at(j) < 1.0 ){
	
	  Int_t ybin = (Int_t)(rapid->at(j)/dxbin);
	  

	  vecY[ybin]  .push_back(rapid->at(j));

	  Double_t dblphi = abs(TVector2::Phi_mpi_pi(vn*deltphi->at(j)));
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

void flatten_data(Int_t val = 4, Bool_t bout = 0)
{

  const Int_t harm = val;

  Int_t    mtrack;
  TVector2 *unitP=NULL;
  TBranch  *bunitP;

  vector<Double_t> phi_lang;

 
  TH1D *hphi[2];
  TH1D *hcphi[2];
  TH2D *hccphi[2];
  TH2D *hbpxpy[2];
  TH2D *hapxpy[2];



  for(Int_t m = 0; m < 2; m++){

    STFlowCorrection *flowcorr = new STFlowCorrection(rChain[m]->GetListOfFiles(), harm,m);

    hphi[m]   = new TH1D(Form("hphi%d",m),   "original Phi"   ,60.,-4.,4.);
    hcphi[m]  = new TH1D(Form("hcphi%d",m),  "corrected Phi"  ,60.,-4.,4.);
    hccphi[m] = new TH2D(Form("hccphi%d",m), "bef aft correct",60.,-4.,4.,60.,-4.,4.);
    hbpxpy[m] = new TH2D(Form("hbpxpy%d",m),"bef px py       ",100,-1.2,1.2,100,-1.2,1.2);
    hapxpy[m] = new TH2D(Form("hapxpy%d",m),"afte px py      ",100,-1.2,1.2,100,-1.2,1.2);


    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("unitP",&unitP,&bunitP);

    phi_lang.clear();
 
    //@@@@@@@@@@@@@@@@@@@@2
    Int_t nevt = rChain[m]->GetEntries();  

    cout << " Number of events " << nevt << endl;

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;

      Double_t pphi = TVector2::Phi_mpi_pi(unitP->Phi());
      phi_lang.push_back(pphi);

      hbpxpy[m]->Fill(unitP->Unit().X(),unitP->Unit().Y());
    }
 
    vector<Double_t> cpphi = phi_lang;
 
    cout << " m " << m << " size " << phi_lang.size() << " / " << cpphi.size() << endl;;

    
    Double_t Bn[harm];
    Double_t An[harm];
    Double_t Bn_rms[harm];
    Double_t An_rms[harm];

    flowcorr->FourierCorrection(cpphi);

    if(bout) flowcorr->SaveCorrectionFactor("phi_lang: flatten_data()");


    for(Int_t i = 0; i < phi_lang.size(); i++){
      hphi[m]->Fill(phi_lang.at(i));
      hcphi[m]->Fill(cpphi.at(i));
      
      hccphi[m]->Fill(phi_lang.at(i),cpphi.at(i));
      
      TVector2 avct(0.5*cos(cpphi.at(i)), 0.5*sin(cpphi.at(i)));
      hapxpy[m]->Fill(avct.X(), avct.Y());
      
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



void SubeventCorrelation(UInt_t nfl=0)
{
  const Int_t harm = 4;

  Int_t    mtrack;
  Int_t    mtrack_1;
  Int_t    mtrack_2;
  TVector2 *unitP=NULL;
  TBranch  *bunitP;
  TVector2 *unitP_1=NULL;
  TBranch  *bunitP_1;
  TVector2 *unitP_2=NULL;
  TBranch  *bunitP_2;

  vector<Double_t> phi_lang;
  vector<Double_t> phi_1;
  vector<Double_t> phi_2;
  vector<Double_t> phi_1s2a;
  vector<Double_t> phi_1s2b;

  TH1D *hbphi[2];
  TH1D *haphi[2];
  TH2D *h2bphi[2];
  TH2D *h2aphi[2];
  TH1D *hbcos[2];
  TH1D *hacos[2];


  for(Int_t m = 0; m < 2; m++){

    STFlowCorrection *flowcorr = new STFlowCorrection(rChain[m]->GetListOfFiles(), harm, m);
    
    hbphi[m]  = new TH1D(Form("hbphi%d",m), "before correction; #Delta(#Phi_1 - #Phi_2) [rad] "   ,60.,-4.,4.);
    haphi[m]  = new TH1D(Form("haphi%d",m), "after correction; #Delta(#Phi_1 - #Phi_2) [rad]"   ,60.,-4.,4.);
    h2bphi[m] = new TH2D(Form("h2bphi%d",m),"before correction; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);
    h2aphi[m] = new TH2D(Form("h2aphi%d",m),"after correcttion; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);
    
    hbcos[m]  = new TH1D(Form("hbcos%d",m), "before correction; cos(#Delta#Phi) "   ,100,-1.,1.);
    hacos[m]  = new TH1D(Form("hacos%d",m), "after  correction; cos(#Delta#Phi) "   ,100,-1.,1.);


    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("mtrack_1",&mtrack_1);
    rChain[m]->SetBranchAddress("mtrack_2",&mtrack_2);
    rChain[m]->SetBranchAddress("unitP",&unitP,&bunitP);
    rChain[m]->SetBranchAddress("unitP_1"   ,&unitP_1,&bunitP_1);
    rChain[m]->SetBranchAddress("unitP_2"   ,&unitP_2,&bunitP_2);

    phi_lang.clear();
    phi_1.clear();
    phi_2.clear();
    phi_1s2a.clear();
    phi_1s2b.clear();
 
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

    if(nfl == 1){
      cout << "Fourier correction  will be done. " << m << endl; 
      flowcorr->FourierCorrection(phi_1);    
      flowcorr->FourierCorrection(phi_2);
    }


    for(Int_t i = 0; i < phi_1.size(); i++){
      h2bphi[m]->Fill(bphi_1.at(i),TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      h2aphi[m]->Fill( phi_1.at(i),TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));
      hbphi[m]->Fill(TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      haphi[m]->Fill(TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));
      
      hbcos[m]->Fill(cos(bphi_1.at(i)-bphi_2.at(i)));
      hacos[m]->Fill(cos( phi_1.at(i)- phi_2.at(i)));

      phi_1s2b.push_back(TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      phi_1s2a.push_back(TVector2::Phi_mpi_pi(phi_1.at(i)-phi_2.at(i)));
    }

    // results    
    Double_t *vcos;
    vcos = flowcorr->GetAverageCosin(1, phi_1s2b);
    cout << m << "-> before  vcos[0] " << vcos[0] << "  rms " << vcos[1] << endl;    

    vcos = flowcorr->GetAverageCosin(1, phi_1s2a);
    cout << m << "-> after   vcos[0] " << vcos[0] << "  rms " << vcos[1] << endl;    

    Double_t scale = hbphi[m]->Integral();
    hbphi[m]->Scale(1./scale);
    scale = haphi[m]->Integral();
    haphi[m]->Scale(1./scale);
  }




  //  TH2D *h2_rm = new TH2D( (*h2_r)/(*h2_m) );

  TH1D *hbphi_sub = new TH1D( (*hbphi[0])-(*hbphi[1]) );
  TH1D *hbphi_div = new TH1D( (*hbphi[0])/(*hbphi[1]) );

  TH1D *haphi_sub = new TH1D( (*haphi[0])-(*haphi[1]) );
  TH1D *haphi_div = new TH1D( (*haphi[0])/(*haphi[1]) );

  hbphi_sub->SetName("hbphi_sub");  hbphi_sub->SetTitle("before REAL - MIXed");
  hbphi_div->SetName("hbphi_div");  hbphi_div->SetTitle("before REAL / MIXed");
  haphi_sub->SetName("haphi_sub");  haphi_sub->SetTitle("after  REAL - MIXed");
  haphi_div->SetName("haphi_div");  haphi_div->SetTitle("after  REAL / MIXed");

  TF1 *f1 = new TF1("f1","[0]+[1]*cos(x)",-3.2,3.2);

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

  cc2 = new TCanvas("cc2", "cc2 div and sub",1000, 700);
  gStyle->SetOptFit(1111);
  cc2->Divide(2,2);
  cc2->cd(1);
  hbphi_sub->Fit("f1");
  cc2->cd(2);
  hbphi_div->Fit("f1");
  cc2->cd(3);
  haphi_sub->Fit("f1");
  cc2->cd(4);
  haphi_div->Fit("f1");


  cc3 = new TCanvas("cc3", "cc3",1200, 700);
  cc3->Divide(2,3);
  cc3->cd(1);
  hacos[0]->Draw();
  cc3->cd(2);
  hacos[1]->Draw();

  cc3->cd(3);
  TH1D *hacos_sub = new TH1D( (*hacos[0])-(*hacos[1]) );
  hacos_sub->SetName("hacos_sub"); 
  hacos_sub->SetTitle("after REAL - MIXED");
  hacos_sub->Draw();

  cc3->cd(4);
  TH1D *hacos_div = new TH1D( (*hacos[0])/(*hacos[1]) );
  hacos_div->SetName("hacos_div"); 
  hacos_div->SetTitle("after REAL / MIXED");
  hacos_div->Draw();

  cc3->cd(5);
  TH1D *hbcos_sub = new TH1D( (*hbcos[0])-(*hbcos[1]) );
  hbcos_sub->SetName("hbcos_sub"); 
  hbcos_sub->SetTitle("before REAL - MIXED");
  hbcos_sub->Draw();
  
  cc3->cd(6);
  TH1D *hbcos_div = new TH1D( (*hbcos[0])/(*hbcos[1]) );
  hbcos_div->SetName("hbcos_div"); 
  hbcos_div->SetTitle("before REAL / MIXED");
  hbcos_div->Draw();
  
  //
  cout << " < cos > " << endl;
  cout << " before REAL       " << hbcos[0]->GetMean()
       << " before MIXed      " << hbcos[1]->GetMean()
       << endl;
  cout << " after  REAL       " << hacos[0]->GetMean()
       << " after  MIXed      " << hacos[1]->GetMean()
       << endl;
  cout << " before REAL/MIXED " << hbcos_div->GetMean()
       << " before REAL-MIXED " << hbcos_sub->GetMean()
       << endl;
  cout << " after  REAL/MIXED " << hacos_div->GetMean()
       << " after  REAL-MIXED " << hacos_sub->GetMean()
       << endl;


}


void RPResolution()
{
  //  Int_t harm = flowcorr->GetNHarmonics();

  vector<Double_t> phi_1;
  vector<Double_t> phi_2;
  vector<Double_t> phi_1s2a;
  vector<Double_t> phi_1s2b;

  TH1D *hbphi[2];
  TH1D *haphi[2];
  TH2D *h2bphi[2];
  TH2D *h2aphi[2];

  for(Int_t m = 0; m < 2; m++){

    STFlowCorrection *flowcorr = new STFlowCorrection(4, m);

    
    hbphi[m]  = new TH1D(Form("hbphi%d",m), "before correction; #Delta(#Phi_1 - #Phi_2) [rad] "   ,60.,-4.,4.);
    haphi[m]  = new TH1D(Form("haphi%d",m), "after correction; #Delta(#Phi_1 - #Phi_2) [rad]"   ,60.,-4.,4.);
    h2bphi[m] = new TH2D(Form("h2bphi%d",m),"before correction; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);
    h2aphi[m] = new TH2D(Form("h2aphi%d",m),"after correcttion; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);


    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("mtrack_1",&mtrack_1);
    rChain[m]->SetBranchAddress("mtrack_2",&mtrack_2);
    rChain[m]->SetBranchAddress("unitP",     &unitP,&bunitP);
    rChain[m]->SetBranchAddress("unitP_1"   ,&unitP_1,&bunitP_1);
    rChain[m]->SetBranchAddress("unitP_2"   ,&unitP_2,&bunitP_2);

    phi_1.clear();
    phi_2.clear();
    phi_1s2a.clear();
    phi_1s2b.clear();
 
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

    // flowcorr->GetCorrectionFactor();
    // flowcorr->GetCorrection(phi_1);

    // flowcorr->GetCorrectionFactor();
    // flowcorr->GetCorrection(phi_2);

    // flowcorr->FourierCorrection(phi_1);
    // flowcorr->FourierCorrection(phi_2);

    
    for(Int_t i = 0; i < phi_1.size(); i++){


      h2bphi[m]->Fill(bphi_1.at(i),TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      h2aphi[m]->Fill( phi_1.at(i),TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));
      hbphi[m]->Fill(TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      haphi[m]->Fill(TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));

      phi_1s2b.push_back(TVector2::Phi_mpi_pi(bphi_1.at(i)-bphi_2.at(i)));
      phi_1s2a.push_back(TVector2::Phi_mpi_pi(phi_1.at(i)-phi_2.at(i)));
    }
    
    Double_t *vcos;
    vcos = flowcorr->GetAverageCosin(1, phi_1s2b);
    cout << m << "-> before  vcos[0] " << vcos[0] << "  rms " << vcos[1] << endl;    

    vcos = flowcorr->GetAverageCosin(1, phi_1s2a);
    cout << m << "-> after   vcos[0] " << vcos[0] << "  rms " << vcos[1] << endl;    

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


void flatten_iphi(Int_t val = 20, UInt_t iflt = 0)
{

  const Int_t harm = val;

  TH1D *haiphi[2];
  TH1D *harphi[2];
  TH1D *hadphi[2];
  TH2D *haydphi_prt[2];
  TH2D *haydphi_frg[2];
  TH2D *haycos_prt[2];
  TH2D *haycos_frg[2];
  TH2D *haycos2_prt[2];
  TH2D *haycos2_frg[2];


  vector<Double_t> phi_i;
  vector<Double_t> phi_r;
  vector<Double_t> rapid_i;
  vector<Double_t> prapid_i;
  vector<Int_t>    pid_i;

  for(Int_t m = 0; m < 2; m++){


    STFlowCorrection *flowcorr = new STFlowCorrection(rChain[m]->GetListOfFiles(), harm, m);
    //STFlowCorrection *flowcorr = new STFlowCorrection(harm, m);


    
    haiphi[m] = new TH1D(Form("haiphi%d",m), "after iphi"        , 400,-4.,4.);
    harphi[m] = new TH1D(Form("harphi%d",m), "after rpphi"       , 400,-4.,4.);
    hadphi[m] = new TH1D(Form("hadphi%d",m), "after iphi - rpphi", 400,-4.,4.);

    haydphi_prt[m] = new TH2D(Form("haydphi_prt%d",m), "dphi vs rapid proton"  , 100,0.,1.,400,-3.2, 3.2);
    haydphi_frg[m] = new TH2D(Form("haydphi_frg%d",m), "dphi vs rapid fragment",  90,0.,3.,400,-3.2, 3.2);

    haycos_prt[m]  = new TH2D(Form("haycos_prt%d",m), "cos vs rapid proton"  ,  30,0.,1.,100,-1.,1.);
    haycos_frg[m]  = new TH2D(Form("haycos_frg%d",m), "cos vs rapid fragment",  30,0.,3.,100,-1.,1.);

    haycos2_prt[m]  = new TH2D(Form("haycos2_prt%d",m), "cos2 vs rapid proton"  ,  30,0.,1.,100,-1.,1.);
    haycos2_frg[m]  = new TH2D(Form("haycos2_frg%d",m), "cos2 vs rapid fragment",  30,0.,3.,100,-1.,1.);


    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("rpphi" ,&rpphi ,&brpphi);
    rChain[m]->SetBranchAddress("iphi"  ,&iphi  ,&biphi);
    rChain[m]->SetBranchAddress("rapid" ,&rapid ,&brapid);
    rChain[m]->SetBranchAddress("prapid",&prapid,&bprapid);
    rChain[m]->SetBranchAddress("pid"   ,&pid   ,&bpid);
    
    Int_t nevt = rChain[m]->GetEntries();

    cout << " Number of events " << nevt << endl;

    phi_i.clear();
    phi_r.clear();
    rapid_i.clear();
    prapid_i.clear();
    pid_i.clear();

    Int_t icout = 0;
    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;
      
      icout++;

      for(Int_t j = 0; j < (Int_t)rpphi->size(); j++){
	//	cout << i << " -> " << mtrack << " " << iphi->at(j) << endl;

	phi_i.push_back(iphi->at(j));
	phi_r.push_back(rpphi->at(j));
	rapid_i.push_back(rapid->at(j));
	prapid_i.push_back(prapid->at(j));
	pid_i.push_back(pid->at(j));

      }
    }

    if(fVer < 210 ) { 
      if(iflt == 1){
	flowcorr->FourierCorrection( phi_i );
	flowcorr->SaveCorrectionFactor(Form("%dniphi:flatten_iphi",harm));
	flowcorr->FourierCorrection( phi_r );
      }
    }

    TAxis *xaxis    = (TAxis*)haydphi_prt[m]->GetXaxis();
    TAxis *yaxis    = (TAxis*)haydphi_prt[m]->GetYaxis();
    Double_t nfactX = (Double_t)xaxis->GetNbins()/(xaxis->GetXmax() - xaxis->GetXmin());
    Double_t nfactY = (Double_t)yaxis->GetNbins()/(yaxis->GetXmax() - yaxis->GetXmin()) / 2.*TMath::Pi();
    Double_t nfact  = nfactX * nfactY / (Double_t)icout;


    cout << " X " << nfactX << " Y " << nfactY << " icout " << icout << " -> " << nfact << endl;


    for(Int_t j = 0; j < (Int_t)phi_i.size(); j++){
      haiphi[m]->Fill( phi_i.at(j) );
      harphi[m]->Fill( phi_r.at(j) );

      hadphi[m]->Fill( TVector2::Phi_mpi_pi( phi_i.at(j) - phi_r.at(j) ));

      if(pid_i.at(j) == 12212){
	haydphi_prt[m]->Fill( rapid_i.at(j), TVector2::Phi_mpi_pi( phi_i.at(j) - phi_r.at(j)) , nfact );
	haycos_prt[m] ->Fill( rapid_i.at(j), cos( (phi_i.at(j) - phi_r.at(j)) ), nfact );
	haycos2_prt[m]->Fill( rapid_i.at(j), cos( 2.*( phi_i.at(j) - phi_r.at(j)) ), nfact );
      }
      else if(pid_i.at(j) > 2212) {
	haydphi_frg[m]->Fill( prapid_i.at(j), TVector2::Phi_mpi_pi( phi_i.at(j) - phi_r.at(j))  );
	haycos_frg[m] ->Fill( prapid_i.at(j), cos( ( phi_i.at(j) - phi_r.at(j)) ));	
	haycos2_frg[m]->Fill( prapid_i.at(j), cos( 2.*( phi_i.at(j) - phi_r.at(j)) ));	
      }
    }

    delete flowcorr;

  }

  // h2_r = new TH2D((*haydphi_prt[0]));
  // h2_m = new TH2D((*haydphi_prt[1]));
  h2_r = (TH2D*)haydphi_prt[0]->RebinX(3,"h2_r");
  h2_r = (TH2D*)h2_r->RebinY(6,"h2_r");
  h2_m = (TH2D*)haydphi_prt[1]->RebinX(3,"h2_m");
  h2_m = (TH2D*)h2_m->RebinY(10,"h2_m");
  Slice2D(30);


  h2cos_r = new TH2D( (*haycos_prt[0]) );
  h2cos_m = new TH2D( (*haycos_prt[1]) );
  v1(30);

  h2cos2_r = new TH2D( (*haycos2_prt[0]) );
  h2cos2_m = new TH2D( (*haycos2_prt[1]) );
  v2(30);

  
  cc0 = new TCanvas("cc0","cc0",700,800);

  cc0->Divide(2,3);
  Int_t iv = 1;
  
  cc0->cd(iv); iv++;
  haiphi[0]->Draw();

  cc0->cd(iv); iv++;
  haiphi[1]->Draw();

  cc0->cd(iv); iv++;
  harphi[0]->Draw();

  cc0->cd(iv); iv++;
  harphi[1]->Draw();

  cc0->cd(iv); iv++;
  hadphi[0]->Draw();

  cc0->cd(iv); iv=1;
  hadphi[1]->Draw();

  cc1 = new TCanvas("cc1","cc1",700,500);
  cc1->Divide(2,2);

  cc1->cd(iv); iv++;
  haydphi_prt[0]->Draw("colz");

  cc1->cd(iv); iv++;
  haydphi_prt[1]->Draw("colz");

  cc1->cd(iv); iv++;
  haydphi_frg[0]->Draw("colz");

  cc1->cd(iv); iv=1;
  haydphi_frg[1]->Draw("colz");

  
}

void Slice2D(Int_t nval)
{
  
  cc3 = new TCanvas("cc3", "cc3 Slice",1200, 1000);
  if(nval == 30) cc3->Divide(6,5);
  else cc3->Divide(5,4);

  cc4 = new TCanvas("cc4", "cc4 Slice",1200, 1000);
  if(nval == 30) cc4->Divide(6,5);
  else cc4->Divide(5,4);
  

  Int_t nbin = h2_r->GetXaxis()->GetNbins();
  Int_t binwidth = nbin/nval;
  cout << "bin width = " << binwidth << endl;

  Int_t iv = 1;
  for(Int_t k = 0; k < std::min(nval,nbinx); k++){

    Int_t lbin = k*binwidth;
    h2s_r[k] = new TH1D(*(h2_r->ProjectionY("",lbin,lbin+binwidth)) );
    h2s_m[k] = new TH1D(*(h2_m->ProjectionY("",lbin,lbin+binwidth)) );
    
    h2s_r[k]->SetName(Form("h2s_r%d",k));
    h2s_m[k]->SetName(Form("h2s_m%d",k));
    Double_t ycent = h2_r->GetXaxis()->GetBinUpEdge(lbin);
    h2s_r[k]->SetTitle(Form("Rapidity %f",ycent));    
    h2s_m[k]->SetTitle(Form("Rapidity %f",ycent));    

    h2s_r[k]->SetDirectory(gROOT);
    h2s_m[k]->SetDirectory(gROOT);

    h2s_r[k]->SetLineColor(4);
    h2s_m[k]->SetLineColor(2);
  
    cc3->cd(iv); 
    h2s_r[k]->Draw();
    cc4->cd(iv); iv++;
    h2s_m[k]->Draw();

  }
}

void v1(Int_t nval)
{
  
  // cc5 = new TCanvas("cc5", "cc5 Slice",1200, 1000);
  // if(nval == 30) cc5->Divide(6,5);
  // else cc5->Divide(5,4);

  // cc6 = new TCanvas("cc6", "cc6 Slice",1200, 1000);
  // if(nval == 30) cc6->Divide(6,5);
  // else cc6->Divide(5,4);

  Int_t nbin = h2cos_r->GetXaxis()->GetNbins();
  Int_t binwidth = nbin/nval;
  cout << "bin width = " << binwidth << endl;

  Double_t xval[nval];
  Double_t yval_r[nval];
  Double_t yval_m[nval];


  Int_t iv = 1;
  for(Int_t k = 0; k < std::min(nval,nbinx); k++){

    Int_t lbin = k*binwidth;
    h2coss_r[k] = new TH1D(*(h2cos_r->ProjectionY("",lbin,lbin+binwidth)) );
    h2coss_m[k] = new TH1D(*(h2cos_m->ProjectionY("",lbin,lbin+binwidth)) );
    
    h2coss_r[k]->SetName(Form("h2coss_r%d",k));
    h2coss_m[k]->SetName(Form("h2coss_m%d",k));
    Double_t ycent = h2cos_r->GetXaxis()->GetBinUpEdge(lbin);
    h2coss_r[k]->SetTitle(Form("Rapidity %f",ycent));    
    h2coss_m[k]->SetTitle(Form("Rapidity %f",ycent));    

    xval[k] = ycent;

    h2coss_r[k]->SetLineColor(4);
    h2coss_m[k]->SetLineColor(2);
  
    // cc5->cd(iv); 
    // h2coss_r[k]->Draw();
    // cc6->cd(iv); iv++;
    // h2coss_m[k]->Draw();

    yval_r[k] = h2coss_r[k]->GetMean();
    yval_m[k] = h2coss_m[k]->GetMean();
    
  }

  cc7 = new TCanvas("cc7","cc7",700,500);

  TGraph *gv_r = new TGraph(nval, xval, yval_r);
  TGraph *gv_m = new TGraph(nval, xval, yval_m);
  
  gv_r->SetName("gv_r");
  gv_m->SetName("gv_m");

  gv_r->SetLineColor(4);
  gv_r->SetMarkerStyle(20);
  gv_r->SetMarkerColor(4);

  gv_m->SetLineColor(2);
  gv_m->SetMarkerStyle(20);
  gv_m->SetMarkerColor(2);


  auto *mg = new TMultiGraph();
  mg->SetTitle("Proton v1; Rapidity; v1");

  mg->Add(gv_r);
  mg->Add(gv_m);
  mg->Draw("ALP");  
}

void v2(Int_t nval)
{
  
  cc8 = new TCanvas("cc8", "cc8 Slice",1200, 1000);
  if(nval == 30) cc8->Divide(6,5);
  else cc8->Divide(5,4);

  cc9 = new TCanvas("cc9", "cc9 Slice",1200, 1000);
  if(nval == 30) cc9->Divide(6,5);
  else cc9->Divide(5,4);

  Int_t nbin = h2cos_r->GetXaxis()->GetNbins();
  Int_t binwidth = nbin/nval;
  cout << "bin width = " << binwidth << endl;

  Double_t xval[nval];
  Double_t yval_r[nval];
  Double_t yval_m[nval];


  Int_t iv = 1;
  for(Int_t k = 0; k < std::min(nval,nbinx); k++){

    Int_t lbin = k*binwidth;
    h2cos2s_r[k] = new TH1D(*(h2cos2_r->ProjectionY("",lbin,lbin+binwidth)) );
    h2cos2s_m[k] = new TH1D(*(h2cos2_m->ProjectionY("",lbin,lbin+binwidth)) );
    
    h2cos2s_r[k]->SetName(Form("h2cos2s_r%d",k));
    h2cos2s_m[k]->SetName(Form("h2cos2s_m%d",k));
    Double_t ycent = h2cos2_r->GetXaxis()->GetBinUpEdge(lbin);
    h2cos2s_r[k]->SetTitle(Form("Rapidity %f",ycent));    
    h2cos2s_m[k]->SetTitle(Form("Rapidity %f",ycent));    

    xval[k] = ycent;

    h2cos2s_r[k]->SetLineColor(4);
    h2cos2s_m[k]->SetLineColor(2);
  
    cc8->cd(iv); 
    h2cos2s_r[k]->Draw();
    cc9->cd(iv); iv++;
    h2cos2s_m[k]->Draw();

    yval_r[k] = h2cos2s_r[k]->GetMean();
    yval_m[k] = h2cos2s_m[k]->GetMean();
    
  }

  cc10 = new TCanvas("cc10","cc10",700,500);

  TGraph *gv2_r = new TGraph(nval, xval, yval_r);
  TGraph *gv2_m = new TGraph(nval, xval, yval_m);
  gv2_r->SetName("gv2_r");
  gv2_m->SetName("gv2_m");

  gv2_r->SetLineColor(4);
  gv2_r->SetMarkerStyle(20);
  gv2_r->SetMarkerColor(4);

  gv2_m->SetLineColor(2);
  gv2_m->SetMarkerStyle(20);
  gv2_m->SetMarkerColor(2);

  auto *mg = new TMultiGraph();
  mg->SetTitle("Proton v2; Rapidity; v2");
  mg->Add(gv2_r);
  mg->Add(gv2_m);

  mg->Draw("ALP");
  
}


void iphi_flat()
{
  /* 
     
   */

  
  TH2D *hbaiphi[2];
  TH1D *hbiphi[2];
  TH1D *haiphi[2];
  TH2D *hbpziphi[2];
  TH2D *hapziphi[2];

  for(Int_t m = 0; m < 2; m++){
    STFlowCorrection *flowcorr = new STFlowCorrection();    

    TString fname;
    if(m == 1)
      fname = "cfMixrun2900_mxflw_v2.0.10.20niphi.txt";
    else
      fname = "cfRealrun2900_rdflw_v2.0.10.20niphi.txt";
    flowcorr->SetRealOrMix(m);


    // UInt_t sel = 1;
    // if(sel){
    //   fname = "cfRealrun3027_rdflw_v2.0.10.20niphi.txt";
    //   flowcorr->SetRealOrMix(0);
    // }
    // else {
    //   fname = "cfMixrun3027_mxflw_v2.0.10.20niphi.txt";
    //   flowcorr->SetRealOrMix(1);
    // }


    flowcorr->SetFileName(fname);
    UInt_t iget =  flowcorr->GetCorrectionFactor();


    hbaiphi[m] = new TH2D(Form("hbaiphi%d",m),  " #phi_{i} before and after; before #phi_{i} [rad]; after #phi_{i} [rad] ", 400,-3.5,3.5,400,-3.5,3.5);
    hbiphi[m]  = new TH1D(Form("hbiphi%d",m),   " #phi_{i} before; Azimuthal angle [rad]"           , 400,-3.5,3.5);
    haiphi[m]  = new TH1D(Form("haiphi%d",m),   " #phi_{i} after ; Azimuthal angle [rad]"           , 400,-3.5,3.5);
    hbpziphi[m]= new TH2D(Form("hbpziphi%d",m), " before ; pz; #phi_{i};  "           , 200,0.,1200., 400,-3.5,3.5); 
    hapziphi[m]= new TH2D(Form("hapziphi%d",m), " after  ; pz; #phi_{i};  "           , 200,0.,1200., 400,-3.5,3.5); 

    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("iphi"  ,&iphi  ,&biphi);
    rChain[m]->SetBranchAddress("pid"   ,&pid   ,&bpid);
    rChain[m]->SetBranchAddress("pz"    ,&pz    ,&bpz);

    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;



    Int_t icout = 0;
    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;

      icout++;

      for(Int_t j = 0; j < (Int_t)iphi->size(); j++){

	Double_t a_iphi = flowcorr->GetCorrection(iphi->at(j));

	hbaiphi[m] -> Fill(iphi->at(j), a_iphi);
	hbiphi[m]  -> Fill(iphi->at(j));
	haiphi[m]  -> Fill(a_iphi);
	hbpziphi[m]-> Fill(pz->at(j), iphi->at(j));
	hapziphi[m]-> Fill(pz->at(j), a_iphi);
      }
    }

    delete flowcorr;
  }


    
  cc0 = new TCanvas("cc0","cc0",700,800);
  cc0->Divide(2,3);

  UInt_t iv = 1;

  cc0->cd(iv); iv++;
  hbiphi[0]->Draw();

  cc0->cd(iv); iv++;
  hbiphi[1]->Draw();

  cc0->cd(iv); iv++;
  haiphi[0]->Draw();

  cc0->cd(iv); iv++;
  haiphi[1]->Draw();

  cc0->cd(iv); iv++;
  hbaiphi[0]->Draw();

  cc0->cd(iv); iv=1;
  hbaiphi[1]->Draw();
  
  cc0->cd(1);

  cc1 = new TCanvas("cc1","cc1",700,600);
  cc1->Divide(2,2);

  cc1->cd(iv); iv++;
  hbpziphi[0]->Draw("colz");

  cc1->cd(iv); iv++;
  hbpziphi[1]->Draw("colz");

  cc1->cd(iv); iv++;
  hapziphi[0]->Draw("colz");

  cc1->cd(iv); iv=1;
  hapziphi[1]->Draw("colz");

  cc1->cd(1);

}


void iphi_pzbin()
{
  const UInt_t harm = 20;
  
  const UInt_t pznbin = 20;
  Double_t pzbin[pznbin+1];
  for(UInt_t n = 1; n <= pznbin; n++)
    pzbin[n] = 3000./(Double_t)pznbin * (Double_t)n;

  
  TH2D *hbaiphi[2];
  TH1D *hbiphi[2];
  TH1D *haiphi[2];
  TH2D *hbpziphi[2];
  TH2D *hapziphi[2];

  for(Int_t m = 0; m < 2; m++){
  //  for(UInt_t m = 0; m < 1; m++){

    hbaiphi[m] = new TH2D(Form("hbaiphi%d",m),  " #phi_{i} before and after; before #phi_{i} [rad]; after #phi_{i} [rad] ", 400,-3.5,3.5,400,-3.5,3.5);
    hbiphi[m]  = new TH1D(Form("hbiphi%d",m),   " #phi_{i} before; Azimuthal angle [rad]"           , 400,-3.5,3.5);
    haiphi[m]  = new TH1D(Form("haiphi%d",m),   " #phi_{i} after ; Azimuthal angle [rad]"           , 400,-3.5,3.5);
    hbpziphi[m]= new TH2D(Form("hbpziphi%d",m), " before ; pz; #phi_{i};  "           , 200,0.,3200., 400,-3.5,3.5); 
    hapziphi[m]= new TH2D(Form("hapziphi%d",m), " after  ; pz; #phi_{i};  "           , 200,0.,3200., 400,-3.5,3.5); 

    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("iphi"  ,&iphi  ,&biphi);
    rChain[m]->SetBranchAddress("pid"   ,&pid   ,&bpid);
    rChain[m]->SetBranchAddress("pz"    ,&pz    ,&bpz);


    STFlowCorrection *flowcorr[pznbin+1];
    for(UInt_t i = 0; i <= pznbin; i++)   flowcorr[i] = new STFlowCorrection(rChain[m]->GetListOfFiles(), harm, m);    

    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;

    vector< vector<Double_t> > bphi(pznbin+1); 
    vector< vector<Double_t> > bpz(pznbin+1); 

    for(UInt_t k = 0; k <= pznbin; k++){
      bphi.clear();
      bpz.clear();
    }

    Int_t icout = 0;

    for(UInt_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);


      for(UInt_t j = 0; j < (UInt_t)iphi->size(); j++){ 

	hbpziphi[m]->Fill(pz->at(j) , iphi->at(j));

	for(UInt_t n = 0; n <= pznbin; n++){
	  if( pz->at(j) < pzbin[n] ) {
	    bphi[n].push_back(iphi->at(j));
	    bpz[n].push_back(pz->at(j));
	    break;
	  }
	  else if ( pz->at(j) > pzbin[pznbin] ){
	    bphi[n].push_back(iphi->at(j));
	    bpz[n].push_back(pz->at(j));
	    break;
	  }
	}
      }
    }
      
    for(UInt_t i = 0; i <= pznbin; i++){
      cout << i << "th "   << bphi[i].size() << " && " << bpz[i].size() <<endl;
      
      if(bphi[i].size() > 0) {
	flowcorr[i]->FourierCorrection(bphi[i]);    
	  
	for(UInt_t j = 0; j < (UInt_t)bphi[i].size(); j++){
	  hapziphi[m]->Fill(bpz[i].at(j), bphi[i].at(j));
	  haiphi[m]->Fill(bphi[i].at(j));	  
	}
      }

      flowcorr[i]-> SaveCorrectionFactor(Form("n%dpz:flatten_iphi along pz<  %f",i,pzbin[i]));    
    }
  }


  cc0 = new TCanvas("cc0","cc0",700,1000);
  cc0->Divide(2,3);

  UInt_t iv = 1;
  cc0->cd(iv); iv++;
  hbpziphi[0]->Draw("colz");

  cc0->cd(iv); iv++;
  hbpziphi[1]->Draw("colz");

  cc0->cd(iv); iv++;
  hapziphi[0]->Draw("colz");

  cc0->cd(iv); iv++;
  hapziphi[1]->Draw("colz");

  cc0->cd(iv); iv++;
  haiphi[0]->Draw();

  cc0->cd(iv); iv++;
  haiphi[1]->Draw();

  cc0->cd(1);

}
