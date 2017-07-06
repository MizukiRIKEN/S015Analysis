#ifndef  ASMPLW_GETFLATTEN2D
#define  ASMPLW_GETFLATTEN2D

#include "TVector3.h"
#include "TVector2.h"

// Reading tree
TClonesArray *mixParticleArray = NULL;
TClonesArray *flowcorrArray    = NULL;
Int_t         ntrack[7];
Int_t         kymult;
Double_t      ProjA;
Double_t      ProjB;
Double_t      aoq;
Double_t      z;

void      SetEnvironment();
void      PrintProcess(Int_t ievt);
void      SetNumberOfProcess(Int_t nmax);
void      Open();
void      Initialize();
void      OutputTree(Long64_t val);
Long64_t  GetRandomNumTrack();
Long64_t  GetMultiplicityDistribution();
Bool_t    DefineVersion();

STParticle* GetRealTrack(Long64_t ival);
STParticle* GetMixedTrack(Long64_t *ival, Int_t *kval);

void      RotateAsBeamAngle(STParticle *apart, TVector3 *p1, TVector2 *pt);
void      SetPtWeight(STParticle *apart, Double_t *rpd, Double_t *psudr);
void      FlatteningCorrection(TVector3 *p1, Int_t ival);
void      SubEventAnalysis();
void      AzmAngleRPTReactionPlane();

void      LoadPIDFile();
Int_t     GetPID(Double_t valx, Double_t valy);
UInt_t    SetDatabaseFiles();
Int_t     GetThetaCorretionIndex(Double_t fval, Int_t ival);
Int_t     GetMultiplicityCorretionIndex(UInt_t ival);
void      CheckPlot(UInt_t ival = 0);

Int_t   iVer[3];
TString sRun;
TString sMix;
TString sRot;
Bool_t  bMix;  // kTRUE mixing kFALSE real data
TString sVer;
TString sAsm;
Bool_t  BeamAngle;
Int_t   iAsm;
TString sbRun;
TString sbVer;
TString sBinp;
UInt_t  nBin; 
TString binpara;

Int_t  maxProc;;

//TChain *fChain;
TTree  *fTree;
Long64_t nEntry;

TRandom3 rnd;
vector<UInt_t> trackID;

TFile *fout;
TTree *mflw = NULL;
TCutG *gProton = NULL;

vector<TVector2> pt;

TClonesArray *aParticleArray = NULL;
TClonesArray *aflowcorrArray = NULL;
STFlowCorrection *flowcorr = NULL;

vector<TString> vfname;
vector< vector<Double_t> >  binmax[2];
vector< vector<Double_t> >  binmin[2];

vector< pair<Double_t, Double_t> > pbinmin;
vector< pair<UInt_t  , Double_t> > binmatrix;
vector< pair<UInt_t  , Double_t> >::iterator itbinmatrix;
vector <vector< pair<UInt_t, Double_t> >::iterator > ppbinmin;

vector<UInt_t> mtkbin;

UInt_t  binmapsize = 0;

// Tree out
Int_t   iRun;
Int_t    numGoodTrack;
Int_t    mtrack;
vector<Int_t>    event;
vector<Int_t>    pid;
vector<Double_t> px;
vector<Double_t> py;
vector<Double_t> pz;
vector<Double_t> mom;
vector<Double_t> dedx;
vector<Double_t> deltphi;
vector<Double_t> rpphi;
vector<Double_t> itheta;
vector<Double_t> iphi;
vector<Double_t> rapid;
vector<Double_t> etot;
vector<Double_t> prapid;
vector<Double_t> rpxt;
vector<Double_t> rpxb;
vector<Int_t>    wgt;
vector<UInt_t>   mxntrk;
TVector3 unitP;
TVector2 unitP_lang;
TVector2 unitP_1;
TVector2 unitP_2;
UInt_t   mtrack_1;
UInt_t   mtrack_2;

vector<Double_t>  theta_xz;
vector<Double_t>  theta_yz;

Double_t         aX;
Double_t         bY;

TClonesArray     *p_rot = NULL;
TClonesArray     *p_org = NULL;

// Tree out end


TRandom2 pran;
TH1I *histGT_r = NULL;
TH1I *histGT;
TH1I *histMixEvt;
TH1I *histMixTrack;
TH1D *hRPrapd;
TH1I *hm_t;
TH1I *hm_b;
TH1I *hgtc;
TH1D *hvphi;
TH1D *hvthet;
TH1I *hvmtk;

TFile *mhfile;


// Important parameters.
UInt_t ntr_diff = 5;  //ver. 2.0.14 
//UInt_t ntr_diff = 3;  // ver. 2.0.15


#endif
