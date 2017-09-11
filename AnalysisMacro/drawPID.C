void drawPID
(
 bool drawPIDLine = true,
 STPID::PID selectPID = STPID::kNon, // kPion, kProton, kDeuteron, kTriton, k3He, k4He
 Int_t nbins = 200,
 Int_t p1 = -500,
 Int_t p2 = 2500,
 Int_t dedx1 = 0,
 Int_t dedx2 = 500
 )

{

  

  auto tree = new TChain("rflw");



  gStyle -> SetOptStat(0);
  gStyle -> SetPadBottomMargin(0.10);
  gStyle -> SetPadLeftMargin(0.11);
  gStyle -> SetPadRightMargin(0.12);
  gStyle -> SetTitleFontSize(0.06);

  // auto tree = new TChain("flw");
  // tree -> Add("../data/run2922_flw_v3.0.root");


  TClonesArray *trackArray = nullptr;
  TClonesArray *vertexArray = nullptr;

  Int_t         ntrack[7];
  tree -> SetBranchAddress("STParticle", &trackArray);
  tree -> SetBranchAddress("ntrack",ntrack);
  //  tree -> SetBranchAddress("STVertex", &vertexArray);

  auto hVertexXY = new TH2D("hVertexXY",";x (mm);y (mm)",100,-100,100,100,-300,-150);
  auto hVertexZY = new TH1D("hVertexZY",";z (mm)",200,-100,1344);

  auto histPID  = new TH2D("histPID ",";p (MeV); dEdx (ADC/mm)",nbins,p1,p2,nbins,dedx1,dedx2);
  auto histPIDs = new TH2D("histPIDs","My Selected Tracks ; p (MeV); dEdx (ADC/mm)",nbins,p1,p2,nbins,dedx1,dedx2);
  histPID -> SetTitleSize(0.04,"xy");
  histPID -> SetTitleOffset(1.4,"y");
  histPID -> SetTitleOffset(1.1,"x");
  histPID -> GetXaxis() -> CenterTitle();
  histPID -> GetYaxis() -> CenterTitle();

  auto histAcc  = new TH2D("histAcc","Genie's selection;#theta in lab ; #phi",100,0.,1.6,100,-3.2,3.2);
  auto histAcc1 = new TH2D("histAcc1","My Best;#theta in lab ; #phi",100,0.,1.6,100,-3.2,3.2);
  auto histphi  = new TH1D("histphi" ,"Genie's selection; #phi",100,-3.2,3.2);

  auto histMAcc  = new TH1I("histMAcc ","My Best; Number of Tracks"          ,70,0,70);
  auto histMAcc1 = new TH1I("histMAcc1","Genei's selection; Number of Tracks",70,0,70);


  Int_t feventID = -1;
  Int_t ftrackID = -1;
  Int_t fpid = -1;
  Double_t fp = -999;
  Double_t fdedx = -999;

  auto pidTest = new STPIDTest();



  Int_t nEvents = tree -> GetEntries();

  cout << " number of events " << nEvents << endl;

  for (Int_t iEvent = 0; iEvent < nEvents; ++iEvent) {
  //  for (Int_t iEvent = 0; iEvent < 18000; ++iEvent) {
    if (iEvent % 500 == 0)
      cout << "Event " << iEvent << endl;


    tree -> GetEntry(iEvent);

    // if (vertexArray -> GetEntries() != 1){
    //   //      cout << " vertexArray -> GetEntries() " << vertexArray -> GetEntries() << endl;                                           
    //   continue;
    // }

    Int_t ntra[] = {0, 0};

    Int_t nTracks = trackArray -> GetEntries();
    for (auto iTrack = 0; iTrack < nTracks; ++iTrack) {
      auto track = (STParticle *) trackArray -> At(iTrack);

      if(iTrack == 0){
	auto posVertex = track -> GetTrackAtTarget();
	hVertexXY -> Fill(posVertex.X(),posVertex.Y());
	hVertexZY -> Fill(posVertex.Z());
      }


      auto P3  = track -> GetMomentum();
      auto pz  = P3.Z();
      if( pz < 0 ){
	//	P3 = TVector3(-P3.X(), -P3.Y(),-P3.Z());
	P3 = -P3;
      }

      auto phi = P3.Phi();
      auto thet= P3.Theta();
      auto p = track->GetCharge() * track->GetMomentum().Mag();
      auto dedx = track->GetdEdx();

      auto pid   = track->GetPID();

      //      if(track->GetBestTrackFlag() && track -> GetReactionPlaneFlag() > 10 ) {
      if(track -> GetReactionPlaneFlag() > 10 ) {
	ntra[1]++;

	histPIDs  -> Fill(p, dedx);
	histAcc1 -> Fill(thet, phi);
      }

      if (selectPID != STPID::kNon && pid != selectPID) continue;
      if (track -> GetVertexID() < 0 ) continue;
      if (track -> GetdEdxPointSize() < 10 ) continue;
      //      if (thet > 0.8 ) continue;

      ntra[0]++; 
      histAcc -> Fill(thet, phi);
      histphi  -> Fill(phi);

      histPID -> Fill(p, dedx);

    }

    if(ntra[1]>0)   histMAcc1 ->Fill(ntra[1]);
    histMAcc->Fill(ntra[0]);

  }

  auto cvsPID = new TCanvas("cvsPID","",20,20,1200,700);
  histPID -> Draw("colz"); 

  for (auto ipid = 0; ipid < STPID::GetNUMSTPID(); ++ipid) {
    auto pid = static_cast<STPID::PID>(ipid);
    pidTest -> GetdEdxFunction(pid) -> Draw("same");
  }


  auto cvsPIDs = new TCanvas("cvsPIDs","",1200,700);
  histPIDs -> SetMaximum(120);
  histPIDs -> Draw("colz"); 

  for (auto ipid = 0; ipid < STPID::GetNUMSTPID(); ++ipid) {
    auto pid = static_cast<STPID::PID>(ipid);
    pidTest -> GetdEdxFunction(pid) -> Draw("same");
  }

  auto cvsVXY = new TCanvas("cvsVXY","",20,20,400,300);
  hVertexXY -> Draw("colz");
  auto cvsVZY = new TCanvas("cvsVZY","",420,20,400,300);
  cvsVZY -> SetLogy();
  hVertexZY -> Draw("colz");

  auto cvsAcc = new TCanvas("cvsAcc","",700,800);
  cvsAcc->Divide(2,2);
  UInt_t id = 1;

  cvsAcc->cd(id); id++;
  //  histphi->Draw();
  histAcc->SetMaximum(250);
  histAcc->Draw("colz");

  cvsAcc->cd(id); id++;
  histMAcc->Draw();


  cvsAcc->cd(id); id++;
  histAcc1->Draw("colz");

  cvsAcc->cd(id); 
  histMAcc1->Draw();

  cout << " Entry    " << endl;
  cout << " hVertexXY" << hVertexXY->GetEntries() << endl;
  cout << " histAcc1 " << histAcc1->GetEntries()  << " : " << histAcc->GetEntries() << endl;


  cout << " Events number " << histMAcc1->GetEntries() << " : " << histMAcc->GetEntries() << endl;

}

void proton()
{


}
