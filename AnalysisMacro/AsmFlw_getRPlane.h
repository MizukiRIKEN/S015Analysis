#ifndef  ASMPLW_GETRPLANE
#define  ASMPLW_GETRPLANE

TClonesArray *aParticleArray = NULL;

void Open();
void Initialize(Int_t val);
void OutputTree(Int_t val);


Int_t   iRun;
TString sRun;

TChain *fChain;

TFile *fout;
TTree *flwp1 = NULL;


Double_t rPlanePhi;
Int_t    numGoodTrack;
TVector2 unitP;
#endif
