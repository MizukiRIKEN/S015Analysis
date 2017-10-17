#include "STFlowCorrection.hh"
//#include "STParticle.hh"

TCanvas *cc0 = NULL;
TCanvas *cc1 = NULL;
TCanvas *cc2 = NULL;
TCanvas *cc3 = NULL;
TCanvas *cc4 = NULL;
TCanvas *cc5 = NULL;
TCanvas *cc6 = NULL;
TCanvas *cc7 = NULL;
TCanvas *cc8 = NULL;
TCanvas *cc9 = NULL;
TCanvas *cc10 = NULL;

TCanvas *cc[7];


const Double_t ycm      = 0.388568; // 132Sn + 124Sn
const Double_t ybeam_cm = 0.36599;
const Double_t ybeam_lb = 0.754558;

// rapidity dependence
const UInt_t   rp_nbin = 16;
Double_t rp_min = -1.;
Double_t rp_max = 1.6;
Double_t rp_dbin = (rp_max - rp_min)/(Double_t)rp_nbin;

// pt dependence
UInt_t   pt_nbin = 11;
Double_t pt_min  = 0;
Double_t pt_max  = 1100;
Double_t pt_dbin = (pt_max - pt_min)/(Double_t)(pt_nbin-1); 

Double_t rapid_max = 0.4;
Double_t rapid_min = 0.2;


TString  partname[] = {"pi-","pi+","proton","deuteron","triton"};
UInt_t   partid[] = {211, 211, 2212, 1000010020, 1000010030};
  
UInt_t ic = 0;

const Int_t nbinx = 30;
TChain *rChain[2];

UInt_t im = 0;
UInt_t m_bgn = 0;
UInt_t m_end = 2;

TMultiGraph *mg;

void plotv1v2(UInt_t selid=2);
void PtDependece(UInt_t hrm=1);
void YDependece(UInt_t hrm=1);
void DrawCenterLine();
void PhiYbin();
void PhiYbinf();


void calcCRDvsMX(UInt_t iv = 0, UInt_t hrm=1)
{
  gROOT->Reset();


  TString sMix = gSystem -> Getenv("MIX");
  if(sMix != "")
    im = atoi(sMix);

  if(im == 0){ // REAL only
    m_bgn = 0;
    m_end = 1;

    cout << " -->  REAL " << endl;
  }
  else if(im == 1){ // MIXed only
    m_bgn = 1;
    m_end = 2;

    cout << " --> MIXed " << endl;
  }


  for( UInt_t ichain = m_bgn; ichain < m_end; ichain++){

    gROOT->Macro(Form("openCRM.C(%d)",ichain));
    rChain[ichain] = (TChain*)gROOT->FindObject(Form("rChain%d",ichain));
    //    if(rChain[ichain] == NULL ) exit(0);                                                                                          

  }


  if(iv == 0)
    YDependece(hrm);
  else if(iv == 1)
    PtDependece(hrm);
  else if(iv == 2)
    PhiYbinf();

}

Double_t *vMean(vector<Double_t> &vec)
{


  vector<Double_t>::iterator ibgn = vec.begin();
  vector<Double_t>::iterator iend = vec.end();

  auto *vc = new Double_t[2];
  vc[0]  =  TMath::Mean(ibgn, iend);
  vc[1]  =  TMath::RMS(ibgn, iend);

  if(vec.size() < 2) {
    vc[0] = -999.;
    vc[1] = 0.;
  }

  return vc;
}
Double_t *vn(UInt_t hm, vector<Double_t> &vphi)
{

  vector<Double_t> fvCos;
  
  //  cout << " vphi size " << vphi.size() << endl;
  Double_t findx = (Double_t)hm;
  for(UInt_t i = 0; i < (UInt_t)vphi.size(); i++)
    fvCos.push_back(cos(findx * vphi.at(i)));

  vector<Double_t>::iterator ibgn = fvCos.begin();
  vector<Double_t>::iterator iend = fvCos.end();

  Double_t *vcos = new Double_t[2];
  vcos[0]       =  TMath::Mean(ibgn, iend);
  Double_t rms  =  TMath::RMS(ibgn, iend);

  rms /= sqrt(vphi.size());
  vcos[1] = rms;

  if(vphi.size() < 2) {
    vcos[0] = -999.;
    vcos[1] = 0.;
  }

  //  cout << hm << " Mean " << vcos[0] << " RMS " << vcos[1] << endl; 
  return vcos;
}
  
void PhiYbinf()
{

  TH1D *hphi_prt[rp_nbin];
  TH1D *hphi_dtr[rp_nbin];
  TH1D *hphi_trt[rp_nbin];

  TString hname;
  TString htitle;
  for(UInt_t i = 0; i < rp_nbin; i++){
    htitle = Form("%f5.2 < ycm/ycm_beam < %f5.2 ;[Rad]",rp_min+i*rp_dbin, rp_min+(i+1.)*rp_dbin); 

    hname  = Form("hphi_prt%d",i);
    hphi_prt[i] = new TH1D(hname,"Proton"+htitle ,15, 0.,3.1);
    hname  = Form("hphi_dtr%d",i);
    hphi_dtr[i] = new TH1D(hname,"Deutron"+htitle,15, 0.,3.1);
    hname  = Form("hphi_trt%d",i);
    hphi_trt[i] = new TH1D(hname,"Triton"+htitle ,15, 0.,3.1);
  }
  Double_t nfct = 15./3.1;


  Int_t mtrack;
  auto aArray = new TClonesArray("STParticle",100);

  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    
    rChain[m]->SetBranchAddress("STParticle",&aArray);
    rChain[m]->SetBranchAddress("mtrack" ,&mtrack);


    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;


    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;

      TIter next(aArray);
      STParticle *aPart = NULL;

      while( (aPart = (STParticle*)next()) ) {

	auto pid = aPart->GetPID();
	auto rapid = aPart-> GetRapidity();
	rapid = (rapid - ycm) / ybeam_cm;

	for(UInt_t k = 0; k < rp_nbin; k++){
	  
	  if(rapid < rp_min + rp_dbin * (Double_t)(k+1)) {
	    if(pid == partid[2])
	      hphi_prt[k]->Fill(abs(aPart->GetAzmAngle_wrt_RP()));
	    else if(pid == partid[3])
	      hphi_dtr[k]->Fill(abs(aPart->GetAzmAngle_wrt_RP()));
	    else if(pid == partid[4])
	      hphi_trt[k]->Fill(abs(aPart->GetAzmAngle_wrt_RP()));

	    break;
	    
	  }
	}
      }
    }   

    gStyle -> SetTitleFontSize(0.1);

    UInt_t ic = 0;
    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),1000,1200);
    cc[ic]->Divide(3,16);

    UInt_t id = 1;
    for(UInt_t i = 0; i < rp_nbin; i++){
      cc[ic]->cd(id); id++; 
      auto N = hphi_prt[i]->GetEntries();
      hphi_prt[i]->Scale(nfct/(Double_t)N);
      hphi_prt[i]->Draw();

      cc[ic]->cd(id); id++; 
      hphi_dtr[i]->Scale(nfct/(Double_t)N);
      hphi_dtr[i]->Draw();

      cc[ic]->cd(id); id++; 
      hphi_trt[i]->Scale(nfct/(Double_t)N);
      hphi_trt[i]->Draw();
    }
  }
}
void PhiYbin()
{

  TH1D *hphi_prt[rp_nbin/2];
  TH1D *hphi_dtr[rp_nbin/2];
  TH1D *hphi_trt[rp_nbin/2];

  TString hname;
  TString htitle;
  for(UInt_t i = 0; i < rp_nbin/2; i++){
    htitle = Form("%f5.2 < ycm/ycm_beam < %f5.2 ;[Rad]",rp_min+i*rp_dbin*2,rp_min+(i+1.)*rp_dbin*2); 

    hname  = Form("hphi_prt%d",i);
    hphi_prt[i] = new TH1D(hname,"Proton"+htitle ,15, 0.,3.1);
    hname  = Form("hphi_dtr%d",i);
    hphi_dtr[i] = new TH1D(hname,"Deutron"+htitle,15, 0.,3.1);
    hname  = Form("hphi_trt%d",i);
    hphi_trt[i] = new TH1D(hname,"Triton"+htitle ,15, 0.,3.1);
  }
  Double_t nfct = 15./3.1;


  Int_t mtrack;
  auto aArray = new TClonesArray("STParticle",100);

  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    
    rChain[m]->SetBranchAddress("STParticle",&aArray);
    rChain[m]->SetBranchAddress("mtrack" ,&mtrack);


    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;


    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;

      TIter next(aArray);
      STParticle *aPart = NULL;

      while( (aPart = (STParticle*)next()) ) {

	auto pid = aPart->GetPID();
	auto rapid = aPart-> GetRapidity();
	rapid = (rapid - ycm) / ybeam_cm;

	for(UInt_t k = 0; k < rp_nbin/2; k++){
	  
	  if(rapid < rp_min + rp_dbin*2.*(Double_t)(k+1)) {
	    if(pid == partid[2])
	      hphi_prt[k]->Fill(abs(aPart->GetAzmAngle_wrt_RP()));
	    else if(pid == partid[3])
	      hphi_dtr[k]->Fill(abs(aPart->GetAzmAngle_wrt_RP()));
	    else if(pid == partid[4])
	      hphi_trt[k]->Fill(abs(aPart->GetAzmAngle_wrt_RP()));

	    break;
	    
	  }
	}
      }
    }   

    gStyle -> SetTitleFontSize(0.1);

    UInt_t ic = 0;
    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),1000,1200);
    cc[ic]->Divide(3,8);

    
    UInt_t id = 1;
    for(UInt_t i = 0; i < 8; i++){
      cc[ic]->cd(id); id++; 
      auto N = hphi_prt[i]->GetEntries();
      hphi_prt[i]->Scale(nfct/(Double_t)N);
      hphi_prt[i]->Draw();

      cc[ic]->cd(id); id++; 
      hphi_dtr[i]->Scale(nfct/(Double_t)N);
      hphi_dtr[i]->Draw();

      cc[ic]->cd(id); id++; 
      hphi_trt[i]->Scale(nfct/(Double_t)N);
      hphi_trt[i]->Draw();
    }
  }
}

void plotv1v2(UInt_t selid=2)
{
  if(selid > 4 ) return;

  gStyle->SetGridColor(7);
  gStyle->SetGridStyle(1);


  Int_t pcharge = 1;
  if(selid == 0)  pcharge = -1;

  Int_t RPflag = 0;
  if(selid >= 2) RPflag = 10;


  cout << " Particle " << partname[selid] << endl;

  auto haccp = new TH2D("haccp",partname[selid]+";ycm/ycm_beam ; Pt [MeV/c]",100,-1.1, 1.5,100,0.,800.);
  auto haccx = new TH2D("haccx",partname[selid]+";ycm/ycm_beam ; Px [MeV/c]",100,-1.1, 1.5,100,-500,500.);


  Double_t rp_res[2] = {0.107183, 0.0003226};
  Double_t rp_rese[2]= {0.69 ,0.71};

  Int_t ntrack[7];
  Int_t mtrack;
  
  auto aArray = new TClonesArray("STParticle",100);

  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    
    rChain[m]->SetBranchAddress("STParticle",&aArray);
    rChain[m]->SetBranchAddress("ntrack",ntrack);
    rChain[m]->SetBranchAddress("mtrack" ,&mtrack);

    vector< vector<Double_t> > bphi(rp_nbin);
    vector< vector<Double_t> > rpdbin(rp_nbin);


    for(UInt_t k = 0; k < rp_nbin; k++){
      bphi[k].clear();
      rpdbin[k].clear();
    }


    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;


    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;

      TIter next(aArray);
      STParticle *aPart = NULL;

      while( (aPart = (STParticle*)next()) ) {

	auto pid = aPart->GetPID();

	if(pid == partid[selid] && aPart->GetCharge() == pcharge ){

	  auto rapid = aPart-> GetRapidity();

	  rapid = (rapid - ycm) / ybeam_cm;

	  haccp -> Fill(rapid, aPart->GetRotatedMomentum().Pt());
	  haccx -> Fill(rapid, aPart->GetFlattenMomentum().X());
	  
	  for(UInt_t k = 0; k < rp_nbin; k++){

	    if(rapid < rp_min + rp_dbin*(Double_t)(k+1)) {
	      bphi[k].push_back(aPart->GetAzmAngle_wrt_RP());
	      rpdbin[k].push_back(rapid);
	      
	      break;
	    }
	  }
	}
      }
    }

    Double_t xval[rp_nbin];
    Double_t xvale[rp_nbin];
    Double_t yval1[rp_nbin];
    Double_t yval1e[rp_nbin];
    Double_t yval2[rp_nbin];
    Double_t yval2e[rp_nbin];

    UInt_t j =0;
    UInt_t jn = 0;
    while(jn < rp_nbin){

      Double_t *getx = new Double_t[2];
      Double_t *gety = new Double_t[2];
      getx   = vMean(rpdbin[j]);

      if(getx[0] > -900){
	gety   = vn(1, bphi[j]);


	xval[j] = getx[0];
	yval1[j] = gety[0]; ///rp_res[m];

	xvale[j] = getx[1];
	yval1e[j] = gety[1];


	gety    = vn(2, bphi[j]);
	yval2[j] = gety[0]; ///rp_res[m];
	yval2e[j]= gety[1];

	j++;
      }
      jn++;
    }

    //print

    for(UInt_t i = 0; i < rp_nbin; i++)
      cout << i << xval[i] << " +-" <<  " v1 " << yval1[i] << " +- " << yval1e[i] << "  w  " << bphi[i].size() << endl;  
    for(UInt_t i = 0; i < rp_nbin; i++)
      cout << i << xval[i] << " +-" <<  " v2 " << yval2[i] << " +- " << yval2e[i] << "  w  " << bphi[i].size() << endl;  

    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    ic++;

    const UInt_t nsl = 9;
    Double_t rap[nsl];
    Double_t rape[nsl];
    Double_t v1[nsl];
    Double_t v1e[nsl];
    Double_t v2[nsl];
    Double_t v2e[nsl];

    for(UInt_t isl = 0; isl < nsl; isl++){
      UInt_t iisl = isl + 2;
      rap[isl]  = xval[iisl];
      rape[isl] = xvale[iisl];
      
      v1[isl]   = yval1[iisl];
      v1e[isl]  = yval1e[iisl];
      v2[isl]   = yval2[iisl];
      v2e[isl]  = yval2e[iisl];

    }

    //cc[ic]->cd(id); id++;
    auto gv_v1 = new TGraphErrors(nsl, rap, v1, rape, v1e);
    gv_v1->SetName("gv_v1");


    // gv_v1->SetMaximum(0.1);
    // gv_v1->SetMinimum(-0.1);
    gv_v1->SetLineColor(4);
    gv_v1->SetMarkerStyle(20);
    gv_v1->SetMarkerColor(4);
    gv_v1->SetTitle(partname[selid]+"; Ycm/Ycm_beam ; v1(a.u.)");
   
    gv_v1->Draw("ALP");

    auto xmin = gv_v1->GetYaxis()->GetXmin();
    auto xmax = gv_v1->GetYaxis()->GetXmax();
    auto aLineX = new TLine(0, xmin, 0,  xmax);
    aLineX->SetLineColor(1);
    aLineX->SetLineStyle(3);
    aLineX->Draw();

    xmin = gv_v1->GetXaxis()->GetXmin();
    xmax = gv_v1->GetXaxis()->GetXmax();
    auto aLineY = new TLine(xmin, 0, xmax, 0);
    aLineY->SetLineColor(1);
    aLineY->SetLineStyle(3);
    aLineY->Draw();


    
    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    ic++;

    
    auto gv_v2 = new TGraphErrors(nsl, rap, v2, rape, v2e);
    gv_v2->SetName("gv_v2");

    // gv_v2->SetMaximum(0.015);
    // gv_v2->SetMinimum(-0.015);
    gv_v2->SetLineColor(2);
    gv_v2->SetMarkerStyle(20);
    gv_v2->SetMarkerColor(2);
    gv_v2->SetTitle(partname[selid]+"; Ycm/Ycm_beam ; v2(a.u.)");

    gv_v2->Draw("ALP");

  
    UInt_t id = 1;
    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);


    //    cc[ic]->Divide(1,2);
    
    //    cc[ic]->cd(id); id++;
    haccp -> Draw("colz");
    
    //    cc[ic]->cd(id); 
    //    haccx -> Draw("colz");

    ic++;
  }
}

void DrawCenterLine()
{
  // Center Line
  auto xmin = mg->GetYaxis()->GetXmin();
  auto xmax = mg->GetYaxis()->GetXmax();
  auto aLineX = new TLine(0, xmin, 0,  xmax);
  aLineX->SetLineColor(1);
  aLineX->SetLineStyle(3);
  aLineX->Draw();

  xmin = mg->GetXaxis()->GetXmin();
  xmax = mg->GetXaxis()->GetXmax();
  auto aLineY = new TLine(xmin, 0, xmax, 0);
  aLineY->SetLineColor(1);
  aLineY->SetLineStyle(3);
  aLineY->Draw();
}


void RPresolution()
{

  UInt_t   nbin = 10;
  Double_t dbin = 6.4/(Double_t)nbin;

  Int_t    mtrack;
  Int_t    mtrack_1;
  Int_t    mtrack_2;
  Int_t ntrack[7];
  Double_t aoq;
  Double_t z;

  TVector2 *unitP_lang  =NULL;
  TVector2 *unitP_1=NULL;
  TVector2 *unitP_2=NULL;

  TBranch  *bunitP_lang;
  TBranch  *bunitP_1;
  TBranch  *bunitP_2;
  TBranch *brpphi=0;
  TBranch *biphi=0;
  TBranch *bdeltphi=0;


  for(Int_t m = m_bgn; m < m_end; m++){

    rChain[m]->SetBranchAddress("ntrack",ntrack);
    rChain[m]->SetBranchAddress("aoq",&aoq);
    rChain[m]->SetBranchAddress("z",&z);

    rChain[m]->SetBranchAddress("unitP_lang",&unitP_lang,&bunitP_lang);
    rChain[m]->SetBranchAddress("unitP_1"   ,&unitP_1,&bunitP_1);
    rChain[m]->SetBranchAddress("unitP_2"   ,&unitP_2,&bunitP_2);
    rChain[m]->SetBranchAddress("mtrack"    ,&mtrack);
    rChain[m]->SetBranchAddress("mtrack_1"  ,&mtrack_1);
    rChain[m]->SetBranchAddress("mtrack_2"  ,&mtrack_2);



    auto nevt = rChain[m]->GetEntries();


    vector< Double_t > alldphi;
    vector< vector<Double_t> > dphi(nbin);
    vector< vector<Double_t> > xphi(nbin);
    
    for(auto i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack_1 < 1 || mtrack_2 < 1) continue;

      alldphi.push_back( TVector2::Phi_mpi_pi( abs( unitP_1->Phi() - unitP_2->Phi() ) ) );

      for(auto k = 0; k < nbin; k++){
	if( unitP_lang->Phi() < dbin*(Double_t)(k+1) ){
	  dphi[k].push_back( TVector2::Phi_mpi_pi( abs( unitP_1->Phi() - unitP_2->Phi() ) ) );
	  xphi[k].push_back( unitP_lang->Phi()  );
	  break;
	}
      }
    }

    Double_t *allcos = new Double_t[2];
    allcos = vn(1, alldphi);
    
    cout << " Resolution " << allcos[0]/sqrt(2.) << " +- " << allcos[1] << endl;


    Double_t xval[nbin];
    Double_t xvale[nbin];
    Double_t yval1[nbin];
    Double_t yval1e[nbin];
    Double_t yval2[nbin];
    Double_t yval2e[nbin];

    for(UInt_t j = 0; j < nbin; j++){
      Double_t *getx = new Double_t[2];
      Double_t *gety = new Double_t[2];

      getx   = vMean(xphi[j]);
      gety   = vn(1, dphi[j]);

      xval[j]  = getx[0];
      yval1[j] = gety[0]/sqrt(2.);

      xvale[j]  = getx[1];
      yval1e[j] = gety[1];

    }


    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    ic++;

    auto *gv_v1 = new TGraphErrors(nbin, xval, yval1, xvale, yval1e);
    gv_v1->SetName("gv_v1");


    gv_v1->SetLineColor(4);
    gv_v1->SetMarkerStyle(20);
    gv_v1->SetMarkerColor(4);
    gv_v1->SetTitle("Subevents correlation ; #Phi ; 1/sqrt(2)*< cos(#Delta#Phi) >");
   
    gv_v1->Draw("ALP");

  }
}



void check(UInt_t selid=2)
{
  if(selid > 4 ) return;


  Int_t pcharge = 1;
  if(selid == 0)  pcharge = -1;

  Int_t RPflag = 0;
  if(selid >= 2) RPflag = 10;


  cout << " Particle " << partname[selid] << endl;

  auto h1 = new TH2D("h1"," ycm/ycm_beam ; #Delta#phi ",100,-1.1, 1.5,100,-3.2, 3.2);
  auto h2 = new TH2D("h2"," ycm/ycm_beam ; #Delta#phi ",100,-1.1, 1.5,100,-3.2, 3.2);

  auto h3 = new TH2D("h3"," #theta ; #Delta#phi ",200,0.,1.6,200,-3.2, 3.2);
  auto h4 = new TH2D("h4"," #theta ; #Delta#phi ",200,0.,1.6,200,-3.2, 3.2);


  Double_t rp_res[2] = {0.107183, 0.0003226};
  Double_t rp_rese[2]= {0.69 ,0.71};

  Double_t aoq;
  Double_t z;
  Int_t ntrack[7];
  Int_t mtrack;
  auto aArray = new TClonesArray("STParticle",100);

  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    
    rChain[m]->SetBranchAddress("STParticle",&aArray);
    rChain[m]->SetBranchAddress("ntrack",ntrack);
    rChain[m]->SetBranchAddress("aoq",&aoq);
    rChain[m]->SetBranchAddress("z",&z);
    rChain[m]->SetBranchAddress("mtrack" ,&mtrack);

    vector< vector<Double_t> > bphi  (rp_nbin);
    vector< vector<Double_t> > rpdbin(rp_nbin);



    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;


    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      if(mtrack == 0) continue;

      TIter next(aArray);
      STParticle *aPart = NULL;

      while( (aPart = (STParticle*)next()) ) {

	auto pid = aPart->GetPID();
	auto rapid = aPart-> GetRapidity();
	rapid = (rapid - ycm) / ybeam_cm;

	if(aPart->GetReactionPlaneFlag() >= RPflag){

	  h1 -> Fill(rapid, aPart->GetFlattenMomentum().Phi());
	  h3 -> Fill(aPart->GetFlattenMomentum().Theta(), aPart->GetFlattenMomentum().Phi());

	  if(pid == partid[selid] && aPart->GetCharge() == pcharge){

	    h2 -> Fill(rapid, aPart->GetFlattenMomentum().Phi());
	    h4 -> Fill(aPart->GetFlattenMomentum().Theta(), aPart->GetFlattenMomentum().Phi());
	  }
	}
      }
    }

    UInt_t id = 1;
    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);

    cc[ic]->Divide(1,2);
    id = 1;
    cc[ic]->cd(id); id++;
    h1 -> Draw("colz");
    
    cc[ic]->cd(id); 
    h2 -> Draw("colz");

    ic++;

    id = 1;
    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    cc[ic]->Divide(1,2);
    
    cc[ic]->cd(id); id++;
    h3 -> Draw("colz");
    
    cc[ic]->cd(id); 
    h4 -> Draw("colz");

    ic++;
  }
}

void PtDependece(UInt_t hrm = 1)
{

  cout << " PtDependence v"<<hrm << endl;

  UInt_t   selid[3] = {2,3,4};

  auto aArray = new TClonesArray("STParticle",100);

  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    rChain[m]->SetBranchAddress("STParticle",&aArray);


    vector< vector< vector<Double_t> > > bphi;
    vector< vector< vector<Double_t> > > ptbin;
    bphi.resize(3);
    ptbin.resize(3);

    for(UInt_t im = 0; im < 3; im++){
      bphi[im] .resize(pt_nbin);
      ptbin[im].resize(pt_nbin);
    }


    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      TIter next(aArray);
      STParticle *aPart = NULL;

      while( (aPart = (STParticle*)next()) ) {

	auto pid   = aPart->GetPID();
	auto rapid = aPart-> GetRapidity();
	rapid = (rapid - ycm)/ybeam_cm;


	if(  aPart->GetBestTrackFlag() > 0 && (rapid < rapid_max && rapid > rapid_min) ){

	  auto pt = aPart->GetRotatedMomentum().Pt();

	  UInt_t k = 0; 
	  while( k < pt_nbin ){
	    if( pt < pt_min + pt_dbin*(Double_t)(k+1) )
	      break;
	    
	    k++;
	  }

	  //	  cout << " K " << k << " " << pt << endl;

	  if(k < pt_nbin){
	    for(UInt_t j = 0; j < 3; j++){
	      if( pid == partid[selid[j]] ){
		bphi[j][k].push_back(aPart->GetAzmAngle_wrt_RP());
		ptbin[j][k].push_back(pt);
		
	      }
	    }
	  }
	}
      }
    }

    Double_t xval [3][pt_nbin];
    Double_t xvale[3][pt_nbin];
    Double_t yval [3][pt_nbin];
    Double_t yvale[3][pt_nbin];

    for(UInt_t j = 0; j < 3; j++) {
      for(UInt_t jn = 0; jn < pt_nbin; jn++) {

	auto getx = vMean(ptbin[j][jn]);
	auto gety = vn(hrm, bphi[j][jn]);

	xval [j][jn] = getx[0];
	xvale[j][jn] = getx[1];

	yval [j][jn] = gety[0];
	yvale[j][jn] = gety[1];

      }
    }
    

    const UInt_t nsl = 11;
    UInt_t iisl = 0;
    auto gr0 = new TGraphErrors(nsl, &xval[0][iisl], &yval[0][iisl], &xvale[0][iisl], &yvale[0][iisl]);
    gr0->SetName("gr0");
    gr0->SetLineColor(2);
    gr0->SetMarkerStyle(20);
    gr0->SetMarkerColor(2);

    auto gr1 = new TGraphErrors(nsl, &xval[1][iisl], &yval[1][iisl], &xvale[1][iisl], &yvale[1][iisl]);
    gr1->SetName("gr1");
    gr1->SetLineColor(4);
    gr1->SetMarkerStyle(21);
    gr1->SetMarkerColor(4);

    auto gr2 = new TGraphErrors(nsl, &xval[2][iisl], &yval[2][iisl], &xvale[2][iisl], &yvale[2][iisl]);
    gr2->SetName("gr2");
    gr2->SetLineColor(8);
    gr2->SetMarkerStyle(22);
    gr2->SetMarkerColor(8);


    mg = new TMultiGraph();
    TString aTitle = Form("; Pt[MeV/c]; v%d(a.u.)",hrm); 
    mg->SetTitle(aTitle);
    mg->SetName("mg");
    mg->Add(gr0,"lp");
    mg->Add(gr1,"lp");
    mg->Add(gr2,"lp");

    // Legend                                                                                                                               

    TString sconf = Form("%4.2f < y < %4.2f",rapid_min,rapid_max);
    auto aLeg = new TLegend(0.1,0.7,0.35,0.9,sconf);
    aLeg->AddEntry(gr0,partname[selid[0]],"lp");
    aLeg->AddEntry(gr1,partname[selid[1]],"lp");
    aLeg->AddEntry(gr2,partname[selid[2]],"lp");


    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    //    gr0->Draw();

    mg  ->Draw("a");
    aLeg->Draw();

    ic++;
  }
}


void YDependece(UInt_t hrm=1)
{
  cout << " YDependece v"<< hrm  << endl;

  UInt_t   selid[3] = {2,3,4};

  auto aArray = new TClonesArray("STParticle",100);

  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    rChain[m]->SetBranchAddress("STParticle",&aArray);

    vector< vector< vector<Double_t> > > bphi;
    vector< vector< vector<Double_t> > > xbin;
    bphi.resize(3);
    xbin.resize(3);

    for(UInt_t im = 0; im < 3; im++){
      bphi[im].resize(rp_nbin);
      xbin[im].resize(rp_nbin);
    }


    Int_t nevt = rChain[m]->GetEntries();
    cout << " Number of events " << nevt << endl;

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);

      TIter next(aArray);
      STParticle *aPart = NULL;

      while( (aPart = (STParticle*)next()) ) {

	auto pid   = aPart->GetPID();
	auto rapid = aPart-> GetRapidity();
	rapid = (rapid - ycm)/ybeam_cm;

	if(  aPart->GetBestTrackFlag() > 0  ){

	  UInt_t k = 0; 
	  while( k < rp_nbin ){
	    if( rapid < rp_min + rp_dbin*(Double_t)(k+1) )
	      break;
	    
	    k++;
	  }

	  if(k < rp_nbin){
	    for(UInt_t j = 0; j < 3; j++){
	      if( pid == partid[selid[j]] ){
		bphi[j][k].push_back(aPart->GetAzmAngle_wrt_RP());
		xbin[j][k].push_back(rapid);
		
	      }
	    }
	  }
	}
      }
    }

    Double_t xval [4][rp_nbin];
    Double_t xvale[4][rp_nbin];
    Double_t yval [4][rp_nbin];
    Double_t yvale[4][rp_nbin];

    for(UInt_t j = 0; j < 3; j++) {
      for(UInt_t jn = 0; jn < rp_nbin; jn++) {

	auto getx = vMean(xbin[j][jn]);
	auto gety = vn(hrm, bphi[j][jn]);

	xval [j][jn] = getx[0];
	xvale[j][jn] = getx[1];

	yval [j][jn] = gety[0];
	yvale[j][jn] = gety[1];

      }
    }

    // reversed
    for(UInt_t jn = 0; jn < rp_nbin; jn++) {
      xval [3][jn] = -xval[0][jn];
      xvale[3][jn] = xvale[0][jn];

      yval [3][jn] =  yval[0][jn] * pow(-1, hrm);
      yvale[3][jn] = yvale[0][jn];
    }
    

    const UInt_t nsl = 10;
    UInt_t iisl = 2;
    auto gr0 = new TGraphErrors(nsl, &xval[0][iisl], &yval[0][iisl], &xvale[0][iisl], &yvale[0][iisl]);
    gr0->SetName("gr0");
    gr0->SetLineColor(2);
    gr0->SetMarkerStyle(20);
    gr0->SetMarkerColor(2);

    auto gr1 = new TGraphErrors(nsl, &xval[1][iisl], &yval[1][iisl], &xvale[1][iisl], &yvale[1][iisl]);
    gr1->SetName("gr1");
    gr1->SetLineColor(4);
    gr1->SetMarkerStyle(21);
    gr1->SetMarkerColor(4);

    auto gr2 = new TGraphErrors(nsl, &xval[2][iisl], &yval[2][iisl], &xvale[2][iisl], &yvale[2][iisl]);
    gr2->SetName("gr2");
    gr2->SetLineColor(8);
    gr2->SetMarkerStyle(22);
    gr2->SetMarkerColor(8);

    auto *rv_gr0 = new TGraphErrors(nsl, &xval[3][iisl], &yval[3][iisl], &xvale[3][iisl], &yvale[3][iisl]);
    rv_gr0->SetName("rv_gv0");
    rv_gr0->SetLineColor(2);
    rv_gr0->SetMarkerStyle(24);
    rv_gr0->SetMarkerColor(2);
    rv_gr0->SetLineStyle(3);


    mg = new TMultiGraph();
    TString aTitle = Form("; Ycm/Ycm_beam; v%d(a.u.)",hrm);
    mg->SetTitle(aTitle);
    mg->SetName("mg");
    mg->Add(gr0,"lp");
    mg->Add(gr1,"lp");
    mg->Add(gr2,"lp");
    mg->Add(rv_gr0,"lp");

    if(hrm == 2)
      mg->SetMaximum(0.001);

    // Legend                                                                                                                               

    TString sconf = ""; //Form("%4.2f < y < %4.2f",rapid_min,rapid_max);
    auto aLeg = new TLegend(0.1,0.7,0.35,0.9,sconf);
    aLeg->AddEntry(gr0,partname[selid[0]],"lp");
    aLeg->AddEntry(gr1,partname[selid[1]],"lp");
    aLeg->AddEntry(gr2,partname[selid[2]],"lp");
    aLeg->AddEntry(rv_gr0,partname[selid[0]]+" reversed","lp");

    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    //    gr0->Draw();

    mg  ->Draw("a");
    aLeg->Draw();

    DrawCenterLine();

    ic++;
  }
}



