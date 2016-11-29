/**     
 * @brief STTrack Class         
 *                                   
 * @author Mizuki         
 */

#ifndef STPARTICLE
#define STPARTICLE

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
  Int_t    CheckTrackonTarget();
  void     RotateMomentum(Double_t value);

  void SetTrack(STTrack *atrack);
  void SetRotatedMomentum(TVector3 value){fRotatedMomentum = value;}

  void  SetVertexAtTargetFlag(Int_t value)      {fVatTarget = value;}
  Int_t GetVertexAtTargetFlag()                 {return fVatTarget;}

  void  SetVertexBDCCorrelationFlag(Int_t value){fVBDCCor   = value;}
  Int_t GetVertexBDCCorrelationFlag()           {return fVBDCCor;}

  void  SetBDCCorrelationFlag(Int_t value)      {fBDCCor     = value;}
  Int_t GetBDCCorrelationFlag()                 {return fBDCCor;}

  void  SetFromTargetFlag(Int_t value)          {fTargetXY       = value;}
  Int_t GetFromTargetFlag()                     {return fTargetXY;}

  Int_t    GetBestTrackFlag()
  {
    if(fVatTarget && fVBDCCor && fBDCCor && fTargetXY)
      return 1;
    else 
      return 0;
  }


  void  SetGotoKATANAFlag(Int_t value)          {fgotoKatana     = value;}
  Int_t GetGotoKATANAFlag()                     {return fgotoKatana;}
  void  SetGotoKYOTOFlag(Int_t value)           {fgotoKyoto      = value;}
  Int_t SetGotoKYOTOFlag()                      {return fgotoKyoto;}

  void SetPhi(Double_t value)        {fphi  = value;}
  Double_t GetPhi()                  {return fphi;}

  void SetRotatedPhi(Double_t value) {frphi = value;}
  Double_t GetRotatedPhi()           {return frphi;}

  STTrack  *GetTrack(){return fTrack;}
  TVector3 GetRotatedMomentum();


private:
  // Track XY
  Double_t trktgt_right =  -12.; //!
  Double_t trktgt_left  =   12.; //!
  Double_t trktgt_top   = -210.; //!
  Double_t trktgt_btm   = -235.; //!


private:
  STTrack *fTrack; //!
  TVector3 fRotatedMomentum;

  Double_t fphi;
  Double_t frphi;
  Int_t    fVatTarget;    //flag for reconstructed vertex comes from the target
  Int_t    fVBDCCor;      //flag for reconstructed vertex is correated with BDC at the target
  Int_t    fBDCCor;   //
  Int_t    fTargetXY;
  Int_t    fgotoKatana;
  Int_t    fgotoKyoto;


  

  ClassDef(STParticle, 1)
};


#endif
