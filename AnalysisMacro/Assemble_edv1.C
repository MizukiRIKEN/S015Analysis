#include "KatanaRoot/KatanaRoot_Load.h"
#include "KyotoRoot/STTriggerArray.hh"

Int_t irun=2903;
//Int_t irun=3023;
//Int_t irun=3023;

//Environment
Bool_t KATANA   = kTRUE;
Bool_t BigRIPS  = kTRUE;
Bool_t KyotoArry= kTRUE;

//Bool_t debug = kFALSE;
Bool_t debug = kTRUE;

//Sn Beam offset
Double_t BDCX_off = 0.; // -20.77;  // Sn108 from run 2189


//setParameters
Double_t tx_right= 10.;
Double_t tx_left = 28.;
Double_t ty_top  = 10.;
Double_t ty_btm  = -10.;

// KATANA offset
Double_t KATANA_frame_OffSet  = -977.;
Double_t KATANA_paddle_OffSet = 35.;
Double_t KATANA_paddle_Width  = 100.;
Double_t KATANA_paddle_Height = 380.; 
//Double_t KATANA_paddle_Right[12] = {1256,1156,1057,956,855,635,535,435,335,235,135,35};
Double_t KATANA_paddle_Right[12] = {35,135,235,335,435,535,635,855,956,1057,1156,1256};


//set run2334
Double_t beamVx_offset = -17.54;
Double_t beamVx_sigma  =  1.53;
Double_t beamVy_offset = -2.28e+02;
Double_t beamVy_sigma  =  1.08;
Double_t beamVx_nsig   = 4;
Double_t beamVy_nsig   = 4;

Double_t trackVx_offset = -19.5;
Double_t trackVx_sigma  =  3.292;
Double_t trackVy_offset = -2.29e+02;
Double_t trackVy_sigma  =  2.7;
Double_t trackVx_nsig   = 6;
Double_t trackVy_nsig   = 4;

Double_t beamVx_min = -80.;
Double_t beamVx_max =  40.;

//v1.01
//Double_t beamVx_offset = -16.95;
//Double_t beamVx_sigma  = 2.711;
//Double_t beamVy_offset = -2.26e+02;
//Double_t beamVy_sigma  = 1.74;

Double_t trktgt_right = -12;
Double_t trktgt_left  = 12;
Double_t trktgt_top   = -210;
Double_t trktgt_btm   = -235 ;

TCutG *g132Sn;
TCutG *g108Sn;


TString SetOutputFileName(TString &sRun)
{
  TString sdeb = "";
  if(debug)  sdeb = ".s";

  TString foutname = "../data/run"+sRun+"_asm3.v1.03"+sdeb+".root";
  
  return foutname;
}

void BeamPID(Int_t ibeam)
{
  if(ibeam == 132){
    auto gcutFile = new TFile("gcut132Sn.root");
    g132Sn=(TCutG*)gcutFile->Get("g132Sn");
    gcutFile->Close();
  }
  else if(ibeam == 108){
    auto gcutFile = new TFile("gcut108Sn.root");
    g108Sn=(TCutG*)gcutFile->Get("g108Sn");
    gcutFile->Close();
  }
}


void setDataDirectory(TString &rootdir)
{
  if(irun == 3023)
      rootdir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/recoData";
  else if(irun >= 2480 && irun <= 3211) {  
    rootdir = "/data/spdaq01/recoData/v1.03+20160411.2016runs/rootfiles";
  }
  else {
    rootdir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/recoData";
  }
}

void setKATANADirectory(TString &ktnrootdir)
{
  ktnrootdir = "/data/spdaq01/katana/root/katana/";
}

void setKYOTODirectory(TString &kytrootdir)
{
  //kytrootdir = "/data/spdaq01/ridf/root/kyotoroot/";
  //kytrootdir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/kyotoData/";
  kytrootdir = "/cache/scr/spirit/kaneko/kyotoroot/";
}

void Assemble_edv1()
{
  //2334//2480//2189//2903//2477
  //////////////////////////////////////////////////////////
  //   This file has been automatically generated 
  //     (Sat Apr  9 22:15:44 2016 by ROOT version6.06/02)
  //   from TTree beam/bigrips
  //   found on file: ../beam1807.root
  //////////////////////////////////////////////////////////


  //Reset ROOT and connect tree file
  gROOT->Reset();

  TString rootDir;
  TString sRun = Form("%d",irun);

  //----- Beam PID -----------------------

  Int_t ibeam = 0;
  if(irun > 2174 && irun < 2509)
    ibeam = 108;
  else if( irun > 2805 && irun < 3039)
    ibeam = 132;
      
  BeamPID(ibeam);


  //----- BigRIPS data --------------------
  auto ribfChain = new TChain("beam");
    
  ribfChain ->Add("../bripsData/run"+sRun+".ridf.root");

  //Declaration of leaves types
  Float_t         aoq;
  Float_t         z;
  Float_t         tof;
  Float_t         beta;
  Float_t         tx;
  Float_t         ty;
  Float_t         ta;
  Float_t         tb;

  //----- Set branch addresses.
  ribfChain->SetBranchAddress("aoq",&aoq);
  ribfChain->SetBranchAddress("z",&z);
  ribfChain->SetBranchAddress("tof",&tof);
  ribfChain->SetBranchAddress("beta",&beta);
  ribfChain->SetBranchAddress("tx",&tx);
  ribfChain->SetBranchAddress("ty",&ty);
  ribfChain->SetBranchAddress("ta",&ta);
  ribfChain->SetBranchAddress("tb",&tb);

  //----- TPC data --------------------
  auto fChain = new TChain("cbmsim");

  setDataDirectory(rootDir);
   
  Int_t i = 0;
  while(kTRUE){
     
    //    TString recoFile = Form("run"+sRun+"_s%d.reco.v1.03_111.root",i);
    TString recoFile = Form("run"+sRun+"_s%d.reco.v1.03.root",i);
    //    TString recoFile = Form("run"+sRun+"_s%d.reco.root",i);
    //    cout << " recoFile " << rootDir+recoFile << endl;
    if(gSystem->FindFile(rootDir,recoFile)){
      //      cout << " Found file " << recoFile << endl;
      fChain -> Add(recoFile);
    }
    else
      break;
    i++;
  }

  TClonesArray *trackArray = NULL;
  fChain -> SetBranchAddress("STTrack", &trackArray);

  TClonesArray *vertexArray = NULL;
  fChain -> SetBranchAddress("STVertex",&vertexArray);
  

  //----- KATANA data --------------------
  gSystem->Load("KatanaRoot/KatanaRoot_Load_cpp.so");
  auto kChain = new TChain("tree");

  ULong_t event_number;
  Float_t max_veto;
  KatanaRoot *katanaroot = NULL;
  TriggerBox *triggerbox = NULL;

  Int_t  kAC = -99;

  if(KATANA) {
    TString ktnrootdir;
    setKATANADirectory(ktnrootdir);

    kChain->Add(ktnrootdir+"run"+sRun+".root");

    kChain -> SetBranchAddress("Katana",&katanaroot);
    kChain -> SetBranchAddress("TriggerBox",&triggerbox);

  }


  //----- Kyoto Array --------------------
  auto kaChain = new TChain("kyotoM");
  Int_t  kynHit;
  vector<int>   *kyhitch  = 0;
  vector<float> *kyhitx = 0;
  vector<float> *kyhitz = 0;

  TBranch *bkyhitch = 0;
  TBranch *bkyhitx = 0;
  TBranch *bkyhitz = 0;

  if(KyotoArry){
    TString kytrootdir;
    setKYOTODirectory(kytrootdir);
    kaChain-> Add(kytrootdir+"run"+sRun+".kyotopos.root");

    kaChain-> SetBranchAddress("multiplicity",&kynHit);
    kaChain-> SetBranchAddress("hitch",&kyhitch,&bkyhitch);
    kaChain-> SetBranchAddress("hitxpos",&kyhitx,&bkyhitx);
    kaChain-> SetBranchAddress("hitzpos",&kyhitz,&bkyhitz);
  }
    
   
  //-------------------- event number check --------------------
  Int_t nEvtTPC = 0;
  if(fChain) nEvtTPC = fChain -> GetEntries();
  cout << "Number of events in TPC: " << nEvtTPC << endl;
  if(nEvtTPC == 0) return;

  Long64_t nEvents = 0;
  if(ribfChain) nEvents = ribfChain->GetEntries();
  cout << "Number of events in RIDF: " << nEvents << endl;

  Long64_t nEvtKTN = 0;
  if(KATANA) nEvtKTN = kChain -> GetEntries();
  cout << "Number of events in KATANA: " << nEvtKTN << endl;


  Long64_t nEvtKyt = 0;
  if(KyotoArry) nEvtKyt = kaChain -> GetEntries();
  cout << "Number of events in KyotoArray: " << nEvtKyt << endl;

  Int_t nEntry = nEvtTPC;
  if(nEvtTPC > nEvents)
    nEntry = nEvents;

  if(nEvtKTN != 0 && nEntry > nEvtKTN)
    nEntry = nEvtKTN;
  
  if(nEvtKyt != 0 && nEntry > nEvtKyt)
    nEntry = nEvtKyt;

  //--------------------output root--------------------
  TString foutname = SetOutputFileName(sRun);
  TFile *fout = new TFile(foutname,"recreate");
  //  TTree *tpc = new TTree("tpc","Beam and TPC track");
  if(tpc)
    cout << "Output file is " << foutname << endl;

  Int_t evtid;
  Int_t ntrack[7];
  Int_t bevt[7];
  Int_t bmpid;
  Int_t kyotomL;
  Int_t kyotomR;
  Int_t KatanaMult;
  Int_t trknKATANA;
  Int_t foundKATANA;

  std::vector<Double_t> phi;
  std::vector<Int_t> gTrack;
  std::vector<Int_t>gTgTrack;
  std::vector<Int_t>KytTrack;
  std::vector<Double_t>hitKyotoLZ;
  std::vector<Double_t>hitKyotoLX;
  std::vector<Double_t>dzKyotoL;
  std::vector<Double_t>hitKyotoRZ;
  std::vector<Double_t>hitKyotoRX;
  std::vector<Double_t>dzKyotoR;
  std::vector<Int_t>   hitKATANAID;
  std::vector<Double_t>hitKATANAX;
  std::vector<Double_t>hitKATANAY;
  std::vector<Double_t>dxhitKATANA;
  std::vector<Double_t>bitpattern;
  std::vector<Double_t>KatanaPadX;
  std::vector<Double_t>KatanaPadID;
  std::vector<UShort_t>fndKATANA;

  //-- output
  // tpc->Branch("irun",&irun,"irun/I");
  // tpc->Branch("evtid",&evtid,"evtid/I");
  // tpc->Branch("bevt",bevt,"bevt[7]/I");
  // tpc->Branch("ntrack",ntrack,"ntrack[7]/I");
  // tpc->Branch("bmpid",&bmpid,"bmpid/I");
  // tpc->Branch("gTrack",&gTrack);
  // tpc->Branch("gTgTrack",&gTgTrack);
  // tpc->Branch("KytTrack",&KytTrack);
  // tpc->Branch("kyotomL",&kyotomL);
  // tpc->Branch("kyotomR",&kyotomR);
  // tpc->Branch("hitKyotoRZ",&hitKyotoRZ);
  // tpc->Branch("hitKyotoRX",&hitKyotoRX);
  // tpc->Branch("dzKyotoL",&dzKyotoL);
  // tpc->Branch("dzKyotoR",&dzKyotoR);
  // tpc->Branch("STTrack",&trackArray);
  // tpc->Branch("STVertex",&vertexArray);
  // tpc->Branch("aoq",&aoq,"aoq/F");
  // tpc->Branch("z",&z,"z/F");
  // tpc->Branch("tof",&tof,"tof/F");
  // tpc->Branch("beta",&beta,"beta/F");
  // tpc->Branch("tx",&tx,"tx/F");
  // tpc->Branch("ty",&ty,"ty/F");
  // tpc->Branch("ta",&ta,"ta/F");
  // tpc->Branch("tb",&tb,"tb/F");
  // tpc->Branch("max_veto",&max_veto,"max_veto/F");
  // tpc->Branch("bitpattern",&bitpattern);
  // tpc->Branch("KatanaPadX",&KatanaPadX);
  // tpc->Branch("KatanaPadID",&KatanaPadID);
  // tpc->Branch("hitKATANAID",&hitKATANAID);
  // tpc->Branch("hitKATANAX",&hitKATANAX);
  // tpc->Branch("hitKATANAY",&hitKATANAY);
  // tpc->Branch("dxhitKATANA",&dxhitKATANA);
  // tpc->Branch("fndKATANA",&fndKATANA);
  // tpc->Branch("KatanaMult",&KatanaMult,"KatanaMult/I");
  // tpc->Branch("trknKATANA",&trknKATANA,"trknKATANA/I");
  // tpc->Branch("foundKATANA",&foundKATANA,"foundKATANA/I");

  // Histgram



  if(debug)  nEntry = 2000;
  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  //-------------------- event loop --------------------
  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
  for (Int_t i=0; i<nEntry;i++) {
    evtid = i;
     
    for (Int_t m = 0; m < 7; m++) bevt[m] = 0;
    for (Int_t m = 0; m < 7; m++) ntrack[m] = 0;

    phi.clear();
    gTrack.clear();
    gTgTrack.clear();
    KytTrack.clear();

    dzKyotoL.clear();
    hitKyotoLZ.clear();
    hitKyotoLX.clear();
    dzKyotoR.clear();
    hitKyotoRZ.clear();
    hitKyotoRX.clear();
    hitKATANAID.clear();
    hitKATANAX.clear();
    hitKATANAY.clear();
    fndKATANA.clear();
    dxhitKATANA.clear();
    bitpattern.clear();
    KatanaPadX.clear();
    KatanaPadID.clear();

    ngtrack   = 0;
    ngtgtrack = 0;

    kyotomL  = 0;
    kyotomR  = 0;
    KatanaMult = 0;
    trknKATANA = 0;
    foundKATANA = 0;

    if(i%1000 == 0) std::cout << "Process " << i << std::endl;

    // --------------- RIDF --------------
    ribfChain->GetEntry(i);
    // --------------- TPC ---------------
    fChain -> GetEntry(i);
    // --------------- KATANA ------------
    if(KATANA && i < nEvtKTN) {
      kChain -> GetEntry(i);
    }
    // --------------- KytoArray ----------
    if(KyotoArry && i < nEvtKyt){
      kaChain -> GetEntry(i);
      bkyhitx -> GetEntry(i);
      bkyhitz -> GetEntry(i);

      for(Int_t ik = 0; ik < (Int_t)kyhitz->size(); ik++){
	if(kyhitch->at(ik) <= 31) kyotomL++;
	else kyotomR++;
      }
    }

    // ------- test ----------
    if(i < nEvtKTN)    max_veto = katanaroot->max_veto;

    // --- Beam Selection --------------------
    if((tx >= tx_right &&tx <= tx_left) &&  
       (ty >= ty_btm  && ty <= ty_top ))   bevt[0] = 1;

    // --- BDC offset --------------------
    tx += BDCX_off;

    
    bmpid = 0;
    if(g132Sn){
      if(g132Sn->IsInside(aoq,z))
	bmpid = 20;
    }
    else if(g108Sn){
      if(g108Sn->IsInside(aoq,z))
	bmpid = 10;
    }

    // --- Vertex selection --------------------    
    genfit::GFRaveVertex *vertex = (genfit::GFRaveVertex *) vertexArray -> At(0);
    if(vertex) {
       
      Double_t vertexX = vertex->getPos().X();
      Double_t vertexY = vertex->getPos().Y();
      Double_t diffVx  = vertexX - tx;
      Double_t diffVy  = vertexY - ty;

      bevt[1] = 1;

      if((vertex->getPos().Z() <= -6.7 && vertex->getPos().Z() >= -14.6)) bevt[2] = 1;

      if(vertexX >= trktgt_right && vertexX <= trktgt_left &&
	 vertexY >= trktgt_btm && vertexY <= trktgt_top)    bevt[3] = 1;

      if((diffVx < beamVx_offset + beamVy_nsig*beamVx_sigma && diffVx > 
	  beamVx_offset-beamVy_nsig*beamVx_sigma) &&
	 (diffVy < beamVy_offset + beamVy_nsig*beamVy_sigma && diffVy > 
	  beamVy_offset-beamVy_nsig*beamVy_sigma)) 
	bevt[4]=1;
    }


    //-------------------- user analysis --------------------
    Int_t numTracksFromArray = trackArray -> GetEntries();
    ntrack[0] = numTracksFromArray;

    for (Int_t iTrack = 0; iTrack < numTracksFromArray; iTrack++) {

      STTrack *trackFromArray = (STTrack*) trackArray -> At(iTrack);


      //--- check origin of the track ---;
      Double_t trackVx = trackFromArray->GetBeamVx();
      Double_t trackVy = trackFromArray->GetBeamVy();
      Double_t diffVx = trackVx - tx;
      Double_t diffVy = trackVy - ty;
	
      if((diffVx < trackVx_offset+trackVx_nsig*trackVx_sigma && 
	  diffVx > trackVx_offset-trackVx_nsig*trackVx_sigma) &&
	 (diffVy < trackVy_offset+trackVy_nsig*trackVy_sigma && 
	  diffVy > trackVy_offset-trackVy_nsig*trackVy_sigma)){

	// if((diffVx < beamVx_max && diffVx > beamVx_min) &&
	//    (diffVy < beamVy_offset+beamVy_nsig*beamVy_sigma && 
	//     diffVy > beamVy_offset-beamVy_nsig*beamVy_sigma)){

	bevt[5] = 1;
	gTrack.push_back(1);

	ntrack[1]++;
      }
      else{
	gTrack.push_back(0);
	ntrack[3]++;
      }

      if(trackVx >= trktgt_right && trackVx <= trktgt_left &&
	 trackVy >= trktgt_btm   && trackVy <= trktgt_top){
	gTgTrack.push_back(1);

	bevt[6] = 1;
	ntrack[2]++;
      }
      else{
	gTgTrack.push_back(0);
	ntrack[4]++;
      }

      // ---- Correlation with KATANA --------------------
      if(KATANA){

	if(iTrack == 0)  {
	  Int_t    bitsize    = triggerbox->bitpattern.size();
	  copy(triggerbox->bitpattern.begin(),triggerbox->bitpattern.end(), back_inserter(bitpattern));
	  for(Int_t ik = 0; ik < bitsize; ik++){
	    if(bitpattern.at(ik) < 12 && bitpattern.at(ik) >= 0){
	      Double_t KATANAX_c = KATANA_paddle_Right[(Int_t)bitpattern.at(ik)] + KATANA_frame_OffSet;
	      KatanaPadID.push_back((Int_t)bitpattern.at(ik));
	      KatanaPadX.push_back(KATANAX_c);
	      KatanaMult = triggerbox->KatanaM;
	      
	      
	      h1a->Fill(

	    }
	  }
	}


	Double_t trkKATANAx = trackFromArray->GetKatanaHitX(); 
	Double_t trkKATANAy = trackFromArray->GetKatanaHitY(); 

	Int_t    KATANAID = -9;
	Double_t dKATANAX = 9999;	  
	Double_t KATANAX  = 9999;
	Double_t KATANAY  = 9999;

	if(trkKATANAx >= -900 && trkKATANAx <= 350 && trkKATANAy <= 0 && trkKATANAy >= -400){
	  trknKATANA++;

	  for(Int_t ik = 0; ik < KatanaMult; ik++){
	    Double_t distX = trkKATANAx - KatanaPadX.at(ik);
	    
	    // cout << " KATANA X " << KATANAX << " bitpattern " << triggerbox->bitpattern.at(ik) 
	    // 	   << " track X " << trkKATANAx 
	    // 	   << " DIST " << distX 
	    // 	   << " min dist " << dKATANAX << endl;
  
	    if(abs(dKATANAX) > abs(distX)){
	      KATANAID = ik;
	      dKATANAX = distX;
	      KATANAX  = trkKATANAx;
	      KATANAY  = trkKATANAy;	      
	    }
	  }
	}
  auto c2 = new TCanvas("c2","c2",0,0,800,600);
  auto c3 = new TCanvas("c3","c3",0,1000,800,600);
  
  c2->Divide(1,2);

  TString h1name = "Katana hit pattern";
  TString h2name = "Extrapolated hit ";
  auto h1 = new TH1D("h1",h1name,24,-942,379);   
  auto h1a= new TH1D("h1a",h1name,240,-942,379);   
  auto h2 = new TH1D("h2",h2name,150,-942,379);
  auto h2a= new TH1D("h2a",h2name,150,-942,379);
  auto h3 = new TH2D("h3","dx on KATANA ",26,-942,379,100,-200,200);
  auto h4 = new TH2D("h4","track X-Y on KATANA within one paddle",150,-942,379,100,-400,0);
  auto h5 = new TH2D("h5","track X-Y on KATANA",150,-942,379,100,-400,0);

  auto n1 = new TH2D("n1","Number of hits in total;# of track; Katana Mult",25,0,25,20,0,25);
  auto n2 = new TH2D("n2","Number of hits event by event",25,0,25,20,0,25);
	  	
	//	cout << " Filling KATANAX " << KATANAX << endl;
	hitKATANAID.push_back(KATANAID);
	hitKATANAX.push_back(KATANAX);
	hitKATANAY.push_back(KATANAY);
	dxhitKATANA.push_back(dKATANAX);

	if(abs(dKATANAX) <= 100){
	  foundKATANA++;
	  fndKATANA.push_back(1);
	}
	else
	  fndKATANA.push_back(0);

	

      }
    }
  
    //--------------------
      
    // if(ntrack[0] != 0)
  }
  
  
  cout << " Output root file is : " << fout->GetName() << endl;
}
