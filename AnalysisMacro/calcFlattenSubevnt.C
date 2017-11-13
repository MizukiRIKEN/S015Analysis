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
void OutputTree(TChain *rCh);
void flatten_Subevent();

void calcFlattenSubevnt()
{
  gROOT->Reset();

  UInt_t im = 0; 
  TString sMix = gSystem -> Getenv("MIX");
  if(sMix != "")
    im = atoi(sMix);


  cout << " im " << im << endl;
 
  if( im < 2){
    gROOT->Macro(Form("openCRM.C(%d)",im));
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


  TString sVer  = gSystem -> Getenv("VER");
  TString ssVer = "";
  TString tVer  = sVer+".";

  for(Int_t i = 0; i < 3; i++){
    Ssiz_t idx = tVer.Index(".");
    ssVer += tVer(0 , (Int_t)idx);
    tVer.Replace(0,(Int_t)(idx+1),"");
  }
  Int_t fVer = atoi(ssVer); 

  dxbin = 1./(Double_t)nbinx; 


  flatten_Subevent();
}


void flatten_Subevent()
{
  UInt_t nbin = 10;
  UInt_t harm = 8;

  const UInt_t mtrknbin=10;
  Double_t mtrkbin[mtrknbin+1];
  Double_t mtrk_min = 0;
  Double_t mtrk_max = 60;
  for(UInt_t n = 0; n < mtrknbin+2; n++)
    mtrkbin[n]   = mtrk_max/mtrknbin * n;

  vector<Int_t>    itrack_str;
  vector<Int_t>    mtrack_str;
  vector<Int_t>    mtrack1_str;
  vector<Int_t>    mtrack2_str;
  vector<Double_t> phi_1;
  vector<Double_t> phi_2;
  vector<Double_t> phi_1s2a;
  vector<Double_t> phi_1s2b;

  TH1D *hbphi[2];
  TH1D *haphi[2];
  TH2D *h2bphi[2];
  TH2D *h2aphi[2];
  TH1D *haphi1[2];
  TH1D *haphi2[2];
  TH1D *haphi0[2];
  TH2D *hmtkA[2];
  TH2D *hmtkB[2];
  TH2D *hmtk1A[2];
  TH2D *hmtk1B[2];
  TH2D *hmtk2A[2];
  TH2D *hmtk2B[2];


  for(Int_t m = m_bgn; m < m_end; m++){


    hbphi[m]  = new TH1D(Form("hbphi%d",m), "before correction; #Delta(#Phi_1 - #Phi_2) [rad] " ,60,-3.2,3.2);
    haphi[m]  = new TH1D(Form("haphi%d",m), "after correction; #Delta(#Phi_1 - #Phi_2) [rad]"   ,60,-3.2,3.2);
    haphi1[m] = new TH1D(Form("haphi1%d",m),"after correction; #Phi_1  [rad]"   ,60,-3.2,3.2);
    haphi2[m] = new TH1D(Form("haphi2%d",m),"after correction; #Phi_2  [rad]"   ,60,-3.2,3.2);
    haphi0[m] = new TH1D(Form("haphi0%d",m),"after correction; #Phi    [rad]"   ,60,-3.2,3.2);
    h2bphi[m] = new TH2D(Form("h2bphi%d",m),"before correction; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] ",60,-3.2,3.2,60,-3.2,3.2);
    h2aphi[m] = new TH2D(Form("h2aphi%d",m),"after correcttion; #Phi_1 [rad]; #Delta(#Phi_1 - #Phi_2) [rad] ",60,-3.2,3.2,60,-3.2,3.2);

    hmtkA[m]  = new TH2D(Form("hmtkA%d",m) ,"before correction; number of track; #Phi",70,0.,70., 60,-3.2,3.2);
    hmtkB[m]  = new TH2D(Form("hmtkB%d",m) ,"after  correction; number of track; #Phi",70,0.,70., 60,-3.2,3.2);
    hmtk1A[m]  = new TH2D(Form("hmtk1A%d",m) ,"before correction; number of track; #Phi_1",30,0.,30., 60,-3.2,3.2);
    hmtk1B[m]  = new TH2D(Form("hmtk1B%d",m) ,"after  correction; number of track; #Phi_1",30,0.,30., 60,-3.2,3.2);
    hmtk2A[m]  = new TH2D(Form("hmtk2A%d",m) ,"before correction; number of track; #Phi_2",30,0.,30., 60,-3.2,3.2);
    hmtk2B[m]  = new TH2D(Form("hmtk2B%d",m) ,"after  correction; number of track; #Phi_2",30,0.,30., 60,-3.2,3.2);

    rChain[m]->SetBranchAddress("mtrack"     ,&mtrack);
    rChain[m]->SetBranchAddress("mtrack_1"   ,&mtrack_1);
    rChain[m]->SetBranchAddress("mtrack_2"   ,&mtrack_2);
    rChain[m]->SetBranchAddress("unitP_rot"  ,&unitP,&bunitP);
    rChain[m]->SetBranchAddress("unitP_1r"   ,&unitP_1,&bunitP_1);
    rChain[m]->SetBranchAddress("unitP_2r"   ,&unitP_2,&bunitP_2);

    phi_1.clear();
    phi_2.clear();
    phi_1s2a.clear();
    phi_1s2b.clear();
    itrack_str.clear();
    mtrack_str.clear();

    STFlowCorrection *flowcorr[mtrknbin+1] ;
    for(UInt_t j = 0; j < mtrknbin+1; j++){
      flowcorr[j] = new STFlowCorrection(rChain[m], harm, m);
      
      flowcorr[j]->SetBin_min(0, mtrkbin[j]);

      if(j < mtrknbin+1)
	flowcorr[j]->SetBin_max(0, mtrkbin[j+1]);

    }


    //@@@@@@@@@@@@@@@@@@@@2
    Int_t nevt = rChain[m]->GetEntries();  

    cout << " Number of events " << nevt << endl;

    for(Int_t i = 0; i < nevt; i++){
      rChain[m]->GetEntry(i);


      if(mtrack_1 < 1 || mtrack_2 < 1) continue;

      UInt_t imtrk  = 0;

      UInt_t j = mtrknbin;
      while(1){ 
	if( mtrack >= mtrkbin[j] ){
	  imtrk = j;
	  break;
	}
	j--;
      }

      //cout << " imtrk " << imtrk << " mtrack " << mtrack << endl;

      Double_t pphi = TVector2::Phi_mpi_pi(unitP->Phi());

      if( imtrk <= mtrknbin)
	flowcorr[imtrk]->Add(mtrack, pphi);     
 
      itrack_str.push_back(imtrk);
      mtrack_str.push_back(mtrack);

      Double_t pphi1 = TVector2::Phi_mpi_pi(unitP_1->Phi());
      phi_1.push_back(pphi1);
      mtrack1_str.push_back(mtrack_1);


      Double_t pphi2 = TVector2::Phi_mpi_pi(unitP_2->Phi());
      phi_2.push_back(pphi2);
      mtrack2_str.push_back(mtrack_2);

      hmtkA[m]  -> Fill(mtrack, pphi);
      hmtk1A[m] -> Fill(mtrack_1, pphi1);
      hmtk2A[m] -> Fill(mtrack_2, pphi2);

    }

    if(phi_1.size() == 0) return;

    cout << "Fourier correction  " << m << endl; 

    for(UInt_t j = 0; j < mtrknbin+1; j++){
      UInt_t nphi = flowcorr[j]->FourierCorrection();
      
      if(nphi == 0){
	std::cout << " no data is stored " << std::endl;
	continue;
      }

      TString comm = Form("m%d:flatten_Subevent; mtrack> %f && mtrack< %f",
			  j,mtrkbin[j],mtrkbin[j+1]);
      cout << "save " << comm << endl;
      flowcorr[j]-> SaveCorrectionFactor(comm);    

      vector<Int_t>    mtk  = flowcorr[j]->GetMTrack();
      vector<Double_t> aphi = flowcorr[j]->GetCorrectedPhi();

      //    cout << "mtrack " << mtrack << endl;
      for(Int_t i = 0; i < aphi.size(); i++){
	haphi0[m]-> Fill(TVector2::Phi_mpi_pi( aphi.at(i) ));
	
	hmtkB[m] -> Fill(mtk.at(i), TVector2::Phi_mpi_pi( aphi.at(i) ));
      }
    }
    
    for(Int_t i = 0; i < itrack_str.size(); i++){


      Double_t aphi_1  = flowcorr[itrack_str[i]]->GetCorrection(phi_1.at(i));
      Double_t aphi_2  = flowcorr[itrack_str[i]]->GetCorrection(phi_2.at(i));

      //      cout << " i " << i << " " << aphi_1 << endl;
      h2bphi[m]->Fill( phi_1.at(i),TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));
      h2aphi[m]->Fill(aphi_1,      TVector2::Phi_mpi_pi(aphi_1-aphi_2));
      hbphi[m] ->Fill(TVector2::Phi_mpi_pi( phi_1.at(i)- phi_2.at(i)));
      haphi[m] ->Fill(TVector2::Phi_mpi_pi(aphi_1-aphi_2));

      haphi1[m]->Fill(TVector2::Phi_mpi_pi(aphi_1));
      haphi2[m]->Fill(TVector2::Phi_mpi_pi(aphi_2));

      hmtk1B[m] -> Fill(mtrack1_str.at(i), TVector2::Phi_mpi_pi( aphi_1 ));
      hmtk2B[m] -> Fill(mtrack2_str.at(i), TVector2::Phi_mpi_pi( aphi_2 ));

    }
     

    UInt_t ic = 0;
    UInt_t id = 1;
    cc[ic] = new TCanvas(Form("cc%d",ic), "",1000, 700);

    cc[ic]->Divide(2,2);

    cc[ic]->cd(id); id++;
    h2bphi[m]->Draw("colz");

    cc[ic]->cd(id); id++;
    hbphi[m]->Draw();

    cc[ic]->cd(id); id++;
    h2aphi[m]->Draw("colz");

    cc[ic]->cd(id); id++;
    haphi[m]->Draw();


    ic++; id=1;
    cc[ic] = new TCanvas(Form("cc%d",ic), "",1000, 700);
    cc[ic]->Divide(2,2);

    cc[ic]->cd(id); id++;
    haphi1[m]->Draw();

    cc[ic]->cd(id); id++;
    haphi2[m]->Draw();

    cc[ic]->cd(id); id++;
    haphi0[m]->Draw();

    ic++; id=1;
    cc[ic] = new TCanvas(Form("cc%d",ic), "",1000, 700);
    cc[ic]->Divide(2,3);

    cc[ic]->cd(id); id++;
    hmtkA[m]->Draw("colz");

    cc[ic]->cd(id); id++;
    hmtkB[m]->Draw("colz");

    cc[ic]->cd(id); id++;
    hmtk1A[m]->Draw("colz");

    cc[ic]->cd(id); id++;
    hmtk1B[m]->Draw("colz");

    cc[ic]->cd(id); id++;
    hmtk2A[m]->Draw("colz");

    cc[ic]->cd(id); id++;
    hmtk2B[m]->Draw("colz");

  }
}


void flatten_iphi_mtrkthetabin()
{
  std::cout << "flatten_iphi_thetamtkbin" << std::endl;

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

	if(aPart1->GetReactionPlaneFlag() >= 11 && aPart1->GetReactionPlaneFlag() <= 13){
	//	if(aPart1->GetReactionPlaneFlag() == 11 && aPart1->GetRotatedMomentum().Mag()<2500){
	  
	  Double_t phi   = aPart1->GetRotatedMomentum().Phi();
	  Double_t theta = aPart1->GetRotatedMomentum().Theta();

	  Double_t P     = aPart1->GetRotatedMomentum().Mag();

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
