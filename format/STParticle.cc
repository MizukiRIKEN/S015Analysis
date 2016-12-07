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
  //  fTrack = *this;
}

STParticle::STParticle(STTrack *atrack)
{
  Clear();
  SetTrack(atrack);
}

void STParticle::CheckTrackonTarget()
{
  TVector3 trackatTarget  = STTrackCandidate::GetTargetPlaneVertex();

  if(trackatTarget.X() >= trktgt_right && trackatTarget.X() <= trktgt_left &&
     trackatTarget.Y() >= trktgt_btm   && trackatTarget.Y() <= trktgt_top)
    fTargetXY = 1;
  else
    fTargetXY = 0;

}


void STParticle::Clear(Option_t *option)
{
  STTrack::Clear();

  fRotatedMomentum = TVector3(-9999,-9999,-9999);

  fP            = -9999.;
  fdEdx         = -9999.;
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

  SetProperty();
}

void STParticle::SetProperty()
{
  fphi  = STTrackCandidate::GetMomentum().Phi();
  fP    = STTrackCandidate::GetP();
  fdEdx = STTrackCandidate::GetTotaldEdx();
  
  SetLinearPID();
  CheckTrackonTarget();
  CheckKATANAHit();
  CheckKYOTOHit();
}

void STParticle::SetLinearPID()
{
  //  for run2900
  Double_t alpha = 2.3;
  //

  flnPIDval = log(alpha*fdEdx)+(alpha-1)*log(fP+0.5*fdEdx);

  flnPID = 0;
  if(fP > 100){
    if(flnPIDval >= 12.9 && flnPIDval <= 14.3)
      flnPID = 2212;
    else if(flnPIDval > 14)
      flnPID = 10020;
  }
  else {
    if(fCharge > 0)
      flnPID = 211;
    else
      flnPID = -211;
  }
}

void STParticle::RotateMomentum(Double_t value)
{
  TRotation rotP;
  Double_t R_angle = value;
  rotP.RotateY(R_angle);

  //  fRotatedMomentum  = rotP * fTrack->GetTargetPlaneMomentum();
  fRotatedMomentum  = rotP * STTrackCandidate::GetMomentum();

  frphi = fRotatedMomentum.Phi();
}

void STParticle::CheckKATANAHit()
{
  if( (fKatanax >= -981 && fKatanax <= 340) &&
      (fKatanay <= -34  && fKatanay >= -430))

    fgotoKatana = 1;
}

void STParticle::CheckKYOTOHit()
{
  if( fKyotoLx == 758 && 
      (fKyotoLz >= 200  && fKyotoLz <= 1800 ) &&
      (fKyotoLy <= 0  && fKyotoLy >= -800))

    fgotoKyoto = 1;

  else if( fKyotoRx == - 758 &&
      (fKyotoRz >= 200  && fKyotoRz <= 1800 ) &&
      (fKyotoRy <= 0  && fKyotoRy >= -800))

    fgotoKyoto = -1;
}
