/**     
 * @brief STTrack Class         
 *                                   
 * @author Mizuki         
 */

#ifndef STPARTICLE
#define STPARTICLE
#include "TCutG.h"
#include "TFile.h"

#include "TVector3.h"
#include "TRotation.h"

#include "STTrackCandidate.hh"
#include "STTrack.hh"

#include <vector>

class STParticle : public STTrack {
public:
  STParticle();
  STParticle(STTrack *atrack);
  virtual ~STParticle(){};

  virtual void Clear(Option_t *option = "");

  //private:
  void     SetProperty();
  void     SetLinearPID();

  void     CheckTrackonTarget();
  void     CheckKATANAHit();
  void     CheckKYOTOHit();


public:
  void     SetTrack(STTrack *atrack);
  void     RotateAlongBeamDirection(Double_t valuex, Double_t valuey);
  void     SetP(Double_t value)          {fP = value;}
  void     SetPiPID();
  Int_t    GetPiPID()                    {return fpipid;}

  void     SetPID(Int_t value);
  Int_t    GetPID()                      {return fPID;}

  Double_t GetRapidity();
  Double_t GetpsudoRapidity();


  Double_t GetEtotal()                   {return fEtotal;}

  Double_t GetP()                        {return fP;}
  void     SetdEdx(Double_t value)       {fdEdx = value;}
  Double_t GetdEdx()                     {return fdEdx;}
  Int_t    GetLinearPID()                {return flnPID;}

  void     SetRotatedMomentum(TVector3 value)   {fRotatedMomentum = value;}
  TVector3 GetRotatedMomentum()                 {return fRotatedMomentum;}

  TVector2 GetRotatedPt()                       {return fRotatedPt;}

  void  SetBeamonTargetFlag(Int_t value)        {fBeamonTarget = value;}
  Int_t GetBeamonTargetFlag()                   {return fBeamonTarget;}

  void  SetVertexAtTargetFlag(Int_t value)      {fVatTarget = value;}
  Int_t GetVertexAtTargetFlag()                 {return fVatTarget;}

  void  SetVertexZAtTargetFlag(Int_t value)      {fVZatTarget = value;}
  Int_t GetVertexZAtTargetFlag()                 {return fVZatTarget;}

  void  SetVertexBDCCorrelationFlag(Int_t value){fVBDCCor   = value;}
  Int_t GetVertexBDCCorrelationFlag()           {return fVBDCCor;}

  void  SetBDCCorrelationFlag(Int_t value)      {fBDCCor     = value;}
  Int_t GetBDCCorrelationFlag()                 {return fBDCCor;}

  void  SetFromTargetFlag(Int_t value)          {fTargetXY   = value;}
  Int_t GetFromTargetFlag()                     {return fTargetXY;}

  void  SetgotoKATANAFlag(Int_t value)          {fgotoKatana   = value;}
  Int_t GetgotoKATANAFlag()                     {return fgotoKatana;}

  void  SetgotoKYOTOFlag(Int_t value)          {fgotoKyoto   = value;}
  Int_t GetgotoKYOTOFlag()                     {return fgotoKyoto;}

 

  void   SetBestTrackFlag()
  {
    if(fVatTarget && fVZatTarget && fVBDCCor && fBDCCor && fTargetXY && fBeamonTarget)
      fgoodtrack = 1;
    else 
      fgoodtrack = 0;
  }
  Int_t GetBestTrackFlag()                     {return fgoodtrack;}


  void  SetGotoKATANAFlag(Int_t value)          {fgotoKatana     = value;}
  Int_t GetGotoKATANAFlag()                     {return fgotoKatana;}
  void  SetGotoKYOTOFlag(Int_t value)           {fgotoKyoto      = value;}
  Int_t SetGotoKYOTOFlag()                      {return fgotoKyoto;}

  void     SetPhi(Double_t value)        {fphi  = value;}
  Double_t GetPhi()                      {return fphi;}

  void     SetRotatedPhi(Double_t value) {frphi = value;}
  Double_t GetRotatedPhi()               {return frphi;}

  STTrack  *GetTrack(){return fTrack;}


private:
  Bool_t    bRotated = kFALSE;

  // Track XY
  Double_t trktgt_right =  -12.; //!
  Double_t trktgt_left  =   12.; //!
  Double_t trktgt_top   = -210.; //!
  Double_t trktgt_btm   = -235.; //!


private:
  STTrack *fTrack; //!
  TVector3 fRotatedMomentum;
  TVector2 fRotatedPt;

  Double_t fP;
  Double_t fdEdx;
  Double_t fphi;
  Double_t frphi;
  Int_t    fPID;
  Int_t    flnPID;
  Double_t flnPIDval;
  Double_t fRapidity;
  Double_t fpsudoRapidity;
  Double_t fEtotal;

  Int_t    fpipid;


  //flags
  Int_t    fBeamonTarget; //flag for beam tracked by BDC goes on the target
  Int_t    fVatTarget;    //flag for reconstructed vertex XY within the target
  Int_t    fVZatTarget;   //flag for reconstructed veretx Z comes from the target
  Int_t    fVBDCCor;      //flag for reconstructed vertex is correated with BDC at the target
  Int_t    fBDCCor;   //
  Int_t    fTargetXY;
  Int_t    fgoodtrack;
  Int_t    fgotoKatana;
  Int_t    fgotoKyoto;
  

  ClassDef(STParticle, 1)
};


#endif
