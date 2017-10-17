TChain *LChain;
static UInt_t id = 0;
 
TString printHeader="";
TString aRun;
TString sVer;

void OpenChain();

void openASM(TString srun="0", TString sver="0")
{
  gROOT->Reset();


  if(srun == "0")
    aRun = gSystem -> Getenv("RUN");
  else
    aRun = srun;

  if(sver == "0")
    sVer = gSystem -> Getenv("VER");
  else
    sVer = sver;


  if( aRun != "" && sVer != "" )
    OpenChain();

}

void OpenChain()
{

  TString ssVer = "";
  TString tVer  = sVer+".";


  if( aRun=="" || sVer=="" || aRun.Length() == 4) {
    cout << "Plase type " << endl;
    cout << "RUN={####} VER=#.# root openAll.C" << endl;
    exit(0);
  }


  Int_t nrun = (aRun.Length()-1)/5;
  cout << aRun << "-> nrun " << nrun << endl;;

  printHeader = "FlwRUN"+aRun(1,4)+ Form("m%d",nrun) + "_v" + sVer; 
  TString printName ;
  vector<Int_t> lrun;
  Int_t ist = 1;
  while(ist < aRun.Length() - 4) {
    TString prun = aRun(ist,4);
    lrun.push_back( atoi(prun) );

    ist+=5;
  }


  for(Int_t i = 0; i < nrun; i++)
    cout << " lrun = " << lrun.at(i) << endl;

  
  TString treename;
  TString fform;

  treename = "flw";
  fform = "_flw_v"+sVer+".root";
 
  LChain = new TChain(treename);


  for(Int_t i = 0; i < (Int_t)lrun.size(); i++){
    TString rootdir = "../data/"; 

    TString fname = Form("run%d",lrun.at(i))+fform;

    if(gSystem->FindFile(rootdir,fname)) {
      LChain->Add(fname);
      std::cout << fname << " was added " << std::endl;
    }
    else
      std::cout << " File is not found ->" << fname << std::endl;

  }

  if(LChain->GetListOfFiles()->GetEntries() == 0){
    std::cout << " No files are loaded. " << endl;
    exit(0);
  }
  else{

    std::cout << "Runs are linked to rChain" << std::endl;
    LChain->SetName("rChain");
    cout << LChain << endl;

  }

}


void SaveCanvas(TString fopt = "")
{
  Int_t iCanvas = gROOT->GetListOfCanvases()->GetEntries();  
  for(Int_t i = 0; i < iCanvas; i++)
    gROOT->GetListOfCanvases()->At(i)->SaveAs(printHeader+fopt+Form("_%d",i)+".png");
}


