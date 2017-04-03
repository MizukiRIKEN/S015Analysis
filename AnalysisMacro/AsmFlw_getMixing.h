#ifndef  ASMPLW_GETAZMCOLL
#define  ASMPLW_GETAZMCOLL

TClonesArray *aParticleArray = NULL;
TClonesArray *mixParticleArray = NULL;
Int_t         ntrack[7];

void Open();
void Initialize();
void OutputTree(Int_t val);
Int_t GetRandomNumTrack();
Int_t GetMultiplicityDistribution();
STParticle* GetMixedTrack(Int_t *ival);


TString sRun;
Bool_t  bMix;  // kTRUE mixing kFALSE real data
TString sVer;
 
TChain *fChain;
Int_t   nEntry;

TFile *fout;
TTree *mflw = NULL;



vector<TVector3> vPart;

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
vector<Double_t> iphi;
vector<Double_t> rapid;
vector<Double_t> etot;
TVector3 unitP;
TVector3 unitP_b;
TVector3 unitP_t;
TVector3 unitP_lang;
Int_t    kymult;
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

TFile *mhfile;

#endif
