void Open()
{
}

void AsmPlot_KatanaHitEff()
{
  Int_t iRun = 2331;
  TString pRun = Form("%d",iRun);

  auto fChain = new TChain("tpc");  
  fChain -> Add(Form("../data/run%d_asm5.v1.03.root",iRun));
  //  fChain -> Add(Form("../data/run%d_asm4.v1.03.d55x25c.root",iRun));

  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);

  Int_t bmpid;
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

  fChain->SetBranchAddress("bmpid",&bmpid);
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

  auto c1 = new TCanvas("c1","c1",1000,45,1000,800);
  auto c2 = new TCanvas("c2","c2",0,100,800,600);
  auto c3 = new TCanvas("c3","c3",1000,100,800,600);
  
  c1->Divide(3,4);
  // c1->SetTopMargin(0.035f);
  // c1->SetBottomMargin(0.054f);


  c2->Divide(1,2);
  c3->Divide(1,2);


  TString h1name = "Katana hit pattern";
  TString h2name = "Extrapolated hit ";
  auto h1 = new TH1D("h1","KATANA hit pattern",140,-1050,350);   
  auto h1n= new TH1D("h1n","KATANA hit pattern (no track)",140,-1050,350);   
  auto h2a= new TH1D("h2a","Track hit on KATANA (All)",140,-1050,350);   
  auto h2b= new TH1D("h2b","Track hit on KATANA (found)",140,-1050,350);   


  auto h4a = new TH2D("h4a","track X-Y on KATANA ",150,-1050,350,100,-400,0);
  auto h4b = new TH2D("h4b","track X-Y on KATANA (found)",150,-1050,350,100,-400,0);
  auto h4c = new TH2D("h4c","track X-Y on KATANA (not found)",150,-1050,350,100,-400,0);

  auto h3 = new TH2D("h3","dx on KATANA ",26,-1050,350,100,-200,200);

  auto h5 = new TH2D("h5","track X-Y on KATANA",150,-1050,350,100,-400,0);

  auto n1 = new TH2D("n1","Number of hits in total;# of track; Katana Mult",25,0,25,20,0,25);
  auto n2 = new TH2D("n2","Number of hits event by event",25,0,25,20,0,25);


  TH1D *hkn[12];
  TH1D *hkm[12];
  for(Int_t i = 0; i < 12; i++){
    TString shname = Form("hkm_%d",i);
    TString shtitle= Form("Hiting track on paddle_%d",i+1);
    hkm[i] = new TH1D(shname,shtitle,10,0,10);

    
    shname = Form("hkn_%d",i);
    hkn[i] = new TH1D(shname,shtitle,10,0,10);
    
  }



  for(Int_t ievt = 0; ievt < nEntry; ievt++){
    // cout << "Event ID " << ievt << endl;


    fChain->GetEntry(ievt);
    bKatanaPadX ->GetEntry(ievt);
    bKatanaPadID->GetEntry(ievt);
    bhitKATANAID->GetEntry(ievt);
    bfndKATANA  ->GetEntry(ievt);
    bhitKATANAX ->GetEntry(ievt);
    bhitKATANAY ->GetEntry(ievt);
    
    //    cout << "mpid " << bmpid << endl;

    if(bmpid !=10 && bmpid !=20) continue;

    //    cout << " KATANA hit " << KatanaPadX->size() << endl;
    for(Int_t ik = 0; ik < (Int_t) KatanaPadX->size(); ik++){

      h1->Fill(KatanaPadX->at(ik) );
 

      Int_t  ntrack= 0;
      for(Int_t ij = 0; ij < (Int_t) hitKATANAID->size(); ij++){
	if(hitKATANAID->at(ij) == KatanaPadID->at(ik) && fndKATANA->at(ij)==1){
	  ntrack++;
	}
      }
      
      Int_t ipad = KatanaPadID->at(ik);
      hkm[ipad]->Fill(ntrack);
      
      if(ntrack == 0)
	h1n->Fill(KatanaPadX->at(ik));
    }

    //    cout << " hit on KATANA "<< hitKATANAID->size() << endl;

    for(Int_t ik = 0; ik < (Int_t) hitKATANAID->size(); ik++){
      h2a->Fill(hitKATANAX->at(ik));
      h4a->Fill(hitKATANAX->at(ik),hitKATANAY->at(ik));

      if(fndKATANA->at(ik)) {
	h2b->Fill(hitKATANAX->at(ik));
	h4b->Fill(hitKATANAX->at(ik),hitKATANAY->at(ik));
      }
      else{
	h4c->Fill(hitKATANAX->at(ik),hitKATANAY->at(ik));
      }      
    }
  }

    c2->cd(1);
    // h1->SetFillStyle(1001);
    // h1->SetFillColor(7);
    // h1->SetLineColor(4);
    // h1->Draw();

    h2a->SetLineColor(3);
    h2a->Draw();
    h2b->SetLineColor(2);
    h2b->Draw("same");
    auto tlh2 = new TLegend(0.15,0.7,0.3,0.9,"");
    tlh2->AddEntry(h2a,"All Tracks","lp");
    tlh2->AddEntry(h2b,"Associated Tracks","lp");
    tlh2->Draw();

    c2->cd(2);
    h1->SetLineColor(3);
    h1->Draw();

    h1n->SetLineColor(2);
    h1n->Draw("same");
    auto tlh1 = new TLegend(0.15,0.7,0.3,0.9,"");
    tlh1->AddEntry(h1,"All KATANA hit","lp");
    tlh1->AddEntry(h1n,"Not Associated hit","lp");
    tlh1->Draw();




    c2->Update();


    c3->cd(1);
    TH1D *h2c = new TH1D((*h2b)/(*h2a));
    h2c->Draw();
    
    c3->cd(2);
    TH1D *h1c = new TH1D((*h1n)/(*h1));
    h1c->Draw();
    
    c3->Update();

    for(Int_t i = 0; i < 12; i++){
      c1->cd(1+i);

      //      c3->cd(1+i);
      Double_t entry = hkm[i]->GetEntries();
      if(entry > 0 )
	hkm[i]->Scale(1./entry);
      hkm[i]->Draw();

    }
    c1->Update();



    
    //   Int_t ans;
    //   cout << " Next Event ID? : 0(next); ###(Event ID); -1(stop)" << endl;
    //   cin  >> ans; 
    //   if(ans == 0) {
    //     ievt++;
    //     continue;
    //   }
    //   else if(ans < 0) {
    //     break;
    //   }
    //   else if(ans > 0) {
    //     ievt = ans;
    //     continue;
    //   }
    //   else break;

    // }
}


