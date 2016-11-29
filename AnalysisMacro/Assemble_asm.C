#include "KatanaRoot/katana.h"
#include "KyotoRoot/STTriggerArray.hh"

//Environment
Bool_t KATANA   = kTRUE;
Bool_t BigRIPS  = kTRUE;
Bool_t KyotoArry= kTRUE;

Bool_t debug = kTRUE;

//Sn Beam offset
Double_t BDCX_off = 0.; // -20.77;  // Sn108 from run 2189


//setParameters
Double_t tx_right= 10.;
Double_t tx_left = 28.;
Double_t ty_top  = 10.;
Double_t ty_btm  = -10.;



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

  //  TString foutname = "../data/run"+sRun+"_ts.v1.03"+sdeb+".boff.root";
  //  TString foutname = "../data/run"+sRun+"_ts.v1.03"+sdeb+".6s4sb4.root";
  TString foutname = "../data/run"+sRun+"_asm.v1.03"+sdeb+".root";
  
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
  rootdir = "/data/spdaq01/recoData/v1.03+20160411.2016runs/rootfiles";
   //rootdir = "/cache/scr/spirit/experiment108Sn_2479-2480/";
   
  //rootdir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/recoData";
}

void setKATANADirectory(TString &ktnrootdir)
{
  ktnrootdir = "/data/spdaq01/katana/root/katana/";
}

void setKYOTODirectory(TString &kytrootdir)
{
  //kytrootdir = "/data/spdaq01/ridf/root/kyotoroot/";

  kytrootdir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/kyotoData/";
}

void Assemble_asm(Int_t irun = 2903)
{
  //2334//2480//2189
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
  gSystem->Load("KatanaRoot/katana_C.so");
  auto kChain = new TChain("tree");

  ULong_t event_number;
  Float_t max_veto;
  KatanaRoot *katanaroot = NULL;

  if(KATANA) {
    TString ktnrootdir;
    setKATANADirectory(ktnrootdir);

    kChain->Add(ktnrootdir+"run"+sRun+".root");

    kChain -> SetBranchAddress("Katana",&katanaroot);
  }


  //----- Kyoto Array --------------------
  gSystem->Load("KyotoRoot/STTriggerArray_C.so");
  auto kaChain = new TChain("tarray");
  
  STTriggerArray *kytanaroot = NULL;
  
  if(KyotoArry){
    TString kytrootdir;
    setKYOTODirectory(kytrootdir);
    kaChain->Add(kytrootdir+"run_"+sRun+".kyototrigbit.root");

    kaChain-> SetBranchAddress("STTriggerArray",&kytanaroot);
  }
    
   
  //-------------------- event number check --------------------
  Int_t nEvtTPC = 0;
  if(fChain) nEvtTPC = fChain -> GetEntries();
  cout << "Number of events in TPC: " << nEvtTPC << endl;

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
  TTree *tpc = new TTree("tpc","Beam and TPC track");
  if(tpc)
    cout << "Output file is " << foutname << endl;

  Int_t evtid;
  Int_t ntrack[5];
  Int_t bevt[7];
  Int_t bmpid;

  std::vector<Double_t> phi;
  std::vector<Int_t> gTrack;
  std::vector<Int_t>gTgTrack;
  std::vector<Double_t>hitKyotoLZ;
  std::vector<Double_t>hitKyotoLX;
  std::vector<Double_t>dzKyotoL;
  std::vector<Double_t>hitKyotoRZ;
  std::vector<Double_t>hitKyotoRX;
  std::vector<Double_t>dzKyotoR;

  //-- output
  tpc->Branch("irun",&irun,"irun/I");
  tpc->Branch("evtid",&evtid,"evtid/I");
  tpc->Branch("bevt",bevt,"bevt[7]/I");
  tpc->Branch("ntrack",ntrack,"ntrack[5]/I");
  tpc->Branch("bmpid",&bmpid,"bmpid/I");
  tpc->Branch("gTrack",&gTrack);
  tpc->Branch("gTgTrack",&gTgTrack);
  tpc->Branch("hitKyotoLZ",&hitKyotoLZ);
  tpc->Branch("hitKyotoLX",&hitKyotoLX);
  tpc->Branch("dzKyotoL",&dzKyotoL);
  tpc->Branch("hitKyotoRZ",&hitKyotoRZ);
  tpc->Branch("hitKyotoRX",&hitKyotoRX);
  tpc->Branch("dzKyotoR",&dzKyotoR);
  tpc->Branch("STTrack",&trackArray);
  tpc->Branch("STVertex",&vertexArray);
  tpc->Branch("aoq",&aoq,"aoq/F");
  tpc->Branch("z",&z,"z/F");
  tpc->Branch("tof",&tof,"tof/F");
  tpc->Branch("beta",&beta,"beta/F");
  tpc->Branch("tx",&tx,"tx/F");
  tpc->Branch("ty",&ty,"ty/F");
  tpc->Branch("ta",&ta,"ta/F");
  tpc->Branch("tb",&tb,"tb/F");
  tpc->Branch("max_veto",&max_veto,"max_veto/F");
  
  if(KyotoArry)
    tpc->Branch("STTriggerArray",&kytanaroot);



  if(debug)  nEntry = 5000;
  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  //-------------------- event loop --------------------
  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
  for (Int_t i=0; i<nEntry;i++) {
    evtid = i;
     
    for (Int_t m = 0; m < 7; m++) bevt[m] = 0;
    for (Int_t m = 0; m < 5; m++) ntrack[m] = 0;

    phi.clear();
    gTrack.clear();
    gTgTrack.clear();

    dzKyotoL.clear();
    hitKyotoLZ.clear();
    hitKyotoLX.clear();
    dzKyotoR.clear();
    hitKyotoRZ.clear();
    hitKyotoRX.clear();

    ngtrack   = 0;
    ngtgtrack = 0;

    if(i%1000 == 0) std::cout << "Process " << i << std::endl;

    // --------------- RIDF --------------
    ribfChain->GetEntry(i);
    // --------------- TPC ---------------
    fChain -> GetEntry(i);
    // --------------- KATANA ------------
    if(i < nEvtKTN)    kChain -> GetEntry(i);
    // --------------- KytoArray ----------
    if(i < nEvtKyt)    kaChain -> GetEntry(i);

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


    //-------------------- Vertex selection --------------------
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


      // ------- Correlation with Kyoto ------
      Double_t dKyoArrayLZ = 9999.;
      Int_t    kyotoidL = 1000;
      Double_t dKyoArrayRZ = 9999.;
      Int_t    kyotoidR = 1000;

      if(KyotoArry){
	Double_t trkKyoArryLx = trackFromArray->GetKyotoLHitX(); 
	Double_t trkKyoArryRx = trackFromArray->GetKyotoRHitX(); 

	Double_t trkKyoArryLz = trackFromArray->GetKyotoLHitZ(); 
	Double_t trkKyoArryRz = trackFromArray->GetKyotoRHitZ(); 

	kyotom = kytanaroot->knhit;
	cout << " nhit " << kyotom << " size z" << kytanaroot->kzpos.size()
	     << " x " << kytanaroot->kxpos.size() << endl;

	for(Int_t ik = 0; ik < kytanaroot->knhit; ik++){
	  
	  if(kytanaroot->kxpos.at(ik) == trkKyoArryLx && trkKyoArryLz > 0 ){

	    Double_t dz = kytanaroot->kzpos.at(ik) - trkKyoArryLz;
	    cout << "min dZL " << kytanaroot->kzpos.at(ik) << " : " << ik << endl;
	      if(abs(dKyoArrayLZ) > abs(dz)){
		kyotoidL = ik;
		dKyoArrayLZ = dz;

	      }
	    }

	  if(kytanaroot->kxpos.at(ik) == trkKyoArryRx && trkKyoArryRz > 0){
	      Double_t dz = kytanaroot->kzpos.at(ik) - trkKyoArryRz;
	      cout << "min dZR " << kytanaroot->kzpos.at(ik) << " : " << ik << endl; 
	      if(abs(dKyoArrayRZ) > abs(dz)) {
		kyotoidR = ik;
		dKyoArrayRZ = dz; 

	      }
	  }
	}	
      }
      

      if(kyotoidL != 100){
	hitKyotoLZ.push_back(kytanaroot->kzpos.at(kyotoidL));
	hitKyotoLX.push_back(kytanaroot->kxpos.at(kyotoidL));
      }
      else{
	hitKyotoLZ.push_back(-9999);
	hitKyotoLX.push_back(-9999);
      }	
      dzKyotoL.push_back(dKyoArrayLZ);

      if(kyotoidR != 100){
	hitKyotoRZ.push_back(kytanaroot->kzpos.at(kyotoidR));
	hitKyotoRX.push_back(kytanaroot->kxpos.at(kyotoidR));
      }
      else{
	hitKyotoRZ.push_back(-9999);
	hitKyotoRX.push_back(-9999);
      }	
      dzKyotoR.push_back(dKyoArrayRZ);


    }
  

    //--------------------
    
    if(ntrack[0] != 0)
      tpc->Fill();
  }
  
  tpc->Write();
  
  cout << " Output root file is : " << fout->GetName() << endl;
}
