{
  auto inff = new TFile("rprotonv1v2.root");

  auto pgv_v1 = (TGraphErrors*)inff->Get("gv_v1");
  auto pgv_v2 = (TGraphErrors*)inff->Get("gv_v2");

  inff->Close();

  auto inff2 = new TFile("rdeuteronv1v2.root");
  auto dgv_v1 = (TGraphErrors*)inff2->Get("gv_v1");
  auto dgv_v2 = (TGraphErrors*)inff2->Get("gv_v2");

  inff2->Close();


  auto aLeg = new TLegend(0.1,0.8,0.3,0.9);
  aLeg->AddEntry(pgv_v1,"Proton","ip");
  aLeg->AddEntry(dgv_v1,"Deuteron","ip");

  auto cc0 = new TCanvas("cc0","cc0",700,500);
  pgv_v1->SetLineColor(4);
  pgv_v1->SetMarkerStyle(20);
  pgv_v1->SetMarkerColor(4);

  dgv_v1->SetLineColor(2);
  dgv_v1->SetMarkerStyle(20);
  dgv_v1->SetMarkerColor(2);


  auto mg_v1 = new TMultiGraph();
  mg_v1->SetTitle(";Ycm/Ycm_beam; v1(a.u.)"); 

  mg_v1->Add(pgv_v1,"lp");
  mg_v1->Add(dgv_v1,"lp");
  mg_v1->Draw("a");
  aLeg->Draw();


  //  pgv_v1->Draw("ALP");
  //  dgv_v1->Draw("ALP");




  auto cc1 = new TCanvas("cc1","cc1",700,500);
  pgv_v2->SetLineColor(4);
  pgv_v2->SetMarkerStyle(20);
  pgv_v2->SetMarkerColor(4);

  dgv_v2->SetLineColor(2);
  dgv_v2->SetMarkerStyle(20);
  dgv_v2->SetMarkerColor(2);


  auto mg_v2 = new TMultiGraph();
  mg_v2->SetTitle(";Ycm/Ycm_beam; v2(a.u.)"); 
  mg_v2->SetMaximum(0.002)
  mg_v2->Add(pgv_v2,"lp");
  mg_v2->Add(dgv_v2,"lp");
  mg_v2->Draw("a");
  aLeg->Draw();

  //  pgv_v2->Draw("ALP");



}
