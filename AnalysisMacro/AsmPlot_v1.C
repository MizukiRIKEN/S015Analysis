Int_t iRun = 2900;
Double_t  BeamBendingDistance = 250;

Double_t GetBeamPositionX(Int_t SnA, Double_t txin )
{
  Double_t Xa = 1.;
  Double_t Xb = 0.;

  if(SnA == 132){
    Xa = 0.976;
    Xb = 15.88;
  }
  else if(SnA = 108){
    Xa = 1.002;
    Xb = -20.39;
  }
  else if(SnA = 112){
    Xa = 0.975;
    Xb = 18.90;
  }
  else if(SnA = 124){
    Xa = 0.935;
    Xb = -15.96;
  }

  return txin*Xa + Xb;
  
}

Double_t GetBeamPositionY(Int_t SnA, Double_t tyin )
{
  Double_t Ya = 1.;
  Double_t Yb = 0.;;

  if(SnA == 132){
    Ya = 1.002;
    Yb = -225.23;
  }
  else if(SnA = 108){
    Ya = 1.002;
    Yb = -226.24;
  }
  else if(SnA = 112){
    Ya = 0.986;
    Yb = 225.23;
  }
  else if(SnA = 124){
    Ya = 0.987;
    Yb = -226.07;
  }

  return tyin*Ya + Yb;
  
}


void AsmPlot_v1()
{
  TString pRun = Form("%d",iRun);

  auto fChain = new TChain("tpc");  
  fChain -> Add(Form("../data/run%d_asm4.v1.03.d55x25c.root",iRun));

  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);

  Int_t bmpid;
  Float_t tx;
  Float_t ty;
  Float_t ta;
  Float_t tb;
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
  fChain->SetBranchAddress("tx",&tx);
  fChain->SetBranchAddress("ty",&ty);
  fChain->SetBranchAddress("ta",&ta);
  fChain->SetBranchAddress("tb",&tb);
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


