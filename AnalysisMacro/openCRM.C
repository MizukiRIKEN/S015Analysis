TChain *LChain[4];
static UInt_t id = 0;
 
TString printHeader="";
TString aRun;
TString sVer;
TString sDBase;
TString cVer;

void OpenChain(UInt_t m = 0);

void openCRM(UInt_t m=0, TString srun="0", TString sver="0", TString dbase="0", TString cver="0")
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
  
  if(dbase == "0")
    sDBase = gSystem -> Getenv("DBASE");
  else
    sDBase = dbase;

  if(cver == "0")
    cVer  = gSystem -> Getenv("CVER");


  if( aRun != "" && sVer != "" && cVer != "")
    OpenChain(m);
  else
    cout << " Define Environment RUN=, VER=, DBASE=, CVER " << endl;

  
}

void OpenChain(UInt_t m)
{
  if( m > 4) exit(0);
  
  TString ssVer = "";
  TString tVer  = sVer+".";

  for(Int_t i = 0; i < 3; i++){
    Ssiz_t idx = tVer.Index(".");
    ssVer += tVer(0 , (Int_t)idx);
    //    tVer.Replace(0,(Int_t)(idx+1),"");
  }

  auto fVer = atoi(ssVer);

  cout << "fVer " << fVer << " ssVer " << ssVer << " tVer " << tVer << endl;

  if( aRun=="" || sVer=="" || aRun.Length() == 4) {
    cout << "Plase type " << endl;
    cout << "RUN={####} VER=#.#.# DBASE=_crdb2913v2.2.6 root openzvRM.C" << endl;
    exit(0);
  }


  Int_t nrun = (aRun.Length()-1)/5;
  cout << aRun << "-> nrun " << nrun << endl;;

  printHeader = "FlwRUN"+aRun(1,4)+ Form("m%d",nrun) + "_v" + sVer + "_cv" + cVer; 
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

  
  TString treename = "cflw";
  TString fform;
  if( m == 1 ) {
    fform = "_mxflw_v"+sVer+sDBase+"_cv"+cVer+".root";
  }
  else {
    fform = "_rdflw_v"+sVer+sDBase+"_cv"+cVer+".root";
  }

 
  LChain[m] = new TChain(treename);
 //  LChain = new TChain(treename);

  for(Int_t i = 0; i < (Int_t)lrun.size(); i++){
    TString rootdir = "../data"; 

    TString fname = Form("run%d",lrun.at(i))+fform;
    cout << fname << endl;

    if(gSystem->FindFile(rootdir,fname))
      LChain[m]->Add(fname);
    else
      std::cout << " File is not found " << fname << std::endl;
  }
  

  if(LChain[m]->GetListOfFiles()->GetEntries() == 0){
    std::cout << " No files are loaded. " << endl;    
    exit(0);
  }


  if( LChain[m]!=NULL ){
    std::cout << "Runs are linked to rChain"<< m  << std::endl;
    LChain[m]->SetName(Form("rChain%d",m));
    cout << LChain[m] << endl;
  }

}


void SaveCanvas(TString fopt = "")
{
  Int_t iCanvas = gROOT->GetListOfCanvases()->GetEntries();  
  for(Int_t i = 0; i < iCanvas; i++)
    gROOT->GetListOfCanvases()->At(i)->SaveAs(printHeader+fopt+Form("_%d",i)+".png");
}


