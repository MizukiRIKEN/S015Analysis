int AsmPlot_KatanaHit2()
{

  Int_t iRun = 2903;
  TString pRun = Form("%d",iRun);

  
  auto fChain = new TChain("tpc");
  fChain -> Add(Form("../data/run%d_asm3.v1.03.s.root",iRun));

  
  


  //--------------------start
  //
  Int_t nEntry = tpc->GetEntries();


  auto c2 = new TCanvas("c2","c2",0,0,800,600);
  auto c3 = new TCanvas("c3","c3",0,1000,800,600);
  
  c2->Divide(1,2);

  TString h1name = "Katana hit pattern";
  TString h2name = "Extrapolated hit ";
  auto h1 = new TH1D("h1",h1name,24,-1010,320);   
  auto h1a= new TH1D("h1a",h1name,240,-1010,320);   
  auto h2 = new TH1D("h2",h2name,150,-1010,320);
  auto h2a= new TH1D("h2a",h2name,150,-1010,320);
  auto h3 = new TH2D("h3","dx on KATANA ",26,-1010,320,100,-200,200);
  auto h4 = new TH2D("h4","track X-Y on KATANA within one paddle",150,-1010,320,100,-400,0);
  auto h5 = new TH2D("h5","track X-Y on KATANA",150,-1010,320,100,-400,0);

  auto n1 = new TH2D("n1","Number of hits in total;# of track; Katana Mult",25,0,25,20,0,25);
  auto n2 = new TH2D("n2","Number of hits event by event",25,0,25,20,0,25);

  auto lim = new TBox(-900,50,400,-50);

  tpc->Draw("KatanaMult:trknKATANA>>n1","","box");  

  Int_t ievt = 0;
  while(ievt < 2 and ievt < nEntry){
    c2->cd(1);
    h1name = Form("KATANA pad X %d",ievt);
    tpc->Draw("KatanaPadX>>h1","","",1,ievt);

    for(Int_t k = 0; k < (Int_t)KatanaPadX.size(); k++){
      for(Int_t ik = 0; ik < 100; ik++){
	Double_t dx = KatanaPadX.at(KatanaPadID.at(k)) + ik;
	h1a->Fill(dx,0.01);
      }
    }

    //    auto h2 = new TH1D("h2",h2name,26,-1010,320);   
    h2name = Form("Track_X hits on KATANA %d",ievt);
    tpc->Draw("hitKATANAX>>h2","fndKATANA==1","",1,ievt);
    tpc->Draw("hitKATANAX>>h2a","fndKATANA==0","",1,ievt);

    h1a->SetFillStyle(1001);
    h1a->SetFillColor(7);
    h1a->SetLineColor(4);
    h1a->Draw();
    h2->SetLineColor(3);
    h2->Draw("same");
    h2a->SetLineColor(2);
    h2a->Draw("same");

    c2->cd(2);
    // h3->SetMarkerStyle(kMultiply);    
    // h3->SetMarkerColor(4);
    // tpc->Draw("dxhitKATANA:hitKATANAX>>h3","","",1,ievt);
    // //    lim->SetFillColorAlpha(kBlue, 0.2);
    // //    lim->Draw();


    // c2->cd(3);
    h4->SetMarkerStyle(kFullCircle);    
    h4->SetMarkerColor(3);
    tpc->Draw("hitKATANAY:hitKATANAX>>h4","fndKATANA","",1,ievt);
    h5->SetMarkerStyle(kFullCircle);    
    h5->SetMarkerColor(2);
    tpc->Draw("hitKATANAY:hitKATANAX>>h5","fndKATANA==0","same",1,ievt);

    c2->Update();

    // -- c3 --
    
    c3->cd();
    n1->Draw("box");
    n2->SetMarkerStyle(kFullCircle);
    n2->SetMarkerColor(4);
    tpc->Draw("KatanaMult:trknKATANA>>n2","","same",1,ievt);  

    c3->Update();
    
    cout << " Event ID " << ievt << endl;
    sleep(5);
    ievt++;
  }

}
