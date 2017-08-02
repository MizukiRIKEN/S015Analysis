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
Double_t rPlanerPhi;
Int_t    numGoodTrack;
TVector3 unitP;
TVector3 runitP;
TVector2 unitPt;
TVector2 runitPt;

TVector3 unitP_lang;
TVector3 runitP_lang;

Int_t    ngtrack;
vector<Double_t> deltphi;
vector<Double_t> iphi;

vector<TVector3> vPart;
vector<Double_t> px;
vector<Double_t> py;
vector<Double_t> pz;




#endif
