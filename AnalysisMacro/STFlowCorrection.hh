#ifndef STFLOWORRECTION_HH
#define STFLOWORRECTION_HH


#include <fstream>
#include <iostream>
#include <TMath.h>
#include "TString.h"
#include "TCollection.h"
#include "TChainElement.h"
#include "TFile.h"
#include "TChain.h"

class STFlowCorrection : public TObject {

 public:
  STFlowCorrection(){};
  STFlowCorrection( TObjArray *chele, UInt_t ival1, UInt_t ival2) {Initialize(chele, ival1, ival2);}
  STFlowCorrection(UInt_t ival1, UInt_t ival2)                    {Initialize(ival1, ival2);}
  ~STFlowCorrection(){};


 private :
  UInt_t     irm; // 1 = mix, otherwise real
  UInt_t     harm;
  UInt_t     charm;
  UInt_t    *indx;
  Double_t  *An;
  Double_t  *Bn;
  Double_t  *An_rms;
  Double_t  *Bn_rms;
  TString    fname;
  TObjArray *ChEle = NULL;
  vector<Double_t> vphi;
  Double_t   pzmax;
  
 public:
  void   Initialize(TObjArray *chele=NULL, UInt_t ival1=4, UInt_t ival2=0);
  void   Initialize(UInt_t ival1=4, UInt_t ival2=0);
  void   SetHarmonics(UInt_t ival){charm = ival;}
  void   SetRealOrMix(UInt_t ival){irm  = ival;}


  void   Add(Double_t val) {vphi.push_back(val);}
  void   FourierCorrection(vector<Double_t> &val);
  vector<Double_t>  FourierCorrection();
  UInt_t GetCorrectionFactor(TString comm=":");
  UInt_t SaveCorrectionFactor(TString comm=":");

  UInt_t   GetNumberOfParam() {return (UInt_t)vphi.size();};
  UInt_t   SetHarmonics()     {return harm;}


  void      GetCorrection(vector<Double_t> &val);
  Double_t  GetCorrection(Double_t val);

  Double_t *GetAverageCosin(Int_t ival, vector<Double_t> &val);
  
  Int_t  GetNHarmonics() {return harm;}
  void   SetFileName(TString sval);

  Double_t GetPz_max()  {return pzmax;}
private:
  void   SetFileName();
  void   Init();

  ClassDef(STFlowCorrection,0);
};


#ifdef __MAKECINT__
#pragma link off all   globals;
#pragma link c++ class STFlowCorrection+;
#endif

#endif
