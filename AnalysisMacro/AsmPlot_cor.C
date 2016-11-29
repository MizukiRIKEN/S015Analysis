{
  gROOT->Macro("AsmPlot_Open_full.C");


  TCut FindKyotoL = "dzKyotoL<40&&dzKyotoL>-40";
  TCut FindKyotoR = "dzKyotoR<40&&dzKyotoR>-40";
  TCut kmult[4] = {"kyotom>=4&&kyotom<6","kyotom>=6&&kyotom<10","kyotom>=10&&kyotom<15","kyotom>=15"};


  TCut ggt = sn&&"gTrack&&gTgTrack";


  gStyle->SetOptFit(1111);

  //--------------------start
  //

  //  TString cname = Form("c%d",i);
  auto c1 = new TCanvas("c1","Right",0,0,1000,800);
  c1->Divide(2,2);
  auto c2 = new TCanvas("c2","Left",0,0,1000,800);
  c2->Divide(2,2);

  c1->SetLogy();
  c2->SetLogy();


  TH1D *hsl[4];
  TH1D *hslc[4];
  TH1D *hsr[4];
  TH1D *hsrc[4];
    
  for(Int_t i=0; i < 4; i++){

    TString hnamel  = Form("hsl[%d]",i);
    TString hnamelc  = Form("hslc[%d]",i);
    TString hnamec = kmult[i].GetTitle();
    hsl[i]  = new TH1D(hnamel,"Left:"+hnamec,100,-250,250);
    hslc[i] = new TH1D(hnamelc,hnamec,100,-250,250);
    

    TCut gtL = ggt&&InKyotoL&&kmult[i];    
    c1->cd(i+1);    
    tpc->Draw("dzKyotoL>>"+hnamel ,gtL&&"fKyotoLz>0");
    tpc->Draw("dzKyotoL>>"+hnamelc,gtL&&FindKyotoL&&"fKyotoLz>0","same");
    c1->Update();


     kmult[i].Print() ;
     cout << "L Found in Kyoto / Track " << hslc[i]->GetEntries() << " / " << hsl[i]->GetEntries() ;
     if(hsl[i]->GetEntries()>0)
       cout << " = " << hslc[i]->GetEntries()/hsl[i]->GetEntries() << endl;

     Double_t ymax = hsl[i]->GetYaxis()->GetXmax();
     TString  ratio= Form("%f10.5",hslc[i]->GetEntries()/hsl[i]->GetEntries());
     TLatex   trate(100,ymax*0.8,ratio);
     trate.Draw();





    TString hnamer  = Form("hsr[%d]",i);
    TString hnamerc  = Form("hsrc[%d]",i);
    hsr[i]  = new TH1D(hnamer,"Right:"+hnamec,100,-250,250);
    hsrc[i] = new TH1D(hnamerc,hnamec,100,-250,250);

    TCut gtR = ggt&&InKyotoR&&kmult[i];
    c2->cd(i+1);
    tpc->Draw("dzKyotoR>>"+hnamer ,gtR&&"fKyotoRz>0");
    tpc->Draw("dzKyotoR>>"+hnamerc,gtR&&FindKyotoR&&"fKyotoRz>0","same");
    c2->Update();

    cout << "R Found in Kyoto / Track " << hsrc[i]->GetEntries() << " / " << hsr[i]->GetEntries() ;
    if(hsr[i]->GetEntries() > 0)
      cout << " = " << hsrc[i]->GetEntries()/hsr[i]->GetEntries() << endl;

  }
}
