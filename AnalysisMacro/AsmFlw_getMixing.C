#include "AsmFlw_getMixing.h"
//----------------------------------------------------------------------
//   AsmFlw_getMixing.C
// (c) Mizuki Kurata-Nishimura 
//----------------------------------------
// An input file comes from Assemble_flwv1.C (ASMV=1)
// An input file comes from Assemble_flwv2.C (ASMV=2) since 11 April 2017
//
// RUN=#### MIX=0 root AsmFlw_getMixing.C or AsmFlw_getMixing.C\(100\)
//         Real create a multiplicity distribution  
//----------------------------------------------------------------------
void AsmFlw_getMixing(Int_t nmax = -1)
{
  sRun = gSystem -> Getenv("RUN");
  sAsm = gSystem -> Getenv("ASMV");
  sVer = gSystem -> Getenv("VER");
  

  TString sMix = gSystem -> Getenv("MIX");

  if(sRun =="" || sVer == "" ||sMix == "" ) {
    cout << " Please type " << endl;
    cout << "$ RUN=#### ASMV= 1 or 2(-2: w/o rotation) MIX=0(real) or 1(mix) VER=#.# root AsmFlw_getMixing(Number of event) " << endl;
    exit(0);
  }

  // Set default to 2.
  if( sAsm == "" )  sAsm = "2";

  if( sAsm == "2")
    BeamAngle = kTRUE;
  else if( sAsm == "-2"){
    sAsm = 2;
    BeamAngle = kFALSE;
  }
  else {
    sAsm = "";
    BeamAngle = kFALSE;
  }

  cout << " BeamAngle : " << BeamAngle << endl;

  if (sMix == "1") bMix = kTRUE;
  else bMix = kFALSE;

  iRun = atoi(sRun);

  cout << "RUN = " << sRun << " Assemble v" << sAsm 
       << " with ver " << sVer  << " mix = " << sMix << endl;

  Open();
  Int_t mEvt = GetMultiplicityDistribution();
  OutputTree(nmax);

  
  //--------------------start
  //
  TDatime dtime;
  TDatime btime(dtime);
  //  TRandom2 pran;

  nEntry = fChain->GetEntries();
  std::cout << " Number of Events " << nEntry << std::endl;

  if( bMix ) {
    if( nmax == -1 )  nmax = mEvt;
    else if( nmax == -2 ) nmax = mEvt*2;
  }
  else 
    if( nmax == -1 || nmax > nEntry )  nmax = nEntry; 

  std::cout << " Maximum Events " << nmax << std::endl;

  for(Int_t ievt = 0; ievt < nmax; ievt++){

    Initialize();

    if(ievt%1000 == 0) {
      dtime.Set();
      Int_t ptime = dtime.Get() - btime.Get();
      std::cout << "Process " << ievt << "/"<< nmax << " = " 
		<< ((Double_t)(ievt)/(Double_t)nmax)*100. << " % --->"
		<< dtime.AsString() << " ---- "
	        << (Int_t)ptime/60 << " [min] "
                << std::endl;
    }
   
    Int_t nGoodTrack = 0;
    if(bMix)
      nGoodTrack = (Int_t)GetRandomNumTrack(); // get number of track 
    else {
      fChain->GetEntry(ievt);
      nGoodTrack = aParticleArray->GetEntries();
    }


    Int_t nLoop = 0;

    while(kTRUE) {


      if(  bMix && numGoodTrack > nGoodTrack - 1 ) break;  // mGoodTrack-1
      if( !bMix && nLoop >=  nGoodTrack) break;


      STParticle *aPart1;
      Int_t mixEvt = ievt;

      //      cout << " event " << ievt << endl;

      if( bMix ) aPart1 = GetMixedTrack(&mixEvt); // Get a track randomly
      else       aPart1 = (STParticle*) aParticleArray -> At(nLoop);
      

      // updated a new version on 3 April 2017.
      TVector3 pp(aPart1->GetMomentum().X(),
		  aPart1->GetMomentum().Y(),
		  aPart1->GetMomentum().Z());


      TVector2 ppt(aPart1->GetMomentum().X(),
		   aPart1->GetMomentum().Y());

      Double_t vpt = pp.Pt();

      Double_t psdrapid = -log( tan((pp.Theta()/2)) );

      TVector3 pp_rot;
      TVector2 ppt_rot;
    // Rotate along beam angle
      if( BeamAngle ){
	if( !bMix ){  
	  aX = ProjA/1000.;
	  bY = ProjB/1000.;
	}
	
	pp_rot = pp; 
	pp_rot.RotateY(-aX);
	pp_rot.RotateX(-bY);

	ppt_rot = TVector2(pp_rot.X(), pp_rot.Y());

	//	vpt = pp_rot.Pt();
      }
      
	// cout << "vpt " << vpt 
	//      << " aPart1->GetBestTrackFlag() "<< aPart1->GetBestTrackFlag() 
	//      << endl;

      if(aPart1 && aPart1->GetBestTrackFlag() && aPart1->GetLinearPID()>1000
       	 && vpt < 500){


	numGoodTrack++;
	if( bMix ) event.push_back(mixEvt);
	else event.push_back(ievt);

	pid.push_back(aPart1->GetLinearPID());
	vPart.push_back(pp);
	iphi.push_back(pp.Phi());
	px.push_back(pp.X());
	py.push_back(pp.Y());
	pz.push_back(pp.Z());
	pt.push_back(ppt);
	mom.push_back(aPart1->GetP());
	dedx.push_back(aPart1->GetTotaldEdx());
	rapid.push_back(aPart1->GetRapidity());
	etot.push_back(aPart1->GetEtotal());
	prapid.push_back(psdrapid);


	//rotated p
	if( BeamAngle ) {
	  TClonesArray &bp_rot = *p_rot;
	  new(bp_rot[numGoodTrack-1]) TVector3(pp_rot);
	  pt_rot.push_back(ppt_rot);

	  theta_xz.push_back(aX);
	  theta_yz.push_back(bY);
	}
	



	unitP += pp.Unit();
    	
	//	cout << " ntrack " << numGoodTrack << endl;
	if( pp.Z() < 400){
	  unitP_t += ppt.Unit();
	  mtrack_t++;	    
	}
	else {
	  unitP_b += ppt.Unit();
	  mtrack_b++;

	}

	if(aPart1->GetMomentum().Theta()<0.4) {
	  unitP_lang += pp.Unit();
	} 
      }    
      nLoop++;
    }


    // Rapidity distribution //
    mtrack = vPart.size();
    for(Int_t i = 0; i < mtrack; i++){

      TVector2 mExcRP(0.,0.);
      TVector2 mExcRP_b(0.,0.);
      TVector2 mExcRP_t(0.,0.);

      for(Int_t k = 0; k < mtrack; k++){

     	if(i != k ) {
	  mExcRP += pt.at(k).Unit();
          hRPrapd->Fill(prapid.at(k));

	  if( pz.at(k) < 400){
	    mExcRP_t += pt.at(k).Unit(); 
	    hRPrapd_t->Fill(prapid.at(k));
	  }
	  else {
	    mExcRP_b += pt.at(k).Unit(); 
	    hRPrapd_b->Fill(prapid.at(k));
	  }
	}
      }

      deltphi.push_back(  TVector2::Phi_mpi_pi( pt.at(i).Phi()-mExcRP.Phi()));
      deltphi_t.push_back(TVector2::Phi_mpi_pi( pt.at(i).Phi()-mExcRP_t.Phi()));
      deltphi_b.push_back(TVector2::Phi_mpi_pi( pt.at(i).Phi()-mExcRP_b.Phi()));
      
      Double_t rpx = pt.at(i).Mod() * cos( pt.at(i).Phi() - mExcRP_b.Phi() );
      hRPpxsr_b->Fill(prapid.at(i), rpx);
      rpxb.push_back(rpx);

      rpx = pt.at(i).Mod() * cos( pt.at(i).Phi() - mExcRP_t.Phi() );
      hRPpxsr_t->Fill(prapid.at(i), rpx);
      rpxt.push_back(rpx);
    }

    hm_b->Fill(mtrack_b);
    hm_t->Fill(mtrack_t);
    
    mflw->Fill();

    if(numGoodTrack>0)  hgtc->Fill(numGoodTrack);
  }

  if(!bMix && mhfile != NULL) {
    mhfile->cd();
    hgtc->Write();
  }

  fout->cd();
  fout->Write();
  std::cout << fout->GetName() << std::endl;

  if(gROOT->IsBatch()) {
    fout->Close();
    exit(0);
  }
}

void Open()
{

  fChain = new TChain("flw");
  TString fn;
  if(sAsm == "1")
    fn = Form("../data/run%d_flw_v"+sVer(0,1)+".root",iRun);
  else
    fn = Form("../data/run%d_flw_"+sAsm+"v"+sVer(0,1)+".root",iRun);


  fChain -> Add(fn);
  if(!fChain) {
    cout << " No data was found " << fn << endl;
    exit(0);
  }
  cout << "Input file is " << fn << endl;


  aParticleArray   = new TClonesArray("STParticle",150);
  mixParticleArray = new TClonesArray("STParticle",150);

  fChain->SetBranchAddress("STParticle",&aParticleArray);
  fChain->SetBranchAddress("ntrack",ntrack);
  fChain->SetBranchAddress("kymult",&kymult);

 if( BeamAngle ){
    fChain->SetBranchAddress("ProjA",&ProjA);
    fChain->SetBranchAddress("ProjB",&ProjB);
    fChain->SetBranchAddress("aoq",&aoq);
    fChain->SetBranchAddress("z",&z);

    p_rot  = new TClonesArray("TVector3",150);
  }


}

void Initialize()
{
  
  unitP   = TVector3(0,0,0);
  unitP_b = TVector2(0,0);
  unitP_t = TVector2(0,0);
  unitP_lang  = TVector3(0,0,0);
  
  numGoodTrack = 0;
  for(Int_t i = 0; i< 7; i++) ntrack[i];
  
  mtrack = 0;
  mtrack_t = 0;
  mtrack_b = 0;
  event.clear();
  pid.clear();
  vPart.clear();
  iphi.clear();
  pt.clear();
  px.clear();
  py.clear();
  pz.clear();
  mom.clear();
  dedx.clear();
  deltphi.clear();
  deltphi_t.clear();
  deltphi_b.clear();
  iphi.clear();
  rapid.clear();
  prapid.clear();
  rpxt.clear();
  rpxb.clear();
  etot.clear();

  if( BeamAngle ){
    pt_rot.clear();
    p_rot->Clear();

    theta_xz.clear();
    theta_yz.clear();
  }

  aParticleArray->Clear();
  mixParticleArray->Clear();
}

Int_t GetMultiplicityDistribution()
{
  // Multplicity histgram
  TString mHistFile = Form("run%d.ngt_v"+sVer(0,1)+".root",iRun);
  TString hf = mHistFile;

  Int_t nEvt = 0;

  if(bMix) { 
    mhfile = new TFile(mHistFile);
    if( mhfile == NULL ) {
      cout << mHistFile << " is not found." << endl;
      exit(0);
    }
    cout << "Multiplicity : "  << mHistFile << " is loaded." << endl;


    if( (TH1I*)mhfile->FindObject("hgtc") ) 
      exit(0);
    
    histGT_r =  (TH1I*)mhfile->Get("hgtc");
    histGT_r -> SetName("hgtc_r");
    histGT_r -> SetDirectory(gROOT);
    nEvt = histGT_r -> GetEntries();
  }
  else {
    //    if( !gSystem->FindFile(".",hf) ) {
      mhfile = new TFile(mHistFile,"recreate");
      cout << mHistFile << " is created." << endl;
      //    }
    // else
    //   cout << mHistFile << " is existing." << endl;
  }

  return nEvt;

}

void OutputTree(Int_t nmax)
{

  // ROOT out
  TString sdeb = ".s";
  if(nmax < 0)  sdeb = "";
  TString foutname = "../data/run"+sRun+"_";

  if( bMix ) {
    foutname += "mxflw";
    foutname += sAsm+"v"+sVer+".root";
  }
  else {
    foutname += "rdflw";
    foutname += sAsm+"v"+sVer+sdeb+".root";
  }
  
  TString fo = foutname;

  if( !gROOT->IsBatch() && gSystem->FindFile(".",fo) ) {
    cout << foutname << " is existing. Do you recreate? (y/n)" << endl;
    TString sAns;
    cin >> sAns;
    if(sAns == "y" || sAns == "Y")
      fout  = new TFile(foutname,"recreate");
    else {
      cout << " Retry" << endl;
      exit(0);
    }      
  }
  else
    fout  = new TFile(foutname,"recreate");



  if( bMix ) 
    mflw = new TTree("mflw","FLOW analysis track mixiing");    
  else
    mflw = new TTree("rflw","FLOW analysis");

  cout << "Output file is " << foutname << endl;

  //-- output                                                                                                              
  mflw->Branch("irun",&iRun,"irun/I");
  mflw->Branch("numGoodTrack",&numGoodTrack);
  mflw->Branch("mtrack",&mtrack,"mtrack/I");
  mflw->Branch("mtrack_t",&mtrack_t,"mtrack_t/I");
  mflw->Branch("mtrack_b",&mtrack_b,"mtrack_b/I");
  mflw->Branch("event",&event);
  mflw->Branch("pid",&pid);
  mflw->Branch("px",&px);
  mflw->Branch("py",&py);
  mflw->Branch("pz",&pz);
  mflw->Branch("mom",&mom);
  mflw->Branch("dedx",&dedx);
  mflw->Branch("deltphi",&deltphi);
  mflw->Branch("deltphi_t",&deltphi_t);
  mflw->Branch("deltphi_b",&deltphi_b);
  mflw->Branch("iphi",&iphi);
  mflw->Branch("rapid",&rapid);
  mflw->Branch("prapid",&prapid);
  mflw->Branch("rpxt",&rpxt);
  mflw->Branch("rpxb",&rpxb);
  mflw->Branch("etot",&etot);
  mflw->Branch("unitP",&unitP);
  mflw->Branch("unitP_b",&unitP_b);
  mflw->Branch("unitP_t",&unitP_t);
  mflw->Branch("unitP_lang",&unitP_lang);
  mflw->Branch("kymult",&kymult,"kymult/I");

  if( BeamAngle ) {
    mflw->Branch("p_rot" ,&p_rot);
    mflw->Branch("theta_xz",&theta_xz);
    mflw->Branch("theta_yz",&theta_yz);
  }

  
  histGT       = new TH1I("histGT","generated tracks",50,0,50);
  histMixEvt   = new TH1I("histMixEvt","mixed event ID",10000,0,10000);
  histMixTrack = new TH1I("histMixTrack","mixed number of track",100,0,100);
  hgtc         = new TH1I("hgtc","number of good fragment",100,0,100);
  hRPrapd      = new TH1D("hRPrapd","Rapidity region for R.P. all",100,0.,5);
  hRPrapd_t    = new TH1D("hRPrapd_t","Rapidity region for R.P. y>0.4",100,0.,3.5);
  hRPrapd_b    = new TH1D("hRPrapd_b","Rapidity region for R.P. y<-0.4",100,0.,3.5);
  hm_t         = new TH1I("hm_t","Multi. y>0.4",50,0,50);
  hm_b         = new TH1I("hm_b","Multi. y>0.4",50,0,50);
  hRPpxsr_b    = new TH2D("hRPpxsr_b","px vs psude Rapidity beam",100,0.,3.5, 100,-500,500);
  hRPpxsr_t    = new TH2D("hRPpxsr_t","px vs psude Rapidity target",100,0.,3.5, 100,-500,500);
}

Int_t GetRandomNumTrack()
{
  if(!histGT){
    cout << " no histgram was found for randowm number " << endl;
    return 0;
  }

  Int_t bcont = (Int_t)histGT_r->GetRandom();
  histGT->Fill(bcont);

  return bcont;
}

STParticle *GetMixedTrack(Int_t *ival)
{
  STParticle *mixPart = NULL;
  static Int_t mevt = *ival;
    //(Int_t)pran.Uniform(0,nEntry);

  if(mevt >= nEntry ) mevt = 0;
  //  cout << " Event in " << mevt << endl;  

  while(kTRUE){	
    //    mevt++;
    mevt += 2;

    if(mevt > nEntry ) mevt = 0;
    fChain->GetEntry(mevt);

    Int_t nmixTrack = aParticleArray->GetEntries();
    Int_t imixTrack = (Int_t)pran.Uniform(0,nmixTrack);

    if(sAsm == "2") {
      aX = ProjA/1000.;
      bY = ProjB/1000.;
    }
	  
    // cout << " Event " << mevt  
    // 	 << "  imixTrack/nmixTrack " << imixTrack << "/" << nmixTrack << endl;

    if(imixTrack < aParticleArray -> GetEntries() ) {
      mixPart = (STParticle*) aParticleArray->At(imixTrack);	        
      histMixEvt  ->Fill(mevt);
      histMixTrack->Fill(nmixTrack);
      break;
    }
  }
  *ival = mevt; 
  return mixPart;
}
