{
  auto fout = new TFile("gcut132Sn.root","recreate");

  TCanvas *cvs2 = new TCanvas("cvspid2", "", 0, 0, 800, 580);
  TF2 *sigmaLine = new TF2("sigmaLine", "xygaus");
  sigmaLine -> SetFillStyle(1000);
  sigmaLine -> SetLineWidth(2);
  sigmaLine -> SetRange(2.635, 49, 2.647, 51);
  sigmaLine -> SetParameters(0.253831, 2.64, 0.00127952, 49.9701, 0.204459, -0.0465522);
  sigmaLine -> SetNpx(1000);
  sigmaLine -> SetNpy(1000);

  double contours[3];
  double corr = sigmaLine->Eval(2.64, 49.9701)/ROOT::Math::bigaussian_pdf(0,0,1,1,0);
  contours[0] = corr*ROOT::Math::bigaussian_pdf(2,2,1,1,0); //0.05;                                                                         
  contours[1] = corr*ROOT::Math::bigaussian_pdf(1.5,1.5,1,1,0);
  contours[2] = corr*ROOT::Math::bigaussian_pdf(1,1,1,1,0);
  sigmaLine -> SetContour(3,contours);

  sigmaLine -> Draw("cont list");
  cvs2 -> Update();

  TObjArray *myContours = (TObjArray *) gROOT -> GetListOfSpecials() -> FindObject("contours");
  auto graphSigma20 = (TGraph *) ((TList *) myContours -> At(0)) -> First();
  auto graphSigma15 = (TGraph *) ((TList *) myContours -> At(1)) -> First();
  auto graphSigma10 = (TGraph *) ((TList *) myContours -> At(2)) -> First();

  auto cutSigma20 = new TCutG("sigma20",graphSigma20 -> GetN());
  auto cutSigma15 = new TCutG("sigma15",graphSigma15 -> GetN());
  auto cutSigma10 = new TCutG("sigma10",graphSigma10 -> GetN());
  Double_t x = 0, y = 0;
  for (Int_t iPoint = 0; iPoint < graphSigma20 -> GetN(); iPoint++) {
    graphSigma20 -> GetPoint(iPoint, x, y);
    cutSigma20 -> SetPoint(iPoint, x, y);
  }
  for (Int_t iPoint = 0; iPoint < graphSigma15 -> GetN(); iPoint++) {
    graphSigma15 -> GetPoint(iPoint, x, y);
    cutSigma15 -> SetPoint(iPoint, x, y);
  }
  for (Int_t iPoint = 0; iPoint < graphSigma10 -> GetN(); iPoint++) {
    graphSigma10 -> GetPoint(iPoint, x, y);
    cutSigma10 -> SetPoint(iPoint, x, y);
  }
  cutSigma20 -> SetVarX("aoq");
  cutSigma20 -> SetVarY("z");
  cutSigma15 -> SetVarX("aoq");
  cutSigma15 -> SetVarY("z");
  cutSigma10 -> SetVarX("aoq");
  cutSigma10 -> SetVarY("z");

  cutSigma20->Write();
  cutSigma15->Write();
  cutSigma10->Write();

  fout->Close();


}
