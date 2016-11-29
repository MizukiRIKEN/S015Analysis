/** 
 * STParticle Class
 *
 * @author Mizuki
 */

#include "STParticle.hh"

#include <iostream>

ClassImp(STParticle)

STParticle::STParticle()
{
  Clear();
}

STParticle::STParticle(STTrack *atrack)
{
  Clear();
  SetTrack(atrack);
}

Int_t STParticle::CheckTrackonTarget()
{
  TVector3 trackatTarget  = fTrack->GetTargetPlaneVertex();

  if(trackatTarget.X() >= trktgt_right && trackatTarget.X() <= trktgt_left &&
     trackatTarget.Y() >= trktgt_btm   && trackatTarget.Y() <= trktgt_top)
    fTargetXY = 1;
  else
    fTargetXY = 0;

  return fTargetXY;
}


void STParticle::Clear(Option_t *option)
{
  STTrack::Clear();

  fRotatedMomentum = TVector3(-9999,-9999,-9999);

  fphi          = -10.;
  frphi         = -10.;
  fVatTarget    = 0;
  fVBDCCor      = 0;
  fBDCCor       = 0;
  fTargetXY     = 0;
  fgotoKatana   = 0;
  fgotoKyoto    = 0;
}

void STParticle::SetTrack(STTrack *atrack)
{
  fTrack = atrack;
  STTrackCandidate::SetTrackCandidate(fTrack);  

  fphi = fTrack->GetTargetPlaneMomentum().Phi();
}

void STParticle::RotateMomentum(Double_t value)
{
  TRotation rotP;
  Double_t R_angle = value;
  rotP.RotateY(R_angle);

  fRotatedMomentum  = rotP * fTrack->GetTargetPlaneMomentum();

  frphi = fRotatedMomentum.Phi();
}
