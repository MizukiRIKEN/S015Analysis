#include "AsmFlw_getMixing.h"
//----------------------------------------------------------------------
//   AsmFlw_getMixing.C
// (c) Mizuki Kurata-Nishimura 
//----------------------------------------
// version 0.0
// An input file comes from Assemble_flwv1.C (ASMV=1)
// version 1.0
// An input file comes from Assemble_flwv2.C (ASMV=2) since 11 April 2017
//
// RUN=#### MIX=0 root AsmFlw_getMixing.C or AsmFlw_getMixing.C\(100\)
//         Real create a multiplicity distribution  
// version 2.0
// parameter ASMV is involved in VER#.#.#
// 
//
//----------------------------------------------------------------------
void AsmFlw_getMixing(Int_t nmax = -1)
{
  sRun = gSystem -> Getenv("RUN");
  sVer = gSystem -> Getenv("VER");
  TString sMix = gSystem -> Getenv("MIX");
  TString sRot = gSystem -> Getenv("ROT");


  if(sRun =="" || sVer == "" ||sMix == "" || sRot == "" || !DefineVersion()) {
    cout << " Please type " << endl;
    cout << "$ RUN=#### VER=#.#.# MIX=0(real) or 1(mix) ROT=0(not rotate) or 1(rotate)  root AsmFlw_getMixing(Number of event) " << endl;
    exit(0);
  }


  cout << "RUN = " << sRun << " Assemble v" << sAsm 
       << " with ver " << sVer  << " mix = " << sMix 
       << " rotation " << sRot
       << endl;



  // Set default to 2.

  if( sRot == "0" )
    BeamAngle = kFALSE;
  else
    BeamAngle = kTRUE;

  if( iVer[0] == 1) {
    BeamAngle = kFALSE;
  }

  cout << " BeamAngle : " << BeamAngle << endl;

  if (sMix == "1") bMix = kTRUE;
  else bMix = kFALSE;

  iRun = atoi(sRun);

  Open();
  Int_t mEvt = GetMultiplicityDistribution();
  OutputTree(nmax);

  
  //--------------------start
  //
  TDatime dtime;
  TDatime btime(dtime);

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


    if(ievt%1000 == 0) {
      dtime.Set();
      Int_t ptime = dtime.Get() - btime.Get();
      std::cout << "Process " << ievt << "/"<< nmax << " = " 
		<< ((Double_t)(ievt)/(Double_t)nmax)*100. << " % --->"
		<< dtime.AsString() << " ---- "
	        << (Int_t)ptime/60 << " [min] "
                << std::endl;
    }

    Initialize(dtime.Get());
   
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

      //     cout << " event " << ievt << endl;

      if( bMix ) aPart1 = GetMixedTrack(&mixEvt); // Get a track randomly
      else {
	aPart1 = (STParticle*) aParticleArray -> At(nLoop);
	aX = ProjA/1000.;
	bY = ProjB/1000.;
      }

      TVector3 mom3  = aPart1->GetMomentum();
      TVector2 momPt = TVector2(aPart1->GetRotatedMomentum().X(), aPart1->GetRotatedMomentum().Y());

    // Rotate along beam angle
      if( BeamAngle ){
	aPart1->RotateAlongBeamDirection(aX, bY);
	mom3  = aPart1->GetRotatedMomentum();
	momPt = aPart1->GetRotatedPt();
      }

      if(aPart1 && aPart1->GetBestTrackFlag() && aPart1->GetLinearPID()>1000) {
	 //       	 && vpt < 500){
       
	numGoodTrack++;
	if( bMix ) event.push_back(mixEvt);
	else event.push_back(ievt);


	Double_t psudr = -log( tan((mom3.Theta()/2))) ;
	Double_t rpd   = aPart1->GetRapidity();


	Int_t gpid = GetPID(aPart1->GetP(),aPart1->GetTotaldEdx()); // for temporal proton pid
	if(gpid != 0){
	  aPart1->SetPID(gpid);
	  pid.push_back(gpid);
	  rpd = aPart1->GetRapidity();

	  if( rpd < 0.4 ) 
	    wgt.push_back(-1);
	  else
	    wgt.push_back(1);
	}

	else{
	  pid.push_back(aPart1->GetLinearPID());
	  if( psudr < 1.4 )
	    wgt.push_back(-1);
	  else
	    wgt.push_back(1);
	  
	}

	trackID.push_back(numGoodTrack);
	vPart.push_back(mom3);
	iphi.push_back( mom3.Phi());
	px.push_back(   mom3.X());
	py.push_back(   mom3.Y());
	pz.push_back(   mom3.Z());
	pt.push_back(   momPt);

	mom.push_back(   aPart1->GetP());
	dedx.push_back(  aPart1->GetTotaldEdx());
	etot.push_back(  aPart1->GetEtotal());
	rapid.push_back( rpd );
	prapid.push_back(psudr);


	// Original momentum
	if( BeamAngle ) {
	  TClonesArray &bp_org = *p_org;
	  new(bp_org[numGoodTrack-1]) TVector3(aPart1->GetMomentum());

	  theta_xz.push_back(aX);
	  theta_yz.push_back(bY);
	}
	
	unitP += mom3.Unit();
    	
	if( mom3.Z() < 400){
	  unitP_t += momPt.Unit();
	  mtrack_t++;	    
	}
	else {
	  unitP_b += momPt.Unit();
	  mtrack_b++;
	}

	//	if(mom3.Theta()<0.4) {
	  //	  unitP_lang += mom3.Unit();
	//}

	if(numGoodTrack != wgt.size()) cout << "Error in number of tracks" << endl;
	unitP_lang += wgt.at(numGoodTrack-1)*momPt.Unit();  // v2.0.9
	
      }
      nLoop++;
      

    }


    // Rapidity distribution //

    mtrack = vPart.size();

    //    cout << "m " << mtrack << " / " << mtrack/2 << endl;
    //subevent analysis
    Int_t itra = 0;
    while( itra < mtrack ){
      Int_t isel = rnd.Integer(2);

      if( mtrack_1 > mtrack/2 && mtrack_2 > mtrack/2 ) break; 

      if( isel == 0 && mtrack_1 < mtrack/2 ) {
	unitP_1 += wgt.at(itra)*pt.at(itra).Unit();
	mtrack_1++;
	//	cout << " _1 " << itra << endl;
	itra++;
      }
      else if( mtrack_2 <= mtrack/2 ) {
	unitP_2 += wgt.at(itra)*pt.at(itra).Unit();
	mtrack_2++;
	//	cout << " _2 " << itra << endl;
	itra++;
      }
      
    }
    


    for(Int_t i = 0; i < mtrack; i++){

      TVector2 mExcRP(0.,0.);
      TVector2 mExcRP_b(0.,0.);
      TVector2 mExcRP_t(0.,0.);
      TVector2 mExcRP_full(0.,0.);

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

	  mExcRP_full += (Double_t)wgt.at(k) * pt.at(k).Unit();

	}
      }
      
      rpphi.push_back( TVector2::Phi_mpi_pi( mExcRP_full.Phi() ));
		       
		       
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
  LoadPIDFile();

  fChain = new TChain("flw");
  TString fn;
  // if( iAsm == 1)
  //   fn = Form("../data/run%d_flw_v"+sVer(0,3)+".root",iRun);
  // else if( iAsm == 2)
  //   fn = Form("../data/run%d_flw_%dv"+sVer(0,1)+".root",iRun,iAsm);
  // else
    fn = Form("../data/run%d_flw_v"+sVer(0,3)+".root",iRun);


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

  if( iVer[0] > 1) {
    fChain->SetBranchAddress("aoq",&aoq);
    fChain->SetBranchAddress("z",&z);
    fChain->SetBranchAddress("ProjA",&ProjA);
    fChain->SetBranchAddress("ProjB",&ProjB);
  }

  if( BeamAngle )
    p_org  = new TClonesArray("TVector3",150);
  


}

void Initialize(Int_t val)
{
  rnd.SetSeed(val);
  trackID.clear();

  unitP   = TVector3(0,0,0);
  unitP_b = TVector2(0,0);
  unitP_t = TVector2(0,0);
  unitP_lang  = TVector2(0,0);
  unitP_1 = TVector2(0.,0.);
  unitP_2 = TVector2(0.,0.);

  mtrack = 0;
  mtrack_t = 0;
  mtrack_b = 0;
  mtrack_1 = 0;
  mtrack_2 = 0;
	
  
  numGoodTrack = 0;
  for(Int_t i = 0; i< 7; i++) ntrack[i];
  
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
  rpphi.clear();
  iphi.clear();
  rapid.clear();
  prapid.clear();
  rpxt.clear();
  rpxb.clear();
  etot.clear();
  wgt.clear();

  if( BeamAngle ){
    p_org->Clear();

    theta_xz.clear();
    theta_yz.clear();
  }

  aParticleArray->Clear();
  mixParticleArray->Clear();
}

Int_t GetMultiplicityDistribution()
{
  // Multplicity histgram
  TString mHistFile = Form("MultRoot/run%d.ngt_v"+sVer(0,3)+".root",iRun);
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

Int_t GetPID(Double_t valx, Double_t valy)
{
  if(gProton){
    if(gProton->IsInside(valx,valy))
      return 12212;
  }
  return 0;
}

void LoadPIDFile()
{
  auto gcutFile = new TFile("gcutProton.root");
  gProton=(TCutG*)gcutFile->Get("gProton");
  gcutFile->Close();
}



void OutputTree(Int_t nmax)
{

  // ROOT out
  TString sdeb = ".s";
  if(nmax < 0)  sdeb = "";
  TString foutname = "../data/run"+sRun+"_";

  if( bMix ) {
    foutname += "mxflw";
    foutname += "_v"+sVer+".root";
  }
  else {
    foutname += "rdflw";
    foutname += "_v"+sVer+sdeb+".root";
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
  if( iVer[0] > 1) {
    mflw->Branch("aoq",&aoq,"aoq/D");
    mflw->Branch("z",&z,"z/D");
    // mflw->Branch("aX",&aX,"aX/D");
    // mflw->Branch("bY",&bY,"bY/D");
  }

  mflw->Branch("numGoodTrack",&numGoodTrack);
  mflw->Branch("mtrack",&mtrack,"mtrack/I");
  mflw->Branch("mtrack_t",&mtrack_t,"mtrack_t/I");
  mflw->Branch("mtrack_b",&mtrack_b,"mtrack_b/I");
  mflw->Branch("mtrack_1",&mtrack_1,"mtrack_1/I");
  mflw->Branch("mtrack_2",&mtrack_2,"mtrack_1/I");
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
  mflw->Branch("rpphi",&rpphi);
  mflw->Branch("wgt",&wgt);
  mflw->Branch("iphi",&iphi);
  mflw->Branch("rapid",&rapid);
  mflw->Branch("prapid",&prapid);
  mflw->Branch("rpxt",&rpxt);
  mflw->Branch("rpxb",&rpxb);
  mflw->Branch("etot",&etot);
  mflw->Branch("unitP",&unitP);
  mflw->Branch("unitP_b",&unitP_b);
  mflw->Branch("unitP_t",&unitP_t);
  mflw->Branch("unitP_1",&unitP_1);
  mflw->Branch("unitP_2",&unitP_2);
  mflw->Branch("unitP_lang",&unitP_lang);
  mflw->Branch("kymult",&kymult,"kymult/I");

  if( BeamAngle ) {
    mflw->Branch("p_org" ,&p_org);
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

    if( BeamAngle ) {
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


Bool_t DefineVersion()
{
  Bool_t bfound = kFALSE;

  TString ver = sVer + ".";
  
  for ( Int_t i = 0; i < 3; i++) {
    if( ver.First(".") < 0 ) break;

    Ssiz_t end = ver.First(".")  ;
    TString ver1 = ver(0, end);

    ver = ver(end+1, ver.Length());

    iVer[i] = atoi(ver1);


    if(i==2) bfound = kTRUE;

  }
  
  if(!bfound)
    cout << " missing version number : " << iVer[0] << "." << iVer[1] << "." << iVer[2] << endl;

  return bfound;

}
