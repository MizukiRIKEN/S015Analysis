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

STParticle::STParticle(const STParticle &cp)
{
  *this = cp;

}

STParticle &STParticle::operator=(const STParticle &cp)
{
  if( this != &cp )
    *this = cp;

  return *this;
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

  fRotatedP3 = TVector3(-9999,-9999,-9999);

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

  fpipid       = 0;
  fPID         = 0;


  // for flow
  ffltnP3 = TVector3(-9999,-9999,-9999);
  ffltnPt = TVector2(-9999,-9999);

  frpphi = -10.;
  fwgt   = 0.;
  fcorrBin[0] = -1;  
  fcorrBin[1] = -1;
  
  fmxevt = -1;
  fmxntrk = -1;
  fReactionPlane = 0;

  fcorrPt = ffltnPt;
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

  TVector3 mom = STTrackCandidate::GetMomentum();
  fcorrPt = TVector2( mom.X(), mom.Y() );

  
  SetLinearPID();
  CheckTrackonTarget();
  CheckKATANAHit();
  CheckKYOTOHit();

  SetPiPID();

  
}

void STParticle::SetLinearPID()
{
  //  for run2900
  Double_t alpha = 2.3;
  //

  flnPIDval = log(alpha*fdEdx)+(alpha-1)*log(fP+0.5*fdEdx);

  flnPID = 0;
  if(fP > 100){
    if(flnPIDval <= 12.){
      if(fCharge > 0)
	flnPID = 211;
      else
	flnPID = -211;
    }
    else if(flnPIDval <= 14.3 || fP <= 430) { // proton
      flnPID = 2212;
      fMass  = 938.272;
    }
    else if(flnPIDval <=15.1) { // deuteron
      flnPID = 10020;
      fMass  = 1875.6128;
    }
    else if(flnPIDval <=15.4) { // trition
      flnPID = 10030;
      fMass  = 2808.920936;
    }
    else if(flnPIDval <=15.8) { // 3He
      flnPID = 20030;
      fMass  = 2808.391346;
    }
    else if(flnPIDval <=16.3)  { // 4He
      flnPID = 20040;
      fMass  = 3727.37909;
    }
    else {
      flnPID = 30000;
      fMass  = 0.;
    }
  }
  else {
    if(fCharge > 0)
      flnPID = 211;
    else
      flnPID = -211;
  }

  fPID = flnPID;

}

void  STParticle::SetPID(Int_t value)
{
  fPID = value;
  if(fPID == 12212)  {
    fMass = 938.272;
  }
}

Double_t STParticle::GetpsudoRapidity()
{
  TVector3 pp;
  if( bRotated )
    pp = fRotatedP3;
  else
    pp = STTrackCandidate::GetMomentum();

  fpsudoRapidity = -log( tan((pp.Theta()/2)) );
  return fpsudoRapidity;
}

 
Double_t STParticle::GetRapidity()
{
  if( bRotated ) {
    fphi  = fRotatedP3.Phi();
    fP    = fRotatedP3.Mag();
    fPz   = fRotatedP3.Z();
  }

  if(fMass != 0 ){
    fEtotal   = sqrt(fMass*fMass + fP*fP);
    fRapidity = 0.5 * log( (fEtotal + fPz) / (fEtotal - fPz) );
  }
  else {
    fEtotal = 0;
    fRapidity = -10.;
  }

  //  std::cout << " pz " << fPz << std::endl;

  return fRapidity;
}

void  STParticle::RotateAlongBeamDirection(Double_t valuex, Double_t valuey)
{
  fRotatedP3 = STTrackCandidate::GetMomentum();

  fRotatedP3.RotateY(-valuex);
  fRotatedP3.RotateX(-valuey);

  fRotatedPt = TVector2(fRotatedP3.X(),fRotatedP3.Y());

  fcorrPt = fRotatedPt;

  bRotated = kTRUE;
}

void STParticle::Flattening(Double_t value)
{
  ffltnP3  = fRotatedP3;

  ffltnP3.SetPhi(value);

  ffltnPt  = TVector2(ffltnP3.X(), ffltnP3.Y());

  fcorrPt  = ffltnPt;

  frphi    = ffltnP3.Phi();

  bFlatten = kTRUE;
}

void STParticle::SetPiPID()
{
  //pion cut                                                                                                                                               
  // TFile *gcutPiFile = new TFile("/cache/scr/spirit/mizuki/SpiRITROOT/macros/AnalysisMacro/gcutPip.root");
  // TCutG *gPip = (TCutG*)gcutPiFile->Get("gPi");
  // gcutPiFile->Close();

  // if(gPip->IsInside(fdEdx,fP))
  //   fpipid = 1;
  // else
  //   fpipid = 0;

}
