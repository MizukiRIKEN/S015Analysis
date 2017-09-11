#include "STFlowCorrection.hh"

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

const UInt_t nbin = 16;
Double_t rpdbin[nbin];
Double_t rp_min = -1.;
Double_t rp_max = 1.6;
Double_t rp_dbin = (rp_max - rp_min)/(Double_t)nbin;

TString  partname[] = {"pi-","pi+","proton","deutron","triton"};
UInt_t   partid[] = {211, 211, 2212, 1000010020, 1000010030};
  


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


Int_t    mtrack;
Int_t    mtrack_1;
Int_t    mtrack_2;

vector<Double_t> *deltphi=0;
TClonesArray     *p_org=0;


TVector2 *unitP_lang  =NULL;
TVector2 *unitP_1=NULL;
TVector2 *unitP_2=NULL;

TBranch  *bunitP_lang;
TBranch  *bunitP_1;
TBranch  *bunitP_2;
TBranch *brpphi=0;
TBranch *biphi=0;
TBranch *bdeltphi=0;

UInt_t im = 0;
UInt_t m_bgn = 0;
UInt_t m_end = 2;

TGraphErrors *gv_v1;
TGraphErrors *gv_v2;




void Slice2D(Int_t nval=30);
void v1(Int_t nval=30);
void v2(Int_t nval=30);

void calcCRDvsMX()
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
  
  cout << " vphi size " << vphi.size() << endl;
  Double_t findx = (Double_t)hm;
  for(UInt_t i = 0; i < (UInt_t)vphi.size(); i++)
    fvCos.push_back(cos(findx * vphi.at(i)));

  vector<Double_t>::iterator ibgn = fvCos.begin();
  vector<Double_t>::iterator iend = fvCos.end();

  Double_t *vcos = new Double_t[2];
  vcos[0]       =  TMath::Mean(ibgn, iend);
  Double_t rms  =  TMath::RMS(ibgn, iend);
  rms *= rms;
  Double_t stv2 = (rms - vcos[0]*vcos[0]);

  vcos[1]       = sqrt(stv2)/sqrt(2.*( (Double_t)vphi.size() - 1 ) );

  if(vphi.size() < 2) {
    vcos[0] = -999.;
    vcos[1] = 0.;
  }

  //  cout << hm << " Mean " << vcos[0] << " RMS " << vcos[1] << endl; 
  return vcos;
}
  


void plotv1v2(UInt_t selid=2)
{
  if(selid > 4 ) return;


  Int_t pcharge = 1;
  if(selid == 0)  pcharge = -1;

  Int_t RPflag = 0;
  if(selid >= 2) RPflag = 10;


  cout << " Particle " << partname[selid] << endl;

  auto haccp = new TH2D("haccp",partname[selid]+";ycm/ycm_beam ; Pt [MeV/c]",100,-1.1, 1.5,100,0.,800.);
  auto haccx = new TH2D("haccx",partname[selid]+";ycm/ycm_beam ; Px [MeV/c]",100,-1.1, 1.5,100,-500,500.);


  Double_t rp_res[2] = {0.107183, 0.0003226};
  Double_t rp_rese[2]= {0.69 ,0.71};

  Double_t aoq;
  Double_t z;
  

  Int_t ntrack[7];

  auto aArray = new TClonesArray("STParticle",100);

  UInt_t ic = 0;
  UInt_t id = 1;
  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    
    rChain[m]->SetBranchAddress("STParticle",&aArray);
    rChain[m]->SetBranchAddress("ntrack",ntrack);
    rChain[m]->SetBranchAddress("aoq",&aoq);
    rChain[m]->SetBranchAddress("z",&z);

    rChain[m]->SetBranchAddress("mtrack" ,&mtrack);

    // rChain[m]->SetBranchAddress("fiphi"   ,&iphi     ,&biphi);
    // rChain[m]->SetBranchAddress("frpphi"  ,&rpphi    ,&brpphi);
    // rChain[m]->SetBranchAddress("fdeltphi",&deltphi  ,&bdeltphi);
    // rChain[m]->SetBranchAddress("fPID"    ,&pid      ,&bpid);
    // rChain[m]->SetBranchAddress("frapid"  ,&rapid    ,&brapid);

    vector< vector<Double_t> > bphi(nbin);
    vector< vector<Double_t> > rpdbin(nbin);


    for(UInt_t k = 0; k < nbin; k++){
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


	//	if(pid == partid[selid] && aPart->GetCharge() == pcharge && aPart->GetReactionPlaneFlag() > RPflag){
	if(pid == partid[selid] && aPart->GetCharge() == pcharge ){

	  auto rapid = aPart-> GetRapidity();

	  //	  if( rapid < 0.2 ) continue;

	  rapid = (rapid - ycm) / ybeam_cm;

	  //	  haccp -> Fill(rapid, aPart->GetFlattenMomentum().Mag());
	  haccp -> Fill(rapid, aPart->GetRotatedMomentum().Pt());
	  haccx -> Fill(rapid, aPart->GetFlattenMomentum().X());
	  
	  for(UInt_t k = 0; k < nbin; k++){

	    if(rapid < rp_min + rp_dbin*(Double_t)(k+1)) {
	      bphi[k].push_back(aPart->GetAzmAngle_wrt_RP());
	      rpdbin[k].push_back(rapid);

	      
	      break;
	    }
	  }
	}
      }
    }

    Double_t xval[nbin];
    Double_t xvale[nbin];
    Double_t yval1[nbin];
    Double_t yval1e[nbin];
    Double_t yval2[nbin];
    Double_t yval2e[nbin];

    UInt_t j =0;
    UInt_t jn = 0;
    while(jn < nbin){

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

    for(UInt_t i = 0; i < nbin; i++)
      cout << i << xval[i] << " +-" <<  " v1 " << yval1[i] << " +- " << yval1e[i] << "  w  " << bphi[i].size() << endl;  
    for(UInt_t i = 0; i < nbin; i++)
      cout << i << xval[i] << " +-" <<  " v2 " << yval2[i] << " +- " << yval2e[i] << "  w  " << bphi[i].size() << endl;  

    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    ic++;
    //    cc[ic]->Divide(1,2); ic++;


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
    gv_v1 = new TGraphErrors(nsl, rap, v1, rape, v1e);
    gv_v1->SetName("gv_v1");


    // gv_v1->SetMaximum(0.1);
    // gv_v1->SetMinimum(-0.1);
    gv_v1->SetLineColor(4);
    gv_v1->SetMarkerStyle(20);
    gv_v1->SetMarkerColor(4);
    gv_v1->SetTitle(partname[selid]+"; Ycm/Ycm_beam ; v1(a.u.)");
   
    gv_v1->Draw("ALP");
    
    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    ic++;

    
    gv_v2 = new TGraphErrors(nsl, rap, v2, rape, v2e);
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

void plotv1(UInt_t selid0=2, UInt_t selid1=3)
{
  if(selid0 > 4 ) return;
  if(selid1 > 4 ) return;

  UInt_t selid[2];
  selid[0] = selid0;
  selid[1] = selid1;


  Int_t pcharge[2] = {1, 1};
  if(selid[0] == 0)  pcharge[0] = -1;
  if(selid[1] == 0)  pcharge[1] = -1;

  //cout << "pcharge " << pcharge[0] << " " << pcharge[1] << endl;

  Int_t RPflag = 0;
  if(selid[0] >= 2) RPflag = 10;


  cout << " Particle " << partname[selid[0]] 
       << " &  " << partname[selid[1]] << " " << RPflag << endl;

  Double_t rp_res[2] = {0.107183, 0.0003226};
  Double_t rp_rese[2]= {0.69 ,0.71};


  Double_t aoq;
  Double_t z;
  
  Int_t ntrack[7];

  auto aArray = new TClonesArray("STParticle",100);

  UInt_t ic = 0;
  UInt_t id = 1;
  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    
    rChain[m]->SetBranchAddress("STParticle",&aArray);
    rChain[m]->SetBranchAddress("ntrack",ntrack);
    rChain[m]->SetBranchAddress("aoq",&aoq);
    rChain[m]->SetBranchAddress("z",&z);

    rChain[m]->SetBranchAddress("mtrack" ,&mtrack);

    vector< vector<Double_t> > bphi0(nbin);
    vector< vector<Double_t> > rpdbin0(nbin);
    vector< vector<Double_t> > bphi1(nbin);
    vector< vector<Double_t> > rpdbin1(nbin);



    for(UInt_t k = 0; k < nbin; k++){
      bphi0[k].clear();
      rpdbin0[k].clear();
      bphi1[k].clear();
      rpdbin1[k].clear();
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


	if(  aPart->GetReactionPlaneFlag() > RPflag){

	    cout << "pid " << pid << " = " << partid[selid[0]] << " & " << aPart->GetCharge() << " = " <<  pcharge[selid[0]] << endl;
	
	  if(pid == partid[selid[0]] && aPart->GetCharge() == 1 ){

	    //	    cout << partname[selid[0]] << endl;

	    auto rapid = aPart-> GetRapidity();
	    rapid = (rapid - ycm)/ybeam_cm;

	    for(UInt_t k = 0; k < nbin; k++){
	       
	      if(rapid < rp_min + rp_dbin*(Double_t)(k+1)) {
		bphi0[k].push_back(aPart->GetAzmAngle_wrt_RP());
		rpdbin0[k].push_back(rapid);
		break;
	      }
	    }
	  }
	  else if( pid == partid[selid[1]] && aPart->GetCharge() == 1 ){

	    //	    cout << partname[selid[1]] << endl;

	    auto rapid = aPart-> GetRapidity();
	    rapid = (rapid - ycm)/ybeam_cm;

	    for(UInt_t k = 0; k < nbin; k++){	       
	      if(rapid < rp_min + rp_dbin*(Double_t)(k+1)) {
		bphi1[k].push_back(aPart->GetAzmAngle_wrt_RP());
		rpdbin1[k].push_back(rapid);
		break;
	      }
	    }
	    
	  }

	}
      }
    }
    Double_t xval[nbin];
    Double_t xvale[nbin];
    Double_t yval1[nbin];
    Double_t yval1e[nbin];
    Double_t yval2[nbin];
    Double_t yval2e[nbin];

    UInt_t j =0;
    UInt_t jn = 0;
    while(jn < nbin){
      Double_t *getx  = new Double_t[2];
      Double_t *gety0 = new Double_t[2];
      Double_t *gety1 = new Double_t[2];

      getx     = vMean(rpdbin0[j]);

      if(getx[0] > -900){
	xval[j]  = getx[0];
	xvale[j] = getx[1];

	gety0    = vn(1, bphi0[j]);
	gety1    = vn(1, bphi1[j]);
	
	yval1[j] = gety0[0]; ///rp_res[m];
	yval1e[j]= 0.;//gety[1];
	
	yval2[j] = gety1[0]; ///rp_res[m];
	yval2e[j]= 0.;//gety[1];

	cout << j << " y " << yval2[j] << " +- " << yval2e[j] << endl;  
	j++;
      }
      jn++;
    }


    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);
    ic++;
    //    cc[ic]->Divide(1,2); ic++;

    //cc[ic]->cd(id); id++;
    const UInt_t nsl = 10;
    Double_t rap[nsl];
    Double_t rape[nsl];
    Double_t v1[nsl];
    Double_t v1e[nsl];
    Double_t v2[nsl];
    Double_t v2e[nsl];

    for(UInt_t isl = 0; isl < 10; isl++){
      rap[isl]  = xval[isl];
      rape[isl] = xvale[isl];
      
      v1[isl]   = yval1[isl];
      v1e[isl]  = yval1e[isl];
      v2[isl]   = yval2[isl];
      v2e[isl]  = yval2e[isl];

    }


    auto *gv0 = new TGraphErrors(nsl, rap, v1, rape, v1e);
    gv0->SetName("gv0");


    gv0->SetLineColor(2);
    gv0->SetMarkerStyle(20);
    gv0->SetMarkerColor(2);
    gv0->SetTitle(partname[selid[0]]+"; Ycm/Ycm_beam ; v1(a.u.)");
   
    auto *gv1 = new TGraphErrors(nsl, rap, v2, rape, v2e);
    gv1->SetName("gv1");


    gv1->SetLineColor(2);
    gv1->SetMarkerStyle(20);
    gv1->SetMarkerColor(2);
    gv1->SetTitle(partname[selid[1]]+"; Ycm/Ycm_beam ; v1(a.u.)");


    auto mg = new TMultiGraph();
    mg->Add(gv1,"lp");
    mg->Add(gv0,"lp");

    mg->SetMaximum(0.2);
    //    mg->SetMinimum(-0.2);
    mg->Draw("a");


    auto aLeg = new TLegend(0.1,0.8,0.3,0.9);
    aLeg->AddEntry(gv0,partname[selid[0]],"lp");
    aLeg->AddEntry(gv1,partname[selid[1]],"lp");
    aLeg->Draw();
  }
    
}

void RPresolution()
{

  UInt_t   nbin = 10;
  Double_t dbin = 6.4/(Double_t)nbin;



  Int_t ntrack[7];
  Double_t aoq;
  Double_t z;

  UInt_t ic = 0;
  UInt_t id = 1;

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
  auto aArray = new TClonesArray("STParticle",100);

  UInt_t ic = 0;
  UInt_t id = 1;
  for(Int_t m = m_bgn; m < m_end; m++){

    aArray->Clear();
    
    rChain[m]->SetBranchAddress("STParticle",&aArray);
    rChain[m]->SetBranchAddress("ntrack",ntrack);
    rChain[m]->SetBranchAddress("aoq",&aoq);
    rChain[m]->SetBranchAddress("z",&z);
    rChain[m]->SetBranchAddress("mtrack" ,&mtrack);

    vector< vector<Double_t> > bphi(nbin);
    vector< vector<Double_t> > rpdbin(nbin);



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


    cc[ic] = new TCanvas(Form("cc%d",ic),Form("cc%d",ic),700,500);

    cc[ic]->Divide(1,2);
    
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


void SaveGraph(TString fopt = "")
{
  auto gout = new TFile(fopt+".root","recreate");
  if( gv_v1 != NULL )
    gv_v1->Write();
  
  if( gv_v2 != NULL )
    gv_v2->Write();

  gout->Close();
}
