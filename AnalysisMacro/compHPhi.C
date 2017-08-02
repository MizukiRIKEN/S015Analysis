
void Copy(TString sfname)
{

  TFile pp(sfname+".root","recreate");

  gROOT->cd();
  TH1D *haiphi0 = (TH1D*)gROOT->FindObject("haiphi0");
  TH1D *haiphi1 = (TH1D*)gROOT->FindObject("haiphi1");
  haiphi0->SetDirectory(&pp);
  haiphi1->SetDirectory(&pp);

  pp.cd();
  pp.Write();
  pp.Close();

}

void RawiPhi()
{  
  TFile *_file0 = TFile::Open("run2900_v2.0.10.root");
  TFile *_file1 = TFile::Open("run3032_v2.0.11.root");

  TString hobj1 = "haiphi0";
  TString hobj2 = "haiphi1";


  TCanvas *cc1 = new TCanvas("cc1","cc1",700,500);
  cc1->Divide(2,2);
  
  UInt_t i = 1;
  _file0->cd();

  cc1->cd(i); i++;
  TH1D *haiphi0_29 = (TH1D*)gROOT->FindObject(hobj1);
  haiphi0_29->SetTitle("Prod. REAL ; #phi_{i} ");
  haiphi0_29->Draw();

  cc1->cd(i); i++;
  TH1D *haiphi1_29 = (TH1D*)gROOT->FindObject(hobj2);
  haiphi1_29->SetTitle("Prod. MIXed ; #phi_{i} ");
  haiphi1_29->Draw();

  _file1->cd();
  cc1->cd(i); i++;
  TH1D *haiphi0_32 = (TH1D*)gROOT->FindObject(hobj1);
  haiphi0_32->SetTitle("Min. REAL ; #phi_{i} ");
  haiphi0_32->Draw();

  cc1->cd(i); 
  TH1D *haiphi1_32 = (TH1D*)gROOT->FindObject(hobj2);
  haiphi1_32->SetTitle("Min. MIXed ; #phi_{i} ");
  haiphi1_32->Draw();

  

}

void Comp(UInt_t iv1 = 0)
{  
  TFile *_file0 = TFile::Open("run2900_v2.0.10.root");
  TFile *_file1 = TFile::Open("run3032_v2.0.11.root");

  TString hobj1 = "haiphi0";
  TString hobj2 = "haiphi1";

  TString ytitle;

  _file0->cd();
  ytitle = "Prod. Real / ";

  if(iv1 == 1){
    _file1->cd();
    ytitle = "Min. Real / ";
  }

  if(iv1 == 4){
    hobj1 = "haiphi1";
    ytitle = "Prod. MIXed / ";
  }

  TH1D *haiphi0_29 = (TH1D*)gROOT->FindObject(hobj1);
  Int_t e29 = haiphi0_29->GetEntries();
  haiphi0_29->SetDirectory(gROOT);

  if( iv1 == 0) {
    hobj2 = "haiphi1";
    ytitle += "Prod. MIXed";
  }

  if( iv1 == 2) {
    _file1->cd();
    hobj2 = "haiphi0";
    ytitle += "Min. Real";
  }

  if( iv1 == 1 || iv1 == 3 || iv1 == 4) {
    _file1->cd();
    hobj2 = "haiphi1";
    ytitle += "Min. MIXed";
  }    


  TH1D *haiphi0_32 = (TH1D*)gROOT->FindObject(hobj2);
  Int_t e32 = haiphi0_32->GetEntries();
  haiphi0_32->SetDirectory(gROOT);


  haiphi0_32->Scale(1./(Double_t)e32);
  haiphi0_29->Scale(1./(Double_t)e29);

  //  TH1D *nhaiphi0d = new TH1D( (*haiphi0_32)/(*haiphi0_29) );
  TH1D *nhaiphi0d = new TH1D( (*haiphi0_29)/(*haiphi0_32) );

  nhaiphi0d->SetMaximum(1.5);
  nhaiphi0d->SetMinimum(0.5);
  nhaiphi0d->Draw();
  nhaiphi0d->SetTitle("; #phi_{i}; "+ytitle);

  cout << ytitle << endl;
  cout << " Norm : " << e29 << " / " << e32 << endl;
}


void compHPhi()
{
  TCanvas *cc0 = new TCanvas("cc0","cc0",1000,700);
  cc0->Divide(2,3);
  for(UInt_t i = 0; i < 5; i++){
    cout << i << " >> " << endl;
    cc0->cd(i+1); 
    Comp(i);
  }


  RawiPhi();
}
