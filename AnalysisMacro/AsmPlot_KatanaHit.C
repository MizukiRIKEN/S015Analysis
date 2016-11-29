{
  Int_t iRun = 2900;
  //  TFile *_file0 = TFile::Open(Form("../data/run%d_asm3.v1.03.root",iRun));
  TFile *_file0 = TFile::Open(Form("../data/run%d_asm4.v1.03.d55x25c.root",iRun));
  TString pRun = Form("%d",iRun);

  gROOT->Macro("AsmPlot_TCut.C");

  gStyle->SetOptFit(1111);


  //--------------------start
  //
  Int_t nEntry = tpc->GetEntries();


  auto c1 = new TCanvas("c1","c1",1000,45,400,1147);
  c1->SetTopMargin(0.035f);
  c1->SetBottomMargin(0.054f);
  c1->Draw();

  auto c3 = new TCanvas("c3","c3",100,100,800,600);
  c3->Draw();

  auto c2 = new TCanvas("c2","c2",0,500,800,600);
  c2->Divide(1,2);
  c2->Draw();


  TString h1name = "Katana hit pattern";
  TString h2name = "Extrapolated hit ";
  auto h1 = new TH1D("h1",h1name,28,-1050,350);   
  auto h1a= new TH1D("h1a",h1name,28,-1050,350);   
  auto h1b= new TH1D("h1b",h1name,28,-1050,350);   
  auto h2 = new TH1D("h2",h2name,150,-1050,350);
  auto h2a= new TH1D("h2a",h2name,150,-1050,350);
  auto h3 = new TH2D("h3","dx on KATANA ",26,-1050,350,100,-200,200);
  auto h4 = new TH2D("h4","track X-Y on KATANA within one paddle",150,-1050,350,100,-400,0);
  auto h5 = new TH2D("h5","track X-Y on KATANA",150,-1050,350,100,-400,0);

  auto n1 = new TH2D("n1","Number of hits in total;# of track; Katana Mult",25,0,25,20,0,25);
  auto n2 = new TH2D("n2","Number of hits event by event",25,0,25,20,0,25);

  auto m1  = new TH2D("m1" ,"KATANA hit pattern",100,1200.,2000,28,-1050,350);
  auto m3  = new TH2D("m3" ,"Track hit pattern ",100,1200.,2000,28,-1050,350);
  auto m3a = new TH2D("m3a" ,"Track hit pattern(associated)",100,1200.,2000,28,-1050,350);
  auto m2  = new TH2D("m2" ,"Track hit pattern (associated)",100,-400,0,150,-1050,350);
  auto m2a = new TH2D("m2a","Track hit pattern",100,-400,0,150,-1050,350);

  tpc->Draw("KatanaMult:trknKATANA>>n1","","box");  


  Int_t ievt = 0;
  while(ievt < nEntry){
    c1->cd();
    m1->SetMarkerStyle(8);
    m1->SetMarkerColor(7);
    tpc->Draw("KatanaPadX:1897>>m1","","",1,ievt);
    
 
    m3->SetMarkerStyle(21);
    m3->SetMarkerColor(3);
    m3a->SetMarkerStyle(21);
    m3a->SetMarkerColor(2);
    tpc->Draw("hitKATANAX:1800>>m3","fndKATANA","same",1,ievt);
    tpc->Draw("hitKATANAX:1800>>m3a","fndKATANA==0","same",1,ievt);
    c1->Update();


    // -- c3 --
    
    c3->cd();
    n1->Draw("box");
    n2->SetMarkerStyle(kFullCircle);
    n2->SetMarkerColor(4);
    tpc->Draw("KatanaMult:trknKATANA>>n2","","same",1,ievt);  

    c3->Update();
    
  

    //-- c2 --
    c2->cd(1);
    h1name = Form("KATANA pad X %d",ievt);
    tpc->Draw("KatanaPadX>>h1","","",1,ievt);
 
    sLavel = Form("KATNAA Hit : Event %d",ievt);
    h1->SetTitle(sLavel);
    h1->Draw();

    h2name = Form("Track_X hits on KATANA %d",ievt);
    tpc->Draw("hitKATANAX>>h2","fndKATANA==1","",1,ievt);
    tpc->Draw("hitKATANAX>>h2a","fndKATANA==0","",1,ievt);

    h1->SetFillStyle(1001);
    h1->SetFillColor(7);
    h1->SetLineColor(4);
    h1->Draw();

    h2->SetLineColor(3);
    h2->Draw("same");
    h2a->SetLineColor(2);
    h2a->Draw("same");

    c2->cd(2);
    h4->SetMarkerStyle(kFullCircle);    
    h4->SetMarkerColor(3);
    tpc->Draw("hitKATANAY:hitKATANAX>>h4","fndKATANA","",1,ievt);
    h5->SetMarkerStyle(kFullCircle);    
    h5->SetMarkerColor(2);
    tpc->Draw("hitKATANAY:hitKATANAX>>h5","fndKATANA==0","same",1,ievt);

    c2->Update();
    
 
    TString pdfout = Form("KHit_Run%d_evt",iRun);
    TString pdfout2 = Form("%d",ievt); 
    //    c2->SaveAs(pdfout+pdfout2+".png");

    cout << " Event ID " << ievt << endl;

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

    //    sleep(5);
  }
}
