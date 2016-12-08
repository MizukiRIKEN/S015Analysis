{
  TFile *_file0 = TFile::Open("../data/run2900_flw3t.p1.root");
  flwp1->Draw("runitPt.Y():runitPt.X()>>hp(100,-10,10,100,-10,10)","fgoodtrack","colz");
  hp->ProjectionX();
  hp_px->Fit("gaus");
}
