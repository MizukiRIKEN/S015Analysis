{
  //  auto gcutFile = new TFile("gcut132Sn.root");
  auto gcutFile = new TFile("genieCuts.root");
  auto g132Sn=(TCutG*)gcutFile->Get("sigma20");
  gcutFile->Close();
}
