#ifndef  ASMPLW_GETMIXING
#define  ASMPLW_GETMIXING

#include "TVector3.h"
#include "TVector2.h"

// Reading tree
TClonesArray *aParticleArray = NULL;
TClonesArray *mixParticleArray = NULL;
Int_t         ntrack[7];
Int_t         kymult;
Double_t      ProjA;
Double_t      ProjB;
Double_t      aoq;
Double_t      z;


void   Open();
void   Initialize(Int_t val);
void   OutputTree(Int_t val);
Int_t  GetRandomNumTrack();
Int_t  GetMultiplicityDistribution();
Bool_t DefineVersion();
STParticle* GetMixedTrack(Int_t *ival);
void   LoadPIDFile();
Int_t  GetPID(Double_t valx, Double_t valy);


Int_t  iVer[3];

TString sRun;
Bool_t  bMix;  // kTRUE mixing kFALSE real data
TString sVer;
TString sAsm;
Bool_t  BeamAngle;
Int_t   iAsm;
 
TChain *fChain;
Int_t   nEntry;

TRandom3 rnd;
vector<UInt_t> trackID;

TFile *fout;
TTree *mflw = NULL;
TCutG *gProton = NULL;


vector<TVector3> vPart;
vector<TVector2> pt;

// Tree out
Int_t   iRun;
Int_t    numGoodTrack;
Int_t    mtrack;
Int_t    mtrack_t;
Int_t    mtrack_b;
vector<Int_t>    event;
vector<Int_t>    pid;
vector<Double_t> px;
vector<Double_t> py;
vector<Double_t> pz;
vector<Double_t> mom;
vector<Double_t> dedx;
vector<Double_t> deltphi;
vector<Double_t> deltphi_t;
vector<Double_t> deltphi_b;
vector<Double_t> rpphi;
vector<Double_t> iphi;
vector<Double_t> rapid;
vector<Double_t> etot;
vector<Double_t> prapid;
vector<Double_t> rpxt;
vector<Double_t> rpxb;
vector<Int_t>    wgt;
TVector3 unitP;
TVector2 unitP_b;
TVector2 unitP_t;
TVector2 unitP_lang;
TVector2 unitP_1;
TVector2 unitP_2;
UInt_t   mtrack_1;
UInt_t   mtrack_2;


vector<Double_t>  theta_xz;
vector<Double_t>  theta_yz;

Double_t         aX;
Double_t         bY;

TClonesArray     *p_org = NULL;


// Tree out end


TRandom2 pran;
TH1I *histGT_r = NULL;
TH1I *histGT;
TH1I *histMixEvt;
TH1I *histMixTrack;
TH1D *hRPrapd;
TH1D *hRPrapd_t;
TH1D *hRPrapd_b;
TH1I *hm_t;
TH1I *hm_b;
TH1I *hgtc;

TH2D *hRPpxsr_t;
TH2D *hRPpxsr_b;


TFile *mhfile;

#endif
