#include "AsmFlw_getRPlane.h"

void AsmFlw_getRPlane(Int_t nmax = -1)
{
  iRun = 2900;

  //  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);

  Open();
  OutputTree(nmax);

  
  //--------------------start
  //
  Int_t nEntry = fChain->GetEntries();
  std::cout << " Number of Events " << nEntry << std::endl;


  for(Int_t ievt = 0; ievt < nEntry; ievt++){

    Initialize(ievt);

    if(ievt%1000 == 0) std::cout << " Process " << ievt << std::endl;
    fChain->GetEntry(ievt);

    Int_t numTracksFromArray = aParticleArray->GetEntries();

    for(Int_t iPart = 0; iPart < numTracksFromArray; iPart++){
      STParticle *aParticle = (STParticle*) aParticleArray -> At(iPart);

      if(aParticle->GetBestTrackFlag()){

	TVector2 pp(aParticle->GetTargetPlaneMomentum().Unit().X(),
		    aParticle->GetTargetPlaneMomentum().Unit().Y());
	unitP += pp;

	numGoodTrack++;
      }
    }

    rPlanePhi = unitP.Phi();


    flwp1->Fill();
  }

  flwp1->Write();
  std::cout << fout->GetName() << std::endl;
}

void Open()
{
  pRun = Form("%d",iRun);

  fChain = new TChain("flw");  
  fChain -> Add(Form("../data/run%d_flw1.root",iRun));

  fChain->SetBranchAddress("STParticle",&aParticleArray);

}

void Initialize(Int_t val)
{
  rPlanePhi = -10.;
  unitP = TVector2(0,0);
  numGoodTrack = 0;
}


void OutputTree(Int_t nmax)
{
  TString sdeb = ".s";
  if(nmax < 0)  sdeb = "";
  TString foutname = "../data/run"+sRun+"_flw1.p1"+sdeb+".root";

  fout  = new TFile(foutname,"recreate");
  flwp1 = new TTree("flwp1","Beam and TPC track");

  cout << "Output file is " << foutname << endl;

  //-- output                                                                                                             
  flwp1->Branch("irun",&iRun,"irun/I");
  //  flwp1->Branch("z",&z,"z/F");
  //  flwp1->Branch("gtrk",&gtrk);
  flwp1->Branch("numGoodTrack",&numGoodTrack);
  flwp1->Branch("rPlanephi",&rPlanePhi);

  
}

