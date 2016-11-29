void AsmPlot_KatanaHitm()
{
  Int_t iRun = 2900;
  TString pRun = Form("%d",iRun);

  auto fChain = new TChain("tpc");  
  //  fChain -> Add(Form("../data/run%d_asm4.v1.03.root",iRun));
  fChain -> Add(Form("../data/run%d_asm4.v1.03.d55x25c.root",iRun));

  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);


  Int_t KatanaMult;
  Int_t trknKATANA;
  Int_t foundKATANA;  
  std::vector<Double_t>*KatanaPadX = 0;
  std::vector<Int_t>   *hitKATANAID= 0;
  std::vector<Double_t>*hitKATANAX = 0;
  std::vector<Double_t>*hitKATANAY = 0;
  std::vector<Double_t>*dxhitKATANA= 0;
  std::vector<Double_t>*bitpattern = 0;
  std::vector<Double_t>*KatanaPadID= 0;
  std::vector<UShort_t>*fndKATANA  = 0;

  TBranch *bKatanaPadX = 0;
  TBranch *bhitKATANAID= 0;
  TBranch *bhitKATANAX = 0;
  TBranch *bhitKATANAY = 0;
  TBranch *bdxhitKATANA= 0;
  TBranch *bbitpattern = 0;
  TBranch *bKatanaPadID= 0;
  TBranch *bfndKATANA  = 0;


  fChain->SetBranchAddress("KatanaPadX", &KatanaPadX , &bKatanaPadX);
  fChain->SetBranchAddress("KatanaPadID",&KatanaPadID, &bKatanaPadID);
  fChain->SetBranchAddress("hitKATANAID",&hitKATANAID, &bhitKATANAID);
  fChain->SetBranchAddress("hitKATANAX", &hitKATANAX,  &bhitKATANAX);
  fChain->SetBranchAddress("hitKATANAY", &hitKATANAY,  &bhitKATANAY);
  fChain->SetBranchAddress("dxhitKATANA",&dxhitKATANA, &bdxhitKATANA);
  fChain->SetBranchAddress("fndKATANA",  &fndKATANA,   &bfndKATANA);

  fChain->SetBranchAddress("KatanaMult",&KatanaMult);
  fChain->SetBranchAddress("trknKATANA",&trknKATANA);
  fChain->SetBranchAddress("foundKATANA",&foundKATANA);


  //--------------------start
  //
  Int_t nEntry = fChain->GetEntries();
  cout << " Total Events " << nEntry << endl;

  //  auto c1 = new TCanvas("c1","c1",1000,45,400,1147);
  auto c2 = new TCanvas("c2","c2",0,100,800,600);
  //  auto c3 = new TCanvas("c3","c3",100,100,800,600);
  
  // c1->Draw();
  // c1->SetTopMargin(0.035f);
  // c1->SetBottomMargin(0.054f);


  c2->Divide(1,2);
  // c2->Draw();
  // c3->Draw();

  TString h1name = "Katana hit pattern";
  TString h2name = "Extrapolated hit ";
  auto h1 = new TH1D("h1","KATANA hit pattern",140,-1050,350);   
  auto h2a= new TH1D("h2a","Track hit on KATANA (associated)",140,-1050,350);   
  auto h2b= new TH1D("h2b","Track hit on KATANA (no association)",140,-1050,350);   

  auto h4a = new TH2D("h4a","track X-Y on KATANA within one paddle",150,-1050,350,100,-400,0);
  auto h4b = new TH2D("h4b","track X-Y on KATANA within one paddle",150,-1050,350,100,-400,0);

  auto h3 = new TH2D("h3","dx on KATANA ",26,-1050,350,100,-200,200);

  auto h5 = new TH2D("h5","track X-Y on KATANA",150,-1050,350,100,-400,0);

  auto n1 = new TH2D("n1","Number of hits in total;# of track; Katana Mult",25,0,25,20,0,25);
  auto n2 = new TH2D("n2","Number of hits event by event",25,0,25,20,0,25);

  auto m1  = new TH2D("m1" ,"KATANA hit pattern",100,1200.,2000,28,-1050,350);
  auto m3  = new TH2D("m3" ,"Track hit pattern ",100,1200.,2000,28,-1050,350);
  auto m3a = new TH2D("m3a","Track hit pattern(associated)",100,1200.,2000,28,-1050,350);
  auto m2  = new TH2D("m2" ,"Track hit pattern (associated)",100,-400,0,150,-1050,350);
  auto m2a = new TH2D("m2a","Track hit pattern",100,-400,0,150,-1050,350);


  for(Int_t ievt = 0; ievt < nEntry; ievt){
    cout << "Event ID " << ievt << endl;

    h1->Reset();
    h2a->Reset();
    h2b->Reset();
    h4a->Reset();
    h4b->Reset();

    fChain->GetEntry(ievt);
    bKatanaPadX ->GetEntry(ievt);
    bhitKATANAID->GetEntry(ievt);
    bfndKATANA  ->GetEntry(ievt);
    bhitKATANAX ->GetEntry(ievt);
    bhitKATANAY ->GetEntry(ievt);
    
    cout << " KATANA hit " << KatanaPadX->size() << endl;
    for(Int_t ik = 0; ik < (Int_t) KatanaPadX->size(); ik++){

      h1->Fill(KatanaPadX->at(ik) );
 
    }

    cout << " hit on KATANA "<< foundKATANA << endl;

    for(Int_t ik = 0; ik < (Int_t) hitKATANAID->size(); ik++){
      if(fndKATANA->at(ik)) {
	h2a->Fill(hitKATANAX->at(ik));
	h4a->Fill(hitKATANAX->at(ik),hitKATANAY->at(ik));
      }
      else {
	h2b->Fill(hitKATANAX->at(ik));
	h4b->Fill(hitKATANAX->at(ik),hitKATANAY->at(ik));
      }
      
    }

    c2->cd(1);
    h1->SetFillStyle(1001);
    h1->SetFillColor(7);
    h1->SetLineColor(4);
    TString sLavel = Form("KATNAA Hit : Event %d",ievt);
    h1->SetTitle(sLavel);
    h1->Draw();


    h2a->SetLineColor(8);
    h2a->Draw("same");
    h2b->SetLineColor(2);
    h2b->Draw("same");

    c2->cd(2);
    h4a->SetMarkerStyle(8);
    h4a->SetMarkerColor(8);
    h4a->SetMarkerSize(1.5);
    h4a->Draw();

    h4b->SetMarkerStyle(8);
    h4b->SetMarkerColor(2);
    h4b->SetMarkerSize(1.5);
    h4b->Draw("same");

    c2->Update();

    TString pdfout = Form("KHit_Run%d_evt",iRun);
    TString pdfout2 = Form("%d",ievt); 
    c2->SaveAs(pdfout+pdfout2+".png");


    Int_t ans;
    cout << " Next Event ID? : 0(next); ###(Event ID); -1(stop)" << endl;
    cin  >> ans; 
    if(ans == 0) {
      ievt++;
      continue;
    }
    else if(ans < 0) {
      break;
    }
    else if(ans > 0) {
      ievt = ans;
      continue;
    }
    else break;

  }
}


