#include "AsmFlw_getFlatten2Dcrr.h"
//----------------------------------------------------------------------
//   AsmFlw_getFlatten.C  succeeded from AsmFlw_getMixing.C
// (c) Mizuki Kurata-Nishimura 
//----------------------------------------

//
//----------------------------------------------------------------------
void AsmFlw_getFlatten2Dcrr(Long64_t nmax = -1)
{
  SetEnvironment();

  // Set number of bins divinding rapidity.
  if(sbRun != "0")
    nBin = SetDatabaseFiles();


  // Print Run configuration 
  cout << " ---------- CONFIGURATION ----------  " << endl;
  cout << "RUN = "      << sRun << " Assemble v" << sAsm 
       << " with ver "  << sVer  << " mix = " << sMix 
       << " rotation "  << sRot
       << " Flatten ; " << nBin
       << endl;


  // I/O
  Open();

  OutputTree(nmax);

  //--------------------start
  //

  SetNumberOfProcess(nmax);


  for(Long64_t ievt = 0; ievt < maxProc; ievt++){

    PrintProcess(ievt);

    Initialize();
   

    Long64_t nGoodTrack = 0;
    if(bMix)
      nGoodTrack = (Long64_t)GetRandomNumTrack(); // get number of track 
    else {
      fTree->GetEntry(ievt);
      nGoodTrack = aParticleArray->GetEntries();
    }

    if(ntrack[3] > 0 || bMix) {
     
      Long64_t nLoop = 0;

      while(kTRUE) {

	if(  bMix && numGoodTrack > nGoodTrack - 1 ) break;  // mGoodTrack-1
	if( !bMix && nLoop >=  nGoodTrack) break;


	STParticle *aPart1 = NULL;
	Long64_t    mixEvt = (Long64_t)ievt;
	
	Int_t ntrk = nGoodTrack;

	if( bMix ) 
	  aPart1 = GetMixedTrack(&mixEvt, &ntrk); // Get a track randomly
	else 
	  aPart1 = GetRealTrack(nLoop);

	aPart1->SetReactionPlaneFlag(0);

	// Good track and fragments are selected
	if(aPart1!=NULL && aPart1->GetBestTrackFlag() && aPart1->GetLinearPID()>1000) {
       
	  numGoodTrack++;


	  aPart1->SetReactionPlaneFlag(1);

	  TVector3 mom3  = aPart1->GetMomentum();
	  TVector2 momPt = TVector2( aPart1->GetMomentum().X(), aPart1->GetMomentum().Y() );

	  // Rotate along beam angle
	  if( BeamAngle )
	    RotateAsBeamAngle(aPart1, &mom3, &momPt);

	  if( bMix ) {
	    aPart1->SetMixedEventID(mixEvt);
	    event.push_back(mixEvt);
	  }
	  else 
	    event.push_back(ievt);

	  
	  SetPtWeight(aPart1);
	  aPart1->SetMixedNtrack(ntrk);

	  trackID.push_back(numGoodTrack);
	  

	}

	//	cout << "nLoop " << nLoop << " / " << nGoodTrack << " - " << numGoodTrack << " : " << aPart1->GetReactionPlaneFlag()<< endl;
	nLoop++;
      }
    }

    mtrack = trackID.size();

    //    cout << " mtrack " << mtrack << endl;

    if(mtrack > 0) {
      mflw->Fill();
      hgtc->Fill(mtrack);
    }
  }


  if(!bMix && mhfile != NULL) {
    mhfile->cd();
    hgtc->Write();
  }

  CheckPlot(126);

  fout->cd();
  fout->Write();
  std::cout << fout->GetName() << std::endl;

  if(gROOT->IsBatch()) {
    fout->Close();
    exit(0);
  }

}

void SetEnvironment()
{

  sRun = gSystem -> Getenv("RUN");  // RUN number
  sVer = gSystem -> Getenv("VER");  // Version ID
  sMix = gSystem -> Getenv("MIX");
  sRot = gSystem -> Getenv("ROT");

  


  if(sRun =="" || sVer == "" ||sMix == "" ||!DefineVersion()) {
    cout << " Please type " << endl;
    cout << "$ RUN=#### VER=#.#.# MIX=0(real) or 1(mix)  root AsmFlw_getFlatten2Dcrr.C(Number of event) " << endl;
    exit(0);
  }

  cout << "sBinp = " << sBinp << " Flatten RUN = " << sbRun << endl;

  // Beam axis rotaion according to beam direction. It is fixed to 1.
  //  sRot = "1";
  if( sRot == "0" )
    BeamAngle = kFALSE;
  else
    BeamAngle = kTRUE;

  if( iVer[0] == 1) {
    BeamAngle = kFALSE;
  }

  // Real or mixed event 
  if (sMix == "1") bMix = kTRUE;
  else bMix = kFALSE;


  // Set RUN number
  iRun = atoi(sRun);

  if( sbRun == "0" )
    nBin = 0;

}

void PrintProcess(Int_t ievt)
{
  TDatime dtime;
  static TDatime btime(dtime);

  UInt_t eprint = 1000;
  if(bMix) eprint = 1000;

  if(ievt%eprint == 0) {
    dtime.Set();
    Int_t ptime = dtime.Get() - btime.Get();
    std::cout << "Process " << setw(8) << ievt << "/"<< maxProc << " = " 
	      << ((Double_t)(ievt)/(Double_t)maxProc)*100. << " % --->"
	      << dtime.AsString() << " ---- "
	      << setw(5) << (Int_t)ptime/60 << " [min] "
	      << std::endl;
  }
}

void SetNumberOfProcess(Int_t nmax)
{
  Long64_t mEvt = GetMultiplicityDistribution();
  nEntry = fTree->GetEntries();

  if( bMix ) {
    if( nmax == -1 )  maxProc = mEvt;
    else if( nmax == -2 ) maxProc = mEvt*10;
  }
  else
    if( nmax == -1 || nmax > nEntry )  maxProc = nEntry;
  

  std::cout << " Number of Events -->" << nEntry   << std::endl;
  std::cout << " Maximum Events   -->" << maxProc  << std::endl;

}

void Open()
{
  LoadPIDFile();

  TString fn = Form("run%d_flw_v"+sVer(0,3)+".root",iRun);
  if( !gSystem->FindFile("../data/", fn) ) {
    
    Int_t iver = atoi((TString)sVer(2,1));

    while( iver > -1 ){
      TString ss = Form(".%d",iver);
      fn = Form("run%d_flw_v"+sVer(0,1)+ss+".root",iRun);

      if( !gSystem->FindFile("../data/", fn) ) 
	iver--;
      else 
	break;
    }

  }
  
  auto fFile = new TFile(fn);
  fTree = (TTree*)fFile->Get("flw");

  if(!fTree) {
    cout << " No data was found " << fn << endl;
    exit(0);
  }
  cout << "Input file is " << fn << endl;


  aParticleArray   = new TClonesArray("STParticle",150);

  fTree->SetBranchAddress("STParticle",&aParticleArray);
  fTree->SetBranchAddress("ntrack",ntrack);
  fTree->SetBranchAddress("kymult",&kymult);

  if( iVer[0] > 1) {
    fTree->SetBranchAddress("aoq",&aoq);
    fTree->SetBranchAddress("z",&z);
    fTree->SetBranchAddress("ProjA",&ProjA);
    fTree->SetBranchAddress("ProjB",&ProjB);
  }

  p_rot = new TClonesArray("TVector3",150);

  if( BeamAngle )
    p_org  = new TClonesArray("TVector3",150);
  


}

void Initialize()
{

  trackID.clear();

  mtrack = 0;
  mxntrk = 0;  
  numGoodTrack = 0;
  for(Int_t i = 0; i< 7; i++) ntrack[i];
  
  event.clear();

  aParticleArray->Clear();

  TDatime dtime;
  rnd.SetSeed(dtime.Get());
}

Long64_t GetMultiplicityDistribution()
{
  // Multplicity histgram
  TString mHistFile = Form("MultRoot/run%d.ngt_v"+sVer(0,3)+".root",iRun);
  TString hf = mHistFile;

  Long64_t nEvt = 0;

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
    nEvt = (Long64_t)histGT_r -> GetEntries();
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



void OutputTree(Long64_t nmax)
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

  mflw->Branch("STParticle",&aParticleArray);

  mflw->Branch("ntrack",ntrack,"ntrack[7]/I");
  mflw->Branch("mtrack",&mtrack,"mtrack/I");
  mflw->Branch("event",&event);
  mflw->Branch("mxntrk",&mxntrk);

  hgtc         = new TH1I("hgtc","number of good fragment",100,0,100);
}

Long64_t GetRandomNumTrack()
{

  TDatime dtime;

  Int_t itime = dtime.Get();

  // gRandom = new TRandom3(0);
  // gRandom->SetSeed(itime);

  if(!histGT_r){
    cout << " no histgram was found for randowm number " << endl;
    return 0;
  }

  Long64_t bcont = (Long64_t)histGT_r->GetRandom();


  return bcont;
}

STParticle *GetRealTrack(Long64_t ival)
{
  STParticle *realPart = NULL;
  realPart = (STParticle*)aParticleArray -> At(ival);
  if(realPart){
    aX = ProjA/1000.;
    bY = ProjB/1000.;
  }

  return realPart;
}


STParticle *GetMixedTrack(Long64_t *ival, Int_t *kval)
{
  STParticle *mixPart = NULL;
  static Long64_t mevt = *ival;

  // ---------- shfiting -- search similar multiplcity events
  while(kTRUE){	
    mevt += 2;
    if(mevt > nEntry) 	mevt = 0;
           
    UInt_t kLoop = 0;
    while( kLoop < 5 ){
      fTree->GetEntry(mevt);

      if( std::abs(ntrack[3] - *kval) < ntr_diff ) break;
      //      cout << "ntrack[3] " << ntrack[3] << " kval " << *kval  << endl;
      mevt++;
      if(mevt > nEntry) {
	mevt = 0;
	kLoop++;
	if(kLoop > 1) cout << " Too much loops " << kLoop << " kval " << *kval << " mevt " << mevt << " / " << nEntry <<  endl;
      }
    }
    //        cout << " get mixed event " << mevt << " / " << nEntry << endl;

    *kval = ntrack[3];
    if( BeamAngle ) {
      aX = ProjA/1000.;
      bY = ProjB/1000.;
    }
	  

    Int_t nmixTrack = aParticleArray->GetEntries();

    Int_t imixTrack = (Int_t)pran.Uniform(0,nmixTrack);

    if( imixTrack < nmixTrack ) {
      mixPart = (STParticle*) aParticleArray->At(imixTrack);	        
      break;
    }

  }

  *ival = mevt; 
  return mixPart;
}


void RotateAsBeamAngle(STParticle *apart, TVector3 *p1, TVector2 *pt)
{
  apart->RotateAlongBeamDirection(aX, bY);
  *p1  = apart->GetRotatedMomentum();
  *pt  = apart->GetRotatedPt();
}

void SetPtWeight(STParticle *apart)
{

  Int_t    gpid  = GetPID(apart->GetP(),apart->GetTotaldEdx()); // for temporal proton pid

  Double_t psudr = apart->GetpsudoRapidity();
  Double_t rpd   = apart->GetRapidity();


  if(gpid != 0){
    apart->SetPID(gpid);
    rpd = apart->GetRapidity();

    if( rpd < 0.4 ) 
      apart->SetRPWeight(-1);
    else
      apart->SetRPWeight(1);
  }
  else{
    if( psudr < 1.4 )
      apart->SetRPWeight(-1);
    else
      apart->SetRPWeight(1);
  }
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

UInt_t SetDatabaseFiles()
{

  //  vector<TString> vfname;
  if(sbRun != "0") {
    TString fname = "cfrun"+sbRun;
    if(bMix)
      //  fname += sbRun+"_mxflw_v2.0.14.";
      fname += "_mxflw_v";
    else
      // fname += sbRun+"_rdflw_v2.0.14.";
      fname += "_rdflw_v";
    fname += sbVer + ".";

    //    cout << " fname " << fname << "+" << sBinp << endl;

    UInt_t ihmsum = 0;
    UInt_t imtk = 0;
    while(1){

      UInt_t ihm = 0;
      while(1){

	TString ffname = fname +  Form("m%dn%d"+sBinp+".txt",imtk,ihm);
	//	cout << ffname << endl;

	
	if( gSystem->FindFile("db",ffname) ){
	  vfname.push_back(ffname);
	  ihm++;
	}
	else 
	  break;
      }
      if(ihm > 0 ) {
	ihmsum += ihm;
	cout << " ihm " << ihm  << " <- " << imtk << " sum " << ihmsum << endl;
      }
      else if(ihm == 0)
	break;

      imtk++;
    }
  }

  
  nBin = (UInt_t)vfname.size();
  if(nBin == 0) return nBin;
  
  aflowcorrArray = new TClonesArray("STFlowCorrection",300);  
  TClonesArray &arr = *aflowcorrArray;

  cout << " nBin " << nBin << endl;

  vector<TString>::iterator itb;
  UInt_t ihm = 0;
 
  for(itb = vfname.begin(); itb != vfname.end(); itb++, ihm++){

    new(arr[ihm]) STFlowCorrection();

    flowcorr = (STFlowCorrection*)arr.At(ihm);
    flowcorr->SetRealOrMix(1);
    flowcorr->SetRealOrMix((UInt_t)bMix);
    flowcorr->SetFileName(vfname.at(ihm));
    flowcorr->GetCorrectionFactor();
    
    cout << " $$$$$$$$$$$ ---->  nbin " << ihm  << " " << flowcorr->GetBin_min(0) << " : "<< flowcorr->GetBin_min(1) << endl;    
  }

  binpara   = flowcorr->GetBinParameter(1);

  

  return nBin;    
}


void CheckPlot(UInt_t ival)
{
  if(aflowcorrArray == NULL) return;

  auto cplot = new TCanvas();
  cplot->Divide(2,2);

  std::cout << "Checking histgram " << ival << std::endl;
  flowcorr = (STFlowCorrection*)aflowcorrArray->At(ival);

  flowcorr->PrintRange();

  hvphi  = new TH1D("hvphi"  ,"phi"   ,100,-3.2,3.2);
  hvthet = new TH1D("hvtheta","theta" ,100,0.,1.4);
  hvmtk  = new TH1I("hvmtk"  ,"mtrack", 60,0,60);

  vector<Double_t>::iterator itr;
  vector<Int_t>::iterator   iitr;

  vector<Double_t> vec1 =  flowcorr->GetOriginalPhi();
  for(itr=vec1.begin(); itr!=vec1.end(); itr++)
    hvphi->Fill(*itr);
  vec1.clear();

  cplot->cd(1);
  hvphi->Draw();
  

  vec1 =  flowcorr->GetTheta();
  for(itr=vec1.begin(); itr!=vec1.end(); itr++)
    hvthet->Fill(*itr);

  cplot->cd(2);
  hvthet->Draw();

  vector<Int_t> vec2 =  flowcorr->GetMTrack();
  for(iitr = vec2.begin(); iitr != vec2.end(); iitr++)
    hvmtk->Fill(*iitr);

  cplot->cd(3);
  hvmtk->Draw();

}

