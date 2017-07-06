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
#include "TROOT.h"

class STFlowCorrection : public TObject {

 public:
  STFlowCorrection(){};
  STFlowCorrection( TChain *chele, UInt_t ival1, UInt_t ival2) {Initialize(chele, ival1, ival2);}
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
  TChain    *ChEle = NULL;
  vector<Double_t> vphi;
  vector<Double_t> bphi;


  Double_t   binmax[2];
  Double_t   binmin[2];
  TString    binpara[2];

  vector<Double_t> vtheta;
  vector<Int_t>    vmtrack;

 public:
  void   Initialize(TChain *chele=NULL, UInt_t ival1=4, UInt_t ival2=0);
  void   Initialize(UInt_t ival1=4, UInt_t ival2=0);
  void   SetHarmonics(UInt_t ival){charm = ival;}
  void   SetRealOrMix(UInt_t ival){irm  = ival;}


  void   Add(Double_t val) {vphi.push_back(val);}
  void   Add(Double_t val1, Double_t val2) {vphi.push_back(val1); vtheta.push_back(val2);}
  void   Add(Int_t ival, Double_t val1, Double_t val2) {
    vmtrack.push_back(ival);
    vphi.push_back(val1); 
    vtheta.push_back(val2);  }

  void   clear();


  vector<Double_t> GetOriginalPhi() {return bphi;}             
  vector<Double_t> GetCorrectedPhi(){return vphi;}             
  vector<Double_t> GetTheta()       {return vtheta;}
  vector<Int_t>    GetMTrack()      {return vmtrack;}
  Double_t         GetMTrackMean(){return TMath::Mean(vmtrack.begin(), vmtrack.end());}
  Double_t         GetThetaMean() {return TMath::Mean(vtheta.begin(),  vtheta.end());}
  

  void   FourierCorrection(vector<Double_t> &val);
  UInt_t FourierCorrection();
  UInt_t GetCorrectionFactor(UInt_t val=0);
  UInt_t SaveCorrectionFactor(TString comm=":");
  void   PrintContents();
  void   PrintRange();

  UInt_t   GetNumberOfParam() {return (UInt_t)vphi.size();};
  UInt_t   SetHarmonics()     {return harm;}


  void      GetCorrection(vector<Double_t> &val);
  Double_t  GetCorrection(Double_t val);

  Double_t *GetAverageCosin(Int_t ival, vector<Double_t> &val);
  
  Int_t   GetNHarmonics() {return harm;}
  void    SetFileName(TString sval);
  TString GetFileName()   {return fname;}

  void     SetBin_max(UInt_t idx=0, Double_t val=999.) {if( idx < 3) binmax[idx] = val;}
  void     SetBin_min(UInt_t idx=0, Double_t val=0.) {if( idx < 3) binmin[idx] = val;}
  Double_t GetBin_max(UInt_t idx=0)  {if(idx < 3) return binmax[idx]; else return -1.;}
  Double_t GetBin_min(UInt_t idx=0)  {if(idx < 3) return binmin[idx]; else return -1.;}
  TString  GetBinParameter(UInt_t idx=0) {return binpara[idx];}

private:
  void   SetFileName();
  void   Init();
  void   ShowParameters();

  ClassDef(STFlowCorrection,0);
};


#ifdef __MAKECINT__
#pragma link off all   globals;
#pragma link c++ class STFlowCorrection+;
#endif

#endif
