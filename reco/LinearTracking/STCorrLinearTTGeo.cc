#include "STCorrLinearTTGeo.hh"

STCorrLinearTTGeo::STCorrLinearTTGeo(Int_t nHitsToFitTrack,
                                     Double_t dotDrectionCut,
                                     Double_t dotNormalCut)
{
  fMinNumHitsToFitTrack = nHitsToFitTrack;
  fDotDirectionCut = dotDrectionCut;
  fDotNormalCut = dotNormalCut;
}

Bool_t 
STCorrLinearTTGeo::Correlate(STLinearTrack *track1, 
                              STLinearTrack *track2,
                              Bool_t &survive, 
                              Double_t &matchQuality)
{
  survive = kFALSE;

  if (track2 -> GetNumHits() + track2 -> GetNumHits() <= fMinNumHitsToFitTrack)
    return kFALSE;

  if (track1 -> IsFitted() == kFALSE)
    fLTFitter -> FitAndSetTrack(track1);

  if (track2 -> IsFitted() == kFALSE)
    fLTFitter -> FitAndSetTrack(track2);

  Double_t dotDirection = track1 -> GetDirection().Dot(track2 -> GetDirection());
  Double_t dotNormal    = track1 -> GetNormal().Dot(track2 -> GetNormal());

  if (dotDirection < 0) dotDirection *= -1;
  if (dotNormal < 0)    dotNormal *= -1;

  Double_t corrDirection  = (fDotDirectionCut  - dotDirection)  / fDotDirectionCut;
  Double_t corrNormal     = (fDotNormalCut - dotNormal) / fDotNormalCut;

  if (corrDirection <= 0 && corrNormal <= 0) 
  {
    matchQuality = TMath::Sqrt(matchQuality*matchQuality + 
                               corrDirection*corrDirection + 
                               corrNormal*corrNormal);
    survive = kTRUE;
  }

  return kTRUE;
}
