#include "AsmFlw_getRPlane.h"

void AsmFlw_getRPlane(Int_t nmax = -1)
{
  iRun = 2900;
  sRun = Form("%d",iRun);
  //  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);

  Open();
  OutputTree(nmax);

  
  //--------------------start
  //
  Int_t nEntry = fChain->GetEntries();
  std::cout << " Number of Events " << nEntry << std::endl;

  if(nmax >= 0) nEntry = nmax;

  for(Int_t ievt = 0; ievt < nEntry; ievt++){

    Initialize(ievt);

    //    std::cout << " Process " << ievt << std::endl;
    if(ievt%1000 == 0) std::cout << " Process " << ievt << std::endl;
    fChain->GetEntry(ievt);

    Int_t numTracksFromArray = aParticleArray->GetEntries();

    for(Int_t iPart = 0; iPart < numTracksFromArray; iPart++){
      STParticle *aParticle = (STParticle*) aParticleArray -> At(iPart);

      Double_t R_angle = 0.032;
      aParticle->RotateMomentum(R_angle);


      //if(aParticle->GetBestTrackFlag()){
      if(aParticle->GetBestTrackFlag() && aParticle->GetLinearPID()>10000){

	TVector3 pp(aParticle->GetMomentum().Unit().X(),
		    aParticle->GetMomentum().Unit().Y(),
		    aParticle->GetMomentum().Unit().Z());

	unitP += pp;
	

	TVector3 rpp(aParticle->GetRotatedMomentum().Unit().X(),
		     aParticle->GetRotatedMomentum().Unit().Y(),
		     aParticle->GetRotatedMomentum().Unit().Y());
	
	runitP += rpp;



	numGoodTrack++;
      }
    }

    rPlanePhi  = unitP.Phi();
    rPlanerPhi = runitP.Phi(); 

    unitPt  =  unitP.XYvector();
    runitPt = runitP.XYvector();

    flwp1->Fill();
  }

  flwp1->Write();
  std::cout << fout->GetName() << std::endl;
}

void Open()
{
  sRun = Form("%d",iRun);

  fChain = new TChain("flw");  
  fChain -> Add(Form("../data/run%d_flw2.root",iRun));

  aParticleArray = new TClonesArray("STParticle",150);

  fChain->SetBranchAddress("STParticle",&aParticleArray);

}

void Initialize(Int_t val)
{
  rPlanePhi = -10.;
  rPlanerPhi = -10.;
  unitP = TVector3(0,0,0);
  runitP = TVector3(0,0,0);
  unitPt = TVector2(0,0);
  runitPt= TVector2(0,0);
  numGoodTrack = 0;
}


void OutputTree(Int_t nmax)
{
  TString sdeb = ".s";
  if(nmax < 0)  sdeb = "";
  TString foutname = "../data/run"+sRun+"_flw3t.p1"+sdeb+".root";

  fout  = new TFile(foutname,"recreate");
  flwp1 = new TTree("flwp1","Beam and TPC track");

  cout << "Output file is " << foutname << endl;

  //-- output                                                                                                             
  flwp1->Branch("irun",&iRun,"irun/I");
  //  flwp1->Branch("z",&z,"z/F");
  //  flwp1->Branch("gtrk",&gtrk);
  flwp1->Branch("STParticle",&aParticleArray);
  flwp1->Branch("numGoodTrack",&numGoodTrack);
  flwp1->Branch("rPlanePhi",&rPlanePhi);
  flwp1->Branch("rPlanerPhi",&rPlanerPhi);
  flwp1->Branch("unitPt",&unitPt);
  flwp1->Branch("runitPt",&runitPt);
  
}

