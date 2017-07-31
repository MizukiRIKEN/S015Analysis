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
#include <utility>

class STParticle : public STTrack {
public:
  STParticle();
  STParticle(STTrack *atrack);
  STParticle(const STParticle &cp);
  STParticle &operator=(const STParticle &cp);

  virtual ~STParticle(){};

  virtual void Clear(Option_t *option = "");

  //private:
  void     SetProperty();
  void     SetLinearPID();

  void     CheckTrackonTarget();
  void     CheckKATANAHit(){};
  void     CheckKYOTOHit(){};


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

  void     SetRotatedMomentum(TVector3 value)   
  { fRotatedP3 = value; fcorrPt = TVector2(fRotatedP3.X(),fRotatedP3.Y()); }

  TVector3 GetRotatedMomentum()                 {return fRotatedP3;}

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


  // for flow analysis
  void     SetMixedEventID(Int_t value) {fmxevt = value;}
  void     SetMixedNtrack(Int_t value)  {fmxntrk= value;}
  void     SetMixTrackID(Int_t ival)    {fmxtrackid = ival;}

  void     SetRPWeight(Double_t value)  {fwgt = value;}
  Double_t GetRPWeight()                {return fwgt;}
  void     SetAzmAngle_wrt_RP(Double_t val) {fdeltphi = val;}
  void     SetIndividualRPAngle(Double_t val) {frpphi = val;}


  void     SetFlattenBinID(UInt_t value1, UInt_t value2) 
  {fcorrBin[0]=value1; fcorrBin[1] = value2;}

  Int_t   GetFlattenBinID(UInt_t value)       {if(value<2) return fcorrBin[value]; else return -1;}


  void     SetReactionPlaneFlag(Int_t value)    {fReactionPlane = value;}
  Int_t    GetReactionPlaneFlag()               {return fReactionPlane;}
  
  void     Flattening(Double_t value); 
  TVector3 GetFlattenMomentum()                 {return ffltnP3;}
  TVector2 GetFlattenPt()                       {return ffltnPt;}

  TVector2 GetCorrectedPt()                     {return fcorrPt;}

private:
  Bool_t    bRotated = kFALSE;
  Bool_t    bFlatten = kFALSE;


  // Track XY
  Double_t trktgt_right =  -12.; //!
  Double_t trktgt_left  =   12.; //!
  Double_t trktgt_top   = -210.; //!
  Double_t trktgt_btm   = -235.; //!


private:
  STTrack *fTrack; //!
  TVector3 fRotatedP3;
  TVector2 fRotatedPt;
  Double_t fRotatedP;

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


  // flow parameters
  TVector2 fcorrPt;

  TVector3 ffltnP3;
  TVector2 ffltnPt;

  Double_t frpphi;
  Double_t fdeltphi;
  Double_t fwgt;

  Int_t   fcorrBin[2];

  
  // --- mixed partiels
  Int_t    fmxevt  = -1;
  Int_t    fmxntrk = -1;
  Int_t    fmxtrackid = -1;


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

  Int_t    fReactionPlane;  

  ClassDef(STParticle, 3)
};


#endif
