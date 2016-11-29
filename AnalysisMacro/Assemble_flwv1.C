#define ASSEMBLE_FLWV1_CXX


Int_t irun = 2900;
//Int_t irun=3023;                                                                                           


Bool_t BigRIPS  = kTRUE;
Bool_t KyotoArry= kFALSE;
Bool_t KATANA   = kFALSE;

#include "Assemble_flwv1.h"

void Assemble_flwv1(Int_t nevt = -1)
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

  //----- Beam PID ------------------------
  BeamPID();

  //----- TPC data ------------------------
  SetTPC();
  Int_t nEvtTPC = 0;
  if(fChain) nEvtTPC = fChain -> GetEntries();
  cout << "Number of events in TPC: " << nEvtTPC << endl;
  if(nEvtTPC == 0) return;

  //----- BigRIPS data --------------------
  SetBigRIPS();
  Long64_t nEvents = 0;
  if(ribfChain) nEvents = ribfChain->GetEntries();
  cout << "Number of events in RIDF: " << nEvents << endl;

  //----- KATANA Array --------------------  
  Long64_t nEvtKTN = 0;
  if(KATANA)   {
    SetKATANARoot();
    nEvtKTN = kChain -> GetEntries();
    cout << "Number of events in KATANA: " << nEvtKTN << endl;
  }
  //----- Kyoto Array ---------------------
  Long64_t nEvtKyt = 0;
  if(KyotoArry) {
    SetKyotoArray();   
    nEvtKyt = kaChain -> GetEntries();
    cout << "Number of events in KyotoArray: " << nEvtKyt << endl;
  }

  //-------------------- event number check --------------------
  Int_t nEntry = nEvtTPC;
  if(nEvtTPC > nEvents)
    nEntry = nEvents;

  if(nEvtKTN != 0 && nEntry > nEvtKTN)
    nEntry = nEvtKTN;
  
  if(nEvtKyt != 0 && nEntry > nEvtKyt)
    nEntry = nEvtKyt;

  //--------------------output root--------------------
  OutputTree(nevt);

  if(nevt > 0)  nEntry = nevt;
  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  //-------------------- event loop --------------------
  // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 
  for (Int_t i=0; i<nEntry;i++) {
    Initialize(i);

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


    // --- Beam on the target  --------------------
    if(CheckBeamPosition(TVector2(tx,ty))) bevt[0] = 1;

    // --- BDC offset --------------------
    SetBeamOnTarget(TVector2(tx,ty));
    
    bmpid = GetBeamPID();
    
    // --- Vertex selection --------------------
    
    genfit::GFRaveVertex *vertex = (genfit::GFRaveVertex *) vertexArray -> At(0);
    if(vertex) {
       
      TVector2 aVertex(TVector2(vertex->getPos().X(), vertex->getPos().Y()));

      bevt[1] = 1;

      // Reaction Vertex selection
      if(CheckVertexZ(vertex->getPos().Z()))  bevt[2] = 1;

      if(CheckVertexXY(aVertex))	      bevt[3] = 1;
      
      if(CheckBDCvsVertexCorrelation(aVertex))bevt[4] = 1;
    }


    //-------------------- User Analysis --------------------
    Int_t numTracksFromArray = trackArray -> GetEntries();
    ntrack[0] = numTracksFromArray;

    tpcParticle->Clear();
    for (Int_t iTrack = 0; iTrack < numTracksFromArray; iTrack++) {
      
      STTrack *trackFromArray = (STTrack*) trackArray -> At(iTrack);      
      TClonesArray &ptpcParticle = *tpcParticle;

      new(ptpcParticle[iTrack]) STParticle(trackFromArray);
      STParticle *aParticle = (STParticle*) tpcParticle->At(iTrack);

      //--- check origin of the track ---;
      if(CheckBDCvsTrackCorrelation(aParticle->GetTargetPlaneVertex())) {
	
	aParticle->SetBDCCorrelationFlag(1);
	ntrack[1]++;
      }


      if(aParticle->CheckTrackonTarget()) {
	ntrack[2]++;
      }

      if(bevt[3]) aParticle->SetVertexAtTargetFlag(1);
      if(bevt[4]) aParticle->SetVertexBDCCorrelationFlag(1);
           
      Double_t R_angle = 6.*(3.141592/180.);
      aParticle->RotateMomentum(R_angle);
      

      //-------------------- end of User Analysis --------------------
    }
    flw->Fill();
  }
  flw->Write();
  
  cout << " Output root file is : " << fout->GetName() << endl;
}


//##################################################//
void OutputTree(Int_t nmax)
{
  TString sdeb = ".s";
  if(nmax < 0)  sdeb = "";
  TString foutname = "../data/run"+sRun+"_flw1"+sdeb+".root";

  fout = new TFile(foutname,"recreate");
  flw = new TTree("flw","Beam and TPC track");
  
  BeamonTarget = new TVector2();

  tpcParticle = new TClonesArray("STParticle",150);

  cout << "Output file is " << foutname << endl;

  //-- output
  flw->Branch("irun",&irun,"irun/I");
  flw->Branch("bevt",bevt,"bevt[7]/I");
  flw->Branch("aoq",&aoq,"aoq/F");
  flw->Branch("z",&z,"z/F");
  flw->Branch("BDCtc",&BeamonTarget);
  //  flw->Branch("STTrack", &trackArray);
  flw->Branch("STVertex",&vertexArray);  
  flw->Branch("STParticle",&tpcParticle);
}

void Initialize(Int_t ievt)
{
  evtid = ievt;

  for (Int_t m = 0; m < 7; m++) bevt[m] = 0;
  for (Int_t m = 0; m < 7; m++) ntrack[m] = 0;

  phi.clear();
  gTrack.clear();
  gTgTrack.clear();

  BeamonTarget->SetX(-999.);
  BeamonTarget->SetY(-999.);

}


Int_t GetBeamPID(){
  Int_t pid = 0;
  if(g132Sn){
    if(g132Sn->IsInside(aoq,z))
      pid = 20;
  }
  else if(g108Sn){
    if(g108Sn->IsInside(aoq,z))
      pid = 10;
  }

  return pid;
}

void BeamPID()
{
  sRun = Form("%d",irun);

  SnA = 0;
  if(irun >= 2174 && irun <= 2509)
    SnA = 108;
  else if( irun >= 2836 && irun <= 3039)
    SnA = 132;
  else if( irun >= 3058 && irun <= 3184)
    SnA = 124;
  else if( irun >= 2520 && irun <= 2653)
    SnA = 112;

  if(SnA == 132){
    auto gcutFile = new TFile("gcut132Sn.root");
    g132Sn=(TCutG*)gcutFile->Get("g132Sn");
    gcutFile->Close();
  }
  else if(SnA == 108){
    auto gcutFile = new TFile("gcut108Sn.root");
    g108Sn=(TCutG*)gcutFile->Get("g108Sn");
    gcutFile->Close();
  }
  else if(SnA == 124){
    auto gcutFile = new TFile("gcut124Sn.root");
    g124Sn=(TCutG*)gcutFile->Get("g124Sn");
    gcutFile->Close();
  }    
}


void SetDataDirectory()
{
  if(irun == 3023)
    rootDir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/recoData/";

  else if(irun >= 2331 && irun <= 3039) {  
    rootDir = "/data/spdaq01/recoData/v1.03+20160411.2016runs/rootfiles/";
  }
  else {
    rootDir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/recoData/";
  }
}

void SetKATANADirectory()
{
  ktnrootDir = "/data/spdaq01/katana/root/katana/";
}

void SetKYOTODirectory()
{
  //kytrootdir = "/data/spdaq01/ridf/root/kyotoroot/";
  //kytrootdir = "/cache/scr/spirit/mizuki/SpiRITROOT/macros/kyotoData/";
  kytrootDir = "/cache/scr/spirit/kaneko/kyotoroot/";
}




void SetBeamOnTarget(TVector2 vt)
{
  Double_t Xa = 1.;
  Double_t Xb = 0.;
  Double_t Ya = 1.;
  Double_t Yb = 0.;;

  if(SnA == 132){
    Xa = 0.976;
    Xb = -15.88;
    Ya = 1.002;
    Yb = -225.23;
  }
  else if(SnA == 108){
    Xa = 1.002;
    Xb = -20.39;
    Ya = 1.002;
    Yb = -226.24;
  }
  else if(SnA == 112){
    Xa = 0.975;
    Xb = -18.90;
    Ya = 0.986;
    Yb = 225.23;
  }
  else if(SnA == 124){
    Xa = 0.935;
    Xb = -15.96;
    Ya = 0.987;
    Yb = -226.07;
  }
  
  
  BeamonTarget->SetX(vt.X()*Xa + Xb);
  BeamonTarget->SetY(vt.Y()*Ya + Yb);
}

Bool_t CheckBeamPosition(TVector2 txy)
{
  if((txy.X() >= tx_right &&txy.X() <= tx_left) &&  
     (txy.Y() >= ty_btm  && txy.Y() <= ty_top ))   
    return kTRUE;
  else
    return kFALSE;
}

Bool_t CheckVertexZ(Double_t vz)
{
  if(vz <= vrt_Zmax && vz >= vrt_Zmin) 
    return kTRUE;
  else 
    return kFALSE;
}
				   
Bool_t CheckVertexXY(TVector2 vxy)
{
  if(vxy.X() >= trktgt_right && vxy.X() <= trktgt_left &&
     vxy.Y() >= trktgt_btm && vxy.Y() <= trktgt_top)
    return kTRUE;
  else
    return kFALSE;
}

Bool_t CheckBDCvsVertexCorrelation(TVector2 vxy)
{

  Double_t diffx = BeamonTarget->X() - vxy.X();
  Double_t diffy = BeamonTarget->Y() - vxy.Y();
  
  if((abs(diffx) < beamVx_nsig*beamVx_sigma) &&
     (abs(diffy) < beamVy_nsig*beamVy_sigma))
    return kTRUE;
  else
    return kFALSE;
}

Bool_t CheckBDCvsTrackCorrelation(TVector3 trackatTarget)
{
  Double_t diffx = BeamonTarget->X() - trackatTarget.X();
  Double_t diffy = BeamonTarget->Y() - trackatTarget.Y();

  if(abs(diffx) < trackVx_nsig*trackVx_sigma &&
     abs(diffy) < trackVy_nsig*trackVy_sigma)
    return kTRUE;
  else
    return kFALSE;
}


void SetKATANARoot()
{
  //----- KATANA data --------------------
  auto kChain = new TChain("kat");

  TString ktnrootdir;
  SetKATANADirectory();

  kChain->Add(ktnrootdir+"run"+sRun+".katana.root");

  kChain -> SetBranchAddress("nhit",&katanaM);
  kChain -> SetBranchAddress("evt",&event_number);
  kChain -> SetBranchAddress("bitpat",&bitpat,&bbitpat);

}

void SetKATANARoot_bt()
{
  //----- KATANA data --------------------
  gSystem->Load("KatanaRoot/KatanaRoot_Load_cpp.so");

  auto kChain = new TChain("tree");

  TString ktnrootdir;
  SetKATANADirectory();

  kChain->Add(ktnrootdir+"run"+sRun+".root");

  kChain -> SetBranchAddress("Katana",&katanaroot);
  kChain -> SetBranchAddress("TriggerBox",&triggerbox);

}

void SetKyotoArray()
{
  auto kaChain = new TChain("kyotoM");

  SetKYOTODirectory();
  kaChain-> Add(kytrootDir+"run"+sRun+".kyotopos.root");

  kaChain-> SetBranchAddress("multiplicity",&kynHit);
  kaChain-> SetBranchAddress("hitch",&kyhitch,&bkyhitch);
  kaChain-> SetBranchAddress("hitxpos",&kyhitx,&bkyhitx);
  kaChain-> SetBranchAddress("hitzpos",&kyhitz,&bkyhitz);
  
}
void SetBigRIPS()
{
  //----- BigRIPS data --------------------
  ribfChain = new TChain("beam");
    
  ribfChain ->Add("../bripsData/run"+sRun+".ridf.root");

  //----- Set branch addresses.
  ribfChain->SetBranchAddress("aoq",&aoq);
  ribfChain->SetBranchAddress("z",&z);
  ribfChain->SetBranchAddress("tof",&tof);
  ribfChain->SetBranchAddress("beta",&beta);
  ribfChain->SetBranchAddress("tx",&tx);
  ribfChain->SetBranchAddress("ty",&ty);
  ribfChain->SetBranchAddress("ta",&ta);
  ribfChain->SetBranchAddress("tb",&tb);
}

void SetTPC()
{
  fChain = new TChain("cbmsim");
  SetDataDirectory();
   
  Int_t i = 0;
  while(kTRUE){
    
    TString recoFile = Form("run"+sRun+"_s%d.reco.v1.03.root",i);
    //    cout << " recoFile " << rootDir+recoFile << endl;
    
    if(gSystem->FindFile(rootDir,recoFile)){
      fChain -> Add(recoFile);
    }
    else
      break;
    i++;
  }


  fChain -> SetBranchAddress("STTrack", &trackArray);
  fChain -> SetBranchAddress("STVertex",&vertexArray);

}

