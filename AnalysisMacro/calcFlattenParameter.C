#include "STFlowCorrection.hh"

TCanvas *cc0 = NULL;
TCanvas *cc1 = NULL;
TCanvas *cc2 = NULL;
TCanvas *cc3 = NULL;
TCanvas *cc4 = NULL;
TCanvas *cc5 = NULL;
TCanvas *cc6 = NULL;

TCanvas *cc[6];

const Int_t nbinx = 30;
UInt_t  fVer = 0;

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

TH1D *h1phimid_r;
TH1D *h1phimid_m;
TH1D *h1phimid_rm;
TF1  *f1;

TFile *fout;
TTree *cflw;



Int_t    ntrack[7];
Int_t    mtrack;
Int_t    mtrack_1;
Int_t    mtrack_2;

TClonesArray *aParticleArray;

Int_t             iRun=0;
Double_t          z = 0.;
Double_t          aoq = 0.;
vector<Double_t> *iphi=0;
vector<Double_t> *rpphi=0;
vector<Double_t> *rapid=0;
vector<Double_t> *prapid=0;
vector<Int_t>    *pid=0;
vector<Double_t> *pz=0;
vector<Double_t> *px=0;
vector<Double_t> *py=0;
vector<Double_t> *deltphi=0;
TClonesArray     *p_org=0;
TClonesArray     *p_rot=0;


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

UInt_t m_bgn = 0;
UInt_t m_end = 2;

void flatten_iphi_mtrkthetabin();
void flatten_iphi_mtrkthetabin2();
void OutputTree(TChain *rCh);

void calcFlattenParameter()
{
  gROOT->Reset();

  UInt_t im = 0; 
  TString sMix = gSystem -> Getenv("MIX");
  if(sMix != "")
    im = atoi(sMix);


  cout << " im " << im << endl;
 
  if( im < 2){
    gROOT->Macro(Form("openRM.C(%d)",im));
    rChain[im] = (TChain*)gROOT->FindObject(Form("rChain%d",im));
    //    if(rChain[im] == NULL ) exit(0);
  }


  if(im == 0){ // REAL only
    m_bgn = 0;
    m_end = 1;
  }
  else if(im == 1){ // MIXed only
    m_bgn = 1;
    m_end = 2;
  }


  TString sVer = gSystem -> Getenv("VER");
  TString ssVer = "";
  TString tVer  = sVer+".";

  for(Int_t i = 0; i < 3; i++){
    Ssiz_t idx = tVer.Index(".");
    ssVer += tVer(0 , (Int_t)idx);
    tVer.Replace(0,(Int_t)(idx+1),"");
  }
  Int_t fVer = atoi(ssVer); 


  dxbin = 1./(Double_t)nbinx; 



  flatten_iphi_mtrkthetabin2();
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

    STFlowCorrection *flowcorr = new STFlowCorrection(rChain[m], harm,m);

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

    STFlowCorrection *flowcorr = new STFlowCorrection(rChain[m], harm, m);
    
    hbphi[m]  = new TH1D(Form("hbphi%d",m), "before correction; #Delta(#Phi_1 - #Phi_2) [rad] "   ,60.,-4.,4.);
    haphi[m]  = new TH1D(Form("haphi%d",m), "after correction; #Delta(#Phi_1 - #Phi_2) [rad]"   ,60.,-4.,4.);
    h2bphi[m] = new TH2D(Form("h2bphi%d",m),"before correction; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);
    h2aphi[m] = new TH2D(Form("h2aphi%d",m),"after correcttion; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] "     ,60.,-4.,4.,60.,-4.,4.);
    
    hbcos[m]  = new TH1D(Form("hbcos%d",m), "before correction; cos(#Delta#Phi) "   ,100,-1.,1.);
    hacos[m]  = new TH1D(Form("hacos%d",m), "after  correction; cos(#Delta#Phi) "   ,100,-1.,1.);


    rChain[m]->SetBranchAddress("mtrack"    ,&mtrack);
    rChain[m]->SetBranchAddress("mtrack_1"  ,&mtrack_1);
    rChain[m]->SetBranchAddress("mtrack_2"  ,&mtrack_2);
    rChain[m]->SetBranchAddress("unitP"     ,&unitP,&bunitP);
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


void flatten_Subevent()
{

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

    flowcorr->GetCorrectionFactor();
    flowcorr->GetCorrection(phi_1);

    flowcorr->GetCorrectionFactor();
    flowcorr->GetCorrection(phi_2);

    flowcorr->FourierCorrection(phi_1);
    flowcorr->FourierCorrection(phi_2);

    
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

    STFlowCorrection *flowcorr = new STFlowCorrection(rChain[m], harm, m);
    
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

    if(fVer < 210 || fVer != 222) { 
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


void flatten_iphi_pzbin()
{
  const UInt_t harm = 20;
  
  const UInt_t pznbin = 20;
  Double_t pzbin[pznbin+1];
  for(UInt_t n = 0; n < pznbin; n++)
    pzbin[n]    = 3500./(Double_t)pznbin * (Double_t)(n+1);
  pzbin[pznbin] = 3500.;

  
  TH2D *hbaiphi[2];
  TH1D *hbiphi[2];
  TH1D *haiphi[2];
  TH2D *hbpziphi[2];
  TH2D *hapziphi[2];

  for(Int_t m = 0; m < 2; m++){
  //for(UInt_t m = 0; m < 1; m++){

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
    for(UInt_t i = 0; i <= pznbin; i++)   flowcorr[i] = new STFlowCorrection(rChain[m], harm, m);    

    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;

    vector< vector<Double_t> > bphi(pznbin+1); 
    vector< vector<Double_t> > bpz(pznbin+1); 

    for(UInt_t k = 0; k <= pznbin; k++){
      bphi[k].clear();
      bpz[k].clear();
    }

    Int_t icout = 0;

    //    cout << " at " << pzbin[pznbin-1] << " " << pzbin[pznbin] << endl;

    for(UInt_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);


      for(UInt_t j = 0; j < (UInt_t)iphi->size(); j++){ 

	hbpziphi[m]->Fill(pz->at(j) , iphi->at(j));


	if ( pz->at(j) >= pzbin[pznbin] ){
	  bphi[pznbin].push_back(iphi->at(j));
	  bpz[pznbin].push_back(pz->at(j));
	}
	else {

	  for(UInt_t n = 0; n < pznbin; n++){
	    if( pz->at(j) < pzbin[n] ) {
	      bphi[n].push_back(iphi->at(j));
	      bpz[n].push_back(pz->at(j));
	      break;
	    }
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
  if(hbpziphi[0]) hbpziphi[0]->Draw("colz");

  cc0->cd(iv); iv++;
  if(hbpziphi[1]) hbpziphi[1]->Draw("colz");

  cc0->cd(iv); iv++;
  if(hapziphi[0]) hapziphi[0]->Draw("colz");

  cc0->cd(iv); iv++;
  if(hapziphi[1]) hapziphi[1]->Draw("colz");

  cc0->cd(iv); iv++;
  if(haiphi[0]) haiphi[0]->Draw();

  cc0->cd(iv); iv++;
  if(haiphi[1]) haiphi[1]->Draw();

  cc0->cd(1);

}

void flatten_iphi_thetabin()
{

  std::cout << "From " << m_bgn << " to " << m_end << std::endl;


  const UInt_t harm = 20;
  
  const UInt_t thetanbin = 40;
  Double_t thetabin[thetanbin+1];
  Double_t theta_min = 0.;
  Double_t theta_max = 1.4;
  for(UInt_t n = 0; n < thetanbin+1; n++)
    thetabin[n]    = theta_max/(Double_t)thetanbin * (Double_t)n;

  TH2D *hbaiphi[2];
  TH1D *hbiphi[2];
  TH1D *haiphi[2];
  TH2D *hbthetaiphi[2];
  TH2D *hathetaiphi[2];

 

  for(UInt_t m = m_bgn; m < m_end; m++){

    hbaiphi[m] = new TH2D(Form("hbaiphi%d",m),  " #phi_{i} before and after; before #phi_{i} [rad]; after #phi_{i} [rad] ", 
			  400,-3.5,3.5,400,-3.5,3.5);
    hbiphi[m]  = new TH1D(Form("hbiphi%d",m),   " #phi_{i} before; Azimuthal angle [rad]"           , 400,-3.5,3.5);
    haiphi[m]  = new TH1D(Form("haiphi%d",m),   " #phi_{i} after ; Azimuthal angle [rad]"           , 400,-3.5,3.5);
    hbthetaiphi[m]= new TH2D(Form("hbthetaiphi%d",m), " before ; theta; #phi_{i};  "           , 200,0.,1.6, 400,-3.5,3.5); 
    hathetaiphi[m]= new TH2D(Form("hathetaiphi%d",m), " after  ; theta; #phi_{i};  "           , 200,0.,1.6, 400,-3.5,3.5); 

    p_org = new TClonesArray("TVector3", 100);
    p_rot = new TClonesArray("TVector3", 100);

    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("iphi"  ,&iphi  ,&biphi);
    rChain[m]->SetBranchAddress("pid"   ,&pid   ,&bpid);
    rChain[m]->SetBranchAddress("p_org" ,&p_org);
    rChain[m]->SetBranchAddress("p_rot" ,&p_rot);


    STFlowCorrection *flowcorr[thetanbin+1];
    for(UInt_t i = 0; i < thetanbin+1; i++)   flowcorr[i] = new STFlowCorrection(rChain[m], harm, m);    

    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;

    vector< vector<Double_t> > bphi(thetanbin+1); 
    vector< vector<Double_t> > btheta(thetanbin+1); 

    for(UInt_t k = 0; k <= thetanbin; k++){
      bphi[k].clear();
      btheta[k].clear();
    }

    Int_t icout = 0;

    //    cout << " at " << thetabin[thetanbin-1] << " " << thetabin[thetanbin] << endl;

    for(UInt_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);


      if( p_org->GetEntries() != iphi->size() ){
	std::cout << "Error: p_org " << p_org->GetEntries() << " iphi " << iphi->size() << std::endl;
	continue;
      }

      for(UInt_t j = 0; j < (UInt_t)iphi->size(); j++){ 

	//Double_t theta = ((TVector3*)p_org->At(j))->Theta();
	Double_t theta = ((TVector3*)p_rot->At(j))->Theta();

	Double_t phi = ((TVector3*)p_rot->At(j))->Phi();
	

	hbthetaiphi[m]->Fill(theta , phi);

	if ( theta >= thetabin[thetanbin] ){
	  bphi[thetanbin].push_back(phi);
	  btheta[thetanbin].push_back(theta);
	}
	else {

	  for(UInt_t n = 0; n < thetanbin; n++){
	    if( theta < thetabin[n+1] ) {
	      bphi[n].push_back(phi);
	      btheta[n].push_back(theta);
	      break;
	    }
	  }

	}
      }
    }
      
    for(UInt_t i = 0; i < thetanbin+1; i++){
      cout << i << "th "   << bphi[i].size() << " && " << btheta[i].size() <<endl;
      
      if(bphi[i].size() > 0) {
	flowcorr[i]->FourierCorrection(bphi[i]);    


	for(UInt_t j = 0; j < (UInt_t)bphi[i].size(); j++){
	  hathetaiphi[m]->Fill(btheta[i].at(j), bphi[i].at(j));
	  haiphi[m]->Fill(bphi[i].at(j));	  
	}
      }

      if( i == thetanbin)
	flowcorr[i]-> SaveCorrectionFactor(Form("n%dtheta:flatten_iphi between theta> %f",i,thetabin[i]));    
      else      
	flowcorr[i]-> SaveCorrectionFactor(Form("n%dtheta:flatten_iphi between theta> %f && theta< %f",i,thetabin[i],thetabin[i+1]));    
    }
  
    cc[m] = new TCanvas(Form("cc%d",m),Form("cc%d",m),350,1000);

    cc[m]->Divide(1,3);

    UInt_t iv = 1;
    cc[m]->cd(iv); iv++;
    if(hbthetaiphi[m]) hbthetaiphi[m]->Draw("colz");

    cc[m]->cd(iv); iv++;
    if(hathetaiphi[m]) hathetaiphi[m]->Draw("colz");

    cc[m]->cd(iv); iv++;
    if(haiphi[m]) haiphi[m]->Draw();

    cc[m]->cd(1);
  }
}

void flatten_iphi_mtrkthetabin()
{
  std::cout << "flatten_iphi_thetamtkbin " << std::endl;

  std::cout << "From " << m_bgn << " to " << m_end << std::endl;

  const UInt_t harm = 20;
  
  const UInt_t thetanbin = 40;
  Double_t thetabin[thetanbin+1];
  Double_t theta_min = 0.;
  Double_t theta_max = 1.4;
  for(UInt_t n = 0; n < thetanbin+2; n++)
    thetabin[n]    = theta_max/(Double_t)thetanbin * (Double_t)n;

  const UInt_t mtrknbin=6;
  Double_t mtrkbin[mtrknbin+1];
  Double_t mtrk_min = 0;
  Double_t mtrk_max = 60;
  for(UInt_t n = 0; n < mtrknbin+2; n++)
    mtrkbin[n]   = mtrk_max/mtrknbin * n;

  TH2D *hbaiphi[2];
  TH1D *hbiphi[2];
  TH1D *haiphi[2];
  TH2D *hbthetaiphi[2];
  TH2D *hathetaiphi[2];
  TH2D *hbmtrkiphi[2];
  TH2D *hamtrkiphi[2];
 

  UInt_t im = 0;

  for(UInt_t m = m_bgn; m < m_end; m++){

    hbaiphi[m] = new TH2D(Form("hbaiphi%d",m),  " #phi_{i} before and after; before #phi_{i} [rad]; after #phi_{i} [rad] ", 
			  400,-3.5,3.5,400,-3.5,3.5);
    hbiphi[m]  = new TH1D(Form("hbiphi%d",m),   " #phi_{i} before; Azimuthal angle [rad]"  , 400,-3.2,3.2);
    haiphi[m]  = new TH1D(Form("haiphi%d",m),   " #phi_{i} after ; Azimuthal angle [rad]"  , 400,-3.2,3.2);
    hbthetaiphi[m]= new TH2D(Form("hbthetaiphi%d",m), " before ; theta; #phi;  "           , 200,0.,1.6, 400,-3.2,3.2); 
    hathetaiphi[m]= new TH2D(Form("hathetaiphi%d",m), " after  ; theta; #phi;  "           , 200,0.,1.6, 400,-3.2,3.2); 

    hbmtrkiphi[m] = new TH2D(Form("hbmtrkiphi%d",m)," before ; Number of tracks; #phi"     , 70,0,70,400,-3.2,3.2);
    hamtrkiphi[m] = new TH2D(Form("hamtrkiphi%d",m)," after  ; Number of tracks; #phi"     , 70,0,70,400,-3.2,3.2);

    p_org = new TClonesArray("TVector3", 100);
    p_rot = new TClonesArray("TVector3", 100);

    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("ntrack",ntrack);
    rChain[m]->SetBranchAddress("iphi"  ,&iphi  ,&biphi);
    rChain[m]->SetBranchAddress("pid"   ,&pid   ,&bpid);
    rChain[m]->SetBranchAddress("p_org" ,&p_org);
    rChain[m]->SetBranchAddress("p_rot" ,&p_rot);

    STFlowCorrection *flowcorr[mtrknbin+1][thetanbin+1];
    for(UInt_t j = 0; j < mtrknbin+1; j++){   
      for(UInt_t i = 0; i < thetanbin+1; i++)  { 
	flowcorr[j][i] = new STFlowCorrection(rChain[m], harm, m); 

	flowcorr[j][i]->SetBin_min(0, mtrkbin[j]);
	flowcorr[j][i]->SetBin_min(1, thetabin[i]);
	
	if(j < mtrknbin+1 && i < thetanbin+1){
	  flowcorr[j][i]->SetBin_max(0, mtrkbin[j+1]);
	  flowcorr[j][i]->SetBin_max(1, thetabin[i+1]);
	}
      }   
    }
    

    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;


    Int_t icout = 0;

    //    cout << " at " << thetabin[thetanbin-1] << " " << thetabin[thetanbin] << endl;

    
    for(UInt_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);


      UInt_t imtrk  = 0;

      UInt_t j = mtrknbin;
      while(1){ 
	if( ntrack[3] >= mtrkbin[j] ){
	  imtrk = j;
	  break;
	}
	j--;
      }

      if( p_org->GetEntries() != iphi->size() ){
	std::cout << "Error: p_org " << p_org->GetEntries() << " iphi " << iphi->size() << std::endl;
	continue;
      }


      UInt_t itheta;
      for(UInt_t j = 0; j < (UInt_t)iphi->size(); j++){ 
	// Double_t phi   = ((TVector3*)p_org->At(j))->Phi();
	// Double_t theta = ((TVector3*)p_org->At(j))->Theta();
	Double_t phi   = ((TVector3*)p_rot->At(j))->Phi();
	Double_t theta = ((TVector3*)p_rot->At(j))->Theta();

	UInt_t k = thetanbin;
	while(1){ 
	  if( theta >= thetabin[k] ){
	    itheta = k;
	    break;
	  }
	  k--;
	}

	hbthetaiphi[m]->Fill(theta , phi);	  
	hbmtrkiphi[m]-> Fill(mtrack  , phi);	  

	if(imtrk <= mtrknbin && itheta <= thetanbin)
	  flowcorr[imtrk][itheta]->Add(mtrack,phi,theta);
      }
    }

    //----------  get corrrection parameters

    for(UInt_t j = 0; j < mtrknbin+1; j++){   

      for(UInt_t i = 0; i < thetanbin+1; i++) {   
	UInt_t nphi = flowcorr[j][i]->FourierCorrection();
	std::cout << " At " << mtrkbin[j] << " : " << thetabin[i] << std::endl;

	if(nphi == 0) {
	  std::cout << " no data is stored " << std::endl;
	  continue;
	}

	vector<Int_t>    mtk   = flowcorr[j][i]->GetMTrack();
	vector<Double_t> aphi  = flowcorr[j][i]->GetCorrectedPhi();
	vector<Double_t> atheta= flowcorr[j][i]->GetTheta();

	// std::cout << " size of pair  " << aphi.size() << " : " << atheta.size() << " / " << nphi << std::endl;
	// std::cout << " mtrack mean " << flowcorr[j][i]->GetMTrackMean() << " theta mean " << flowcorr[j][i]->GetThetaMean() << endl;

	if( aphi.size() != atheta.size() ){
	  std::cout << " size of pair doesn't match " << aphi.size() << " : " << atheta.size() << std::endl;
	  continue;
	}

	
	for(UInt_t k = 0; k < (UInt_t)mtk.size(); k++){	  
	  hathetaiphi[m]->Fill(atheta.at(k), aphi.at(k));
	  hamtrkiphi[m]-> Fill(mtk.at(k)   , aphi.at(k));
	  haiphi[m]->Fill(aphi.at(k));	  
	}
	
	TString comm = Form("m%dn%dmtktheta:flatten_iphi_mtrkthetabin; mtrack> %f && mtrack< %f theta> %f && theta< %f",
			    j,i,mtrkbin[j],mtrkbin[j+1],thetabin[i],thetabin[i+1]);
	flowcorr[j][i]-> SaveCorrectionFactor(comm);    
      }
    }
  

    cc[im] = new TCanvas(Form("cc%d",im),Form("cc%d",im),700,1000);
    cc[im]->Divide(2,3);
    
    UInt_t iv = 1;
    cc[im]->cd(iv); iv++;
    if(hbthetaiphi[im]) hbthetaiphi[m]->Draw("colz");
    
    cc[im]->cd(iv); iv++;
    if(hbmtrkiphi[m])   hbmtrkiphi[m]->Draw("colz");

    cc[im]->cd(iv); iv++;
    if(hathetaiphi[m])  hathetaiphi[m]->Draw("colz");

    cc[im]->cd(iv); iv++;
    if(hamtrkiphi[m])   hamtrkiphi[m]->Draw("colz");
    
    cc[im]->cd(iv); iv++;
    if(haiphi[m])       haiphi[m]->Draw();
    
    cc[im]->cd(1);

    
    im++;



    if(m == 0){
      cc[im] = new TCanvas(Form("cc%d",im),Form("cc%d",im),700,500);
      cc[im]->Divide(2,2);
    
      iv = 1;
      cc[im]->cd(iv); iv++;
    
      auto hvphi  = new TH1D("hvphi"  ,"phi"   ,100,-3.2,3.2);
      auto hvthet = new TH1D("hvtheta","theta" ,100,0.,1.4);
      auto hvmtk  = new TH1I("hvmtk"  ,"mtrack", 60,0,60);
    
      vector<Double_t>::iterator itr;
      vector<Int_t>::iterator   iitr;

      vector<Double_t> vec1 =  flowcorr[3][3]->GetOriginalPhi();
      for(itr=vec1.begin(); itr!=vec1.end(); itr++)      
     	hvphi->Fill(*itr);
      vec1.clear();

      hvphi->Draw();


      cc[im]->cd(iv); iv++;
      vec1 =  flowcorr[3][3]->GetTheta();
      for(itr=vec1.begin(); itr!=vec1.end(); itr++)      
     	hvthet->Fill(*itr);
    
      hvthet->Draw();
    
      cc[im]->cd(iv); iv++;
      vector<Int_t> vec2 =  flowcorr[3][3]->GetMTrack();
      for(iitr = vec2.begin(); iitr != vec2.end(); iitr++)
     	hvmtk->Fill(*iitr);

      hvmtk->Draw();

    }
  }

}

void flatten_iphi_mtrkthetabin2()
{
  std::cout << "flatten_iphi_thetamtkbin2" << std::endl;

  std::cout << "From " << m_bgn << " to " << m_end << std::endl;

  const UInt_t harm = 20;
  
  const UInt_t thetanbin = 40;
  Double_t thetabin[thetanbin+1];
  Double_t theta_min = 0.;
  Double_t theta_max = 1.4;
  for(UInt_t n = 0; n < thetanbin+2; n++)
    thetabin[n]    = theta_max/(Double_t)thetanbin * (Double_t)n;

  const UInt_t mtrknbin=6;
  Double_t mtrkbin[mtrknbin+1];
  Double_t mtrk_min = 0;
  Double_t mtrk_max = 60;
  for(UInt_t n = 0; n < mtrknbin+2; n++)
    mtrkbin[n]   = mtrk_max/mtrknbin * n;

  TH2D *hbaiphi[2];
  TH1D *hbiphi[2];
  TH1D *haiphi[2];
  TH2D *hbthetaiphi[2];
  TH2D *hathetaiphi[2];
  TH2D *hbmtrkiphi[2];
  TH2D *hamtrkiphi[2];
 

  UInt_t im = 0;

  for(UInt_t m = m_bgn; m < m_end; m++){

    //    OutputTree(rChain[m]);

    hbaiphi[m] = new TH2D(Form("hbaiphi%d",m),  " #phi_{i} before and after; before #phi_{i} [rad]; after #phi_{i} [rad] ", 
			  400,-3.5,3.5,400,-3.5,3.5);
    hbiphi[m]  = new TH1D(Form("hbiphi%d",m),   " #phi_{i} before; Azimuthal angle [rad]"  , 400,-3.2,3.2);
    haiphi[m]  = new TH1D(Form("haiphi%d",m),   " #phi_{i} after ; Azimuthal angle [rad]"  , 400,-3.2,3.2);
    hbthetaiphi[m]= new TH2D(Form("hbthetaiphi%d",m), " before ; theta; #phi;  "           , 200,0.,1.6, 400,-3.2,3.2); 
    hathetaiphi[m]= new TH2D(Form("hathetaiphi%d",m), " after  ; theta; #phi;  "           , 200,0.,1.6, 400,-3.2,3.2); 

    hbmtrkiphi[m] = new TH2D(Form("hbmtrkiphi%d",m)," before ; Number of tracks; #phi"     , 70,0,70,400,-3.2,3.2);
    hamtrkiphi[m] = new TH2D(Form("hamtrkiphi%d",m)," after  ; Number of tracks; #phi"     , 70,0,70,400,-3.2,3.2);

    aParticleArray = new TClonesArray("STParticle",100);

    p_org = new TClonesArray("TVector3", 100);
    p_rot = new TClonesArray("TVector3", 100);

    rChain[m]->SetBranchAddress("irun",&iRun);
    rChain[m]->SetBranchAddress("z",&z);
    rChain[m]->SetBranchAddress("aoq",&aoq);
    rChain[m]->SetBranchAddress("mtrack",&mtrack);
    rChain[m]->SetBranchAddress("ntrack",ntrack);

    rChain[m]->SetBranchAddress("STParticle",&aParticleArray);

    STFlowCorrection *flowcorr[mtrknbin+1][thetanbin+1];

    for(UInt_t j = 0; j < mtrknbin+1; j++){   
      for(UInt_t i = 0; i < thetanbin+1; i++)  { 

	flowcorr[j][i] = new STFlowCorrection(rChain[m], harm, m); 

	flowcorr[j][i]->SetBin_min(0, mtrkbin[j]);
	flowcorr[j][i]->SetBin_min(1, thetabin[i]);
	
	if(j < mtrknbin+1 && i < thetanbin+1){
	  flowcorr[j][i]->SetBin_max(0, mtrkbin[j+1]);
	  flowcorr[j][i]->SetBin_max(1, thetabin[i+1]);
	}
      }   
    }
    


    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;


    Int_t icout = 0;

    //    cout << " at " << thetabin[thetanbin-1] << " " << thetabin[thetanbin] << endl;

    
    for(UInt_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);


      UInt_t imtrk  = 0;

      UInt_t j = mtrknbin;
      while(1){ 
	if( ntrack[3] >= mtrkbin[j] ){
	  imtrk = j;
	  break;
	}
	j--;
      }


      UInt_t itheta = 0;
      TIter next(aParticleArray);
      STParticle *aPart1 = NULL;

      
      while( (aPart1 = (STParticle*)next()) ){


	//	cout << " apart loop " << i << endl;

	if(aPart1->GetReactionPlaneFlag()){
	  
	  Double_t phi   = aPart1->GetRotatedMomentum().Phi();
	  Double_t theta = aPart1->GetRotatedMomentum().Theta();

	  UInt_t k = thetanbin;
	  while(1){ 
	    if( theta >= thetabin[k] ){
	      itheta = k;
	      break;
	    }
	    k--;
	  }

	  hbthetaiphi[m]->Fill(theta , phi);	  
	  hbmtrkiphi[m] ->Fill(mtrack  , phi);	  

	  if(imtrk <= mtrknbin && itheta <= thetanbin) {
	    flowcorr[imtrk][itheta]->Add(mtrack,phi,theta);

	    aPart1->SetFlattenBinID(imtrk, itheta);

	  }
	}
      }
      //      cflw->Fill();
    } 

    //----------  get corrrection parameters
    for(UInt_t j = 0; j < mtrknbin+1; j++){   

      for(UInt_t i = 0; i < thetanbin+1; i++) {   
	UInt_t nphi = flowcorr[j][i]->FourierCorrection();
	std::cout << " At " << mtrkbin[j] << " : " << thetabin[i] << std::endl;

	if(nphi == 0) {
	  std::cout << " no data is stored " << std::endl;
	  continue;
	}

	vector<Int_t>    mtk   = flowcorr[j][i]->GetMTrack();
	vector<Double_t> aphi  = flowcorr[j][i]->GetCorrectedPhi();
	vector<Double_t> atheta= flowcorr[j][i]->GetTheta();

	// std::cout << " size of pair  " << aphi.size() << " : " << atheta.size() << " / " << nphi << std::endl;
	// std::cout << " mtrack mean " << flowcorr[j][i]->GetMTrackMean() << " theta mean " << flowcorr[j][i]->GetThetaMean() << endl;

	if( aphi.size() != atheta.size() ){
	  std::cout << " size of pair doesn't match " << aphi.size() << " : " << atheta.size() << std::endl;
	  continue;
	}
	
	for(UInt_t k = 0; k < (UInt_t)mtk.size(); k++){	  
	  hathetaiphi[m]->Fill(atheta.at(k), aphi.at(k));
	  hamtrkiphi[m] ->Fill(mtk.at(k)   , aphi.at(k));
	  haiphi[m]     ->Fill(aphi.at(k));	  
	}
	
	TString comm = Form("m%dn%dmtktheta:flatten_iphi_mtrkthetabin; mtrack> %f && mtrack< %f theta> %f && theta< %f",
			    j,i,mtrkbin[j],mtrkbin[j+1],thetabin[i],thetabin[i+1]);
	cout << "save " << comm << endl;
	flowcorr[j][i]-> SaveCorrectionFactor(comm);    
      }
    }
  

    cc[im] = new TCanvas(Form("cc%d",im),Form("cc%d",im),700,1000);
    cc[im]->Divide(2,3);
    
    UInt_t iv = 1;
    cc[im]->cd(iv); iv++;
    if(hbthetaiphi[m]) hbthetaiphi[m]->Draw("colz");
    
    cc[im]->cd(iv); iv++;
    if(hbmtrkiphi[m])   hbmtrkiphi[m]->Draw("colz");

    cc[im]->cd(iv); iv++;
    if(hathetaiphi[m])  hathetaiphi[m]->Draw("colz");

    cc[im]->cd(iv); iv++;
    if(hamtrkiphi[m])   hamtrkiphi[m]->Draw("colz");
    
    cc[im]->cd(iv); iv++;
    if(haiphi[m])       haiphi[m]->Draw();
    
    cc[im]->cd(1);

    
    im++;


    if(m == 0){
      cc[im] = new TCanvas(Form("cc%d",im),Form("cc%d",im),700,500);
      cc[im]->Divide(2,2);
    
      iv = 1;
      cc[im]->cd(iv); iv++;
    
      auto hvphi  = new TH1D("hvphi"  ,"phi"   ,100,-3.2,3.2);
      auto hvthet = new TH1D("hvtheta","theta" ,100,0.,1.4);
      auto hvmtk  = new TH1I("hvmtk"  ,"mtrack", 60,0,60);
    
      vector<Double_t>::iterator itr;
      vector<Int_t>::iterator   iitr;

      vector<Double_t> vec1 =  flowcorr[3][3]->GetOriginalPhi();
      for(itr=vec1.begin(); itr!=vec1.end(); itr++)      
	hvphi->Fill(*itr);
      vec1.clear();

      hvphi->Draw();


      cc[im]->cd(iv); iv++;
      vec1 =  flowcorr[3][3]->GetTheta();
      for(itr=vec1.begin(); itr!=vec1.end(); itr++)      
	hvthet->Fill(*itr);
    
      hvthet->Draw();
    
      cc[im]->cd(iv); iv++;
      vector<Int_t> vec2 =  flowcorr[3][3]->GetMTrack();
      for(iitr = vec2.begin(); iitr != vec2.end(); iitr++)
	hvmtk->Fill(*iitr);

      hvmtk->Draw();

    }
    
    
    // fout->Close();
    // delete cflw;
    // delete fout;
  }
}

void OutputTree(TChain *rCh)
{

  TString fchain = ((TFile*)rCh->GetListOfFiles()->At(0))->GetFile()->GetTitle();
  
  Ssiz_t bgn = fchain.First("_");
  TString ssName = fchain(bgn-7,bgn+12);
  
  TString foutname = "../data/"+ssName+".c.root";

  fout = new TFile(foutname,"recreate");
  cflw  = new TTree("cflw","Flattened ");


  cout << "Output file is " << foutname << endl;


  //-- output                                                                                                       
  cflw->Branch("irun",&iRun,"irun/I");
  cflw->Branch("aoq",&aoq,"aoq/D");
  cflw->Branch("z",&z,"z/D");
  //  cflw->Branch("STVertex",&vertexArray);
  cflw->Branch("STParticle",&aParticleArray);
  cflw->Branch("ntrack",ntrack,"ntrack[7]/I");



}
