void load_PIDcut()
{
  auto gcutFile1 = new TFile("gcutPID132Sn.root");
  auto gProton   = (TCutG*)gcutFile1->Get("gcutProton132Sn2");
  auto gDeuteron = (TCutG*)gcutFile1->Get("gcutDeutron132Sn");
  auto gTriton   = (TCutG*)gcutFile1->Get("gcutTriton132Sn");
  auto gPip      = (TCutG*)gcutFile1->Get("gcutPip132Sn");
  auto gPim      = (TCutG*)gcutFile1->Get("gcutPim132Sn");


  gcutFile1->Close();

}
