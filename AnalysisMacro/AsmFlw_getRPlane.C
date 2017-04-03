#include "AsmFlw_getRPlane.h"

void AsmFlw_getRPlane(Int_t nmax = -1)
{
  //  iRun = 2477;
  iRun = 2900;
  sRun = Form("%d",iRun);
  //  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);

  Open();
  OutputTree(nmax);

  
  //--------------------start
  //
  TDatime dtime;
  TDatime btime(dtime);
  
  Int_t nEntry = fChain->GetEntries();
  std::cout << " Number of Events " << nEntry << std::endl;

  if(nmax >= 0) nEntry = nmax;

  for(Int_t ievt = 0; ievt < nEntry; ievt++){

    Initialize(ievt);

    //    std::cout << " Process " << ievt << std::endl;
    if(ievt%1000 == 0) {
      dtime.Set();
      std::cout << "Process " << ievt << " Time >> " << dtime.GetHour() << ":" << dtime.GetMunute() << ":"<< dtime.GetSecond() 
		<< " ---- " << (dtime.Second()-btime.Second())/60. 
		<<std::endl;
    }
    fChain->GetEntry(ievt);

    Int_t numTracksFromArray = aParticleArray->GetEntries();

    for(Int_t iPart = 0; iPart < numTracksFromArray; iPart++){
      STParticle *aParticle = (STParticle*) aParticleArray -> At(iPart);

      Double_t R_angle = 0.032;
      aParticle->RotateMomentum(R_angle);

      //if(aParticle->GetBestTrackFlag()){
      if(aParticle->GetBestTrackFlag() && aParticle->GetLinearPID()>10000){

	TVector3 pp(aParticle->GetMomentum().X(),
		    aParticle->GetMomentum().Y(),
		    aParticle->GetMomentum().Z());
	vPart.push_back(pp);
	iphi.push_back(pp.Phi());
	unitP += pp.Unit();

	
	//	if(aParticle->GetMomentum().Theta()<0.4) {
	  unitP_lang += pp;
	  //	  cout << "<< unitPlang " <<endl; 
	  //	}
	TVector3 rpp(aParticle->GetRotatedMomentum().Unit().X(),
		     aParticle->GetRotatedMomentum().Unit().Y(),
		     aParticle->GetRotatedMomentum().Unit().Y());
	
	runitP += rpp;

	if(aParticle->GetRotatedMomentum().Theta()<0.04)
	  runitP_lang += rpp;

	numGoodTrack++;
      }
    }

    rPlanePhi  = unitP.Phi();
    rPlanerPhi = runitP.Phi(); 

    unitPt  =  unitP.XYvector();
    runitPt = runitP.XYvector();

    ngtrack = vPart.size();
    for(Int_t i = 0; i < ngtrack; i++){
       
      TVector3 ExcRP;
      for(Int_t k = 0; k < ngtrack; k++){
	if(i != k ) ExcRP += vPart.at(k).Unit();
      }
      
      deltphi.push_back(TVector2::Phi_mpi_pi( vPart.at(i).Phi() - ExcRP.Phi()));
      px.push_back(vPart.at(i).X());
      py.push_back(vPart.at(i).Y());
      pz.push_back(vPart.at(i).Z());
    }

    if(numGoodTrack > 0)
      flwp1->Fill();
  }
    
  fout->Write();
  std::cout << fout->GetName() << std::endl;
}

void Open()
{
  sRun = Form("%d",iRun);

  fChain = new TChain("flw");  
  fChain -> Add(Form("../data/run%d_flw_v2.root",iRun));
  //  fChain = new TChain("sasm");  
  //fChain -> Add(Form("../data/run%d_v1.root",iRun));

  aParticleArray = new TClonesArray("STParticle",150);

  fChain->SetBranchAddress("STParticle",&aParticleArray);

}

void Initialize(Int_t val)
{
  rPlanePhi  = -10.;
  rPlanerPhi = -10.;
  unitP  = TVector3(0,0,0);
  runitP = TVector3(0,0,0);
  unitPt = TVector2(0,0);
  runitPt= TVector2(0,0);

  unitP_lang  = TVector3(0,0,0);
  runitP_lang = TVector3(0,0,0);
  
  numGoodTrack = 0;
  ngtrack = 0;

  vPart.clear();
  px.clear();
  py.clear();
  pz.clear();

}


void OutputTree(Int_t nmax)


{
  TString sdeb = ".s";
  if(nmax < 0)  sdeb = "";
  //  TString foutname = "../data/run"+sRun+"_flw3t.p1"+sdeb+".root";
  TString foutname = "../data/run"+sRun+"_v2flw.p1"+sdeb+".root";

  fout  = new TFile(foutname,"recreate");
  flwp1 = new TTree("flwp1","Beam and TPC track");

  cout << "Output file is " << foutname << endl;

  //-- output                                                                                                             
  flwp1->Branch("irun",&iRun,"irun/I");
  flwp1->Branch("STParticle",&aParticleArray);
  flwp1->Branch("numGoodTrack",&numGoodTrack);
  flwp1->Branch("ngtrack",&ngtrack,"ngtrack/I");
  flwp1->Branch("px",&px);
  flwp1->Branch("py",&py);
  flwp1->Branch("pz",&pz);
  flwp1->Branch("deltphi",&deltphi);
  flwp1->Branch("unitP",&unitP);

  // flwp1->Branch("rPlanePhi",&rPlanePhi);
  // flwp1->Branch("rPlanerPhi",&rPlanerPhi);
  // flwp1->Branch("unitPt",&unitPt);
  // flwp1->Branch("runitPt",&runitPt);
  // flwp1->Branch("unitP_lang",&unitP_lang);
  // flwp1->Branch("runitP_lang",&runitP_lang);
  
}
