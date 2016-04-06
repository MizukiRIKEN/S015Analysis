#include "STTrackCandidate.hh"

ClassImp(STTrackCandidate)

STTrackCandidate::STTrackCandidate()
{
  // Units: mm, MeV, e(charge)
  fPID = -1;
  fMass = -1;
  fCharge = 0;

  fVx = 0;
  fVy = 0;
  fVz = 0;

  fBeamVx = 0;
  fBeamVy = 0;
  fBeamVz = 0;

  fKyotoLx = 0;
  fKyotoLy = 0;
  fKyotoLz = 0;
  fKyotoRx = 0;
  fKyotoRy = 0;
  fKyotoRz = 0;
  fKatanax = 0;
  fKatanay = 0;
  fKatanaz = 0;

  fPx = 0;
  fPy = 0;
  fPz = 0;

  fTrackLength = -1;
  fdEdxTotal = -1;

  fChi2 = -1;
  fNDF = -1;
}

void STTrackCandidate::SetPID(Int_t value)                                { fPID = value; }
void STTrackCandidate::SetMass(Double_t value)                            { fMass = value; }
void STTrackCandidate::SetCharge(Int_t value)                             { fCharge = value; }

void STTrackCandidate::SetVertex(TVector3 vector)                         { fVx = vector.X(); fVy = vector.Y(); fVz = vector.Z(); }
void STTrackCandidate::SetVertex(Double_t x, Double_t y, Double_t z)      { fVx = x; fVy = y; fVz = z; }

void STTrackCandidate::SetBeamVertex(TVector3 vector)                     { fBeamVx = vector.X(); fBeamVy = vector.Y(); fBeamVz = vector.Z(); }
void STTrackCandidate::SetBeamVertex(Double_t x, Double_t y, Double_t z)  { fBeamVx = x; fBeamVy = y; fBeamVz = z; }
void STTrackCandidate::SetBeamMomentum(TVector3 vector)                     { fBeamMomx = vector.X(); fBeamMomy = vector.Y(); fBeamMomz = vector.Z(); }
void STTrackCandidate::SetBeamMomentum(Double_t x, Double_t y, Double_t z)  { fBeamMomx = x; fBeamMomy = y; fBeamMomz = z; }

void STTrackCandidate::SetKyotoLHit(TVector3 vector)                         { fKyotoLx = vector.X(); fKyotoLy = vector.Y(); fKyotoLz = vector.Z(); }
void STTrackCandidate::SetKyotoRHit(TVector3 vector)                         { fKyotoRx = vector.X(); fKyotoRy = vector.Y(); fKyotoRz = vector.Z(); }
void STTrackCandidate::SetKatanaHit(TVector3 vector)                         { fKatanax = vector.X(); fKatanay = vector.Y(); fKatanaz = vector.Z(); }

void STTrackCandidate::SetMomentum(TVector3 vector)                       { fPx = vector.X(); fPy = vector.Y(); fPz = vector.Z(); }
void STTrackCandidate::SetMomentum(Double_t px, Double_t py, Double_t pz) { fPx = px; fPy = py; fPz = pz; }

void STTrackCandidate::AdddEdx(Double_t value)                            { fdEdxArray.push_back(value); }
void STTrackCandidate::SetTrackLength(Double_t value)                     { fTrackLength = value; }
void STTrackCandidate::SetTotaldEdx(Double_t value)                       { fdEdxTotal = value; }

void STTrackCandidate::SetChi2(Double_t value)                            { fChi2 = value; }
void STTrackCandidate::SetNDF(Int_t value)                                { fNDF = value; }


Int_t STTrackCandidate::GetPID()                         { return fPID; }
Double_t STTrackCandidate::GetMass()                     { return fMass; }
Int_t STTrackCandidate::GetCharge()                      { return fCharge; }

TVector3 STTrackCandidate::GetVertex()                   { return TVector3(fVx, fVy, fVz); }
TVector3 STTrackCandidate::GetBeamVertex()               { return TVector3(fBeamVx, fBeamVy, fBeamVz); }
Double_t STTrackCandidate::GetVx()                       { return fVx; }
Double_t STTrackCandidate::GetVy()                       { return fVy; }
Double_t STTrackCandidate::GetVz()                       { return fVz; }
Double_t STTrackCandidate::GetBeamVx()                   { return fBeamVx; }
Double_t STTrackCandidate::GetBeamVy()                   { return fBeamVy; }
Double_t STTrackCandidate::GetBeamVz()                   { return fBeamVz; }

TVector3 STTrackCandidate::GetBeamMomentum()             { return TVector3(fBeamMomx, fBeamMomy, fBeamMomz); }
Double_t STTrackCandidate::GetBeamMomx()                   { return fBeamMomx; }
Double_t STTrackCandidate::GetBeamMomy()                   { return fBeamMomy; }
Double_t STTrackCandidate::GetBeamMomz()                   { return fBeamMomz; }

TVector3 STTrackCandidate::GetKyotoLHit()                { return TVector3(fKyotoLx, fKyotoLy, fKyotoLz); }
Double_t STTrackCandidate::GetKyotoLHitX()               { return fKyotoLx; }
Double_t STTrackCandidate::GetKyotoLHitY()               { return fKyotoLy; }
Double_t STTrackCandidate::GetKyotoLHitZ()               { return fKyotoLz; }

TVector3 STTrackCandidate::GetKyotoRHit()                { return TVector3(fKyotoRx, fKyotoRy, fKyotoRz); }
Double_t STTrackCandidate::GetKyotoRHitX()               { return fKyotoRx; }
Double_t STTrackCandidate::GetKyotoRHitY()               { return fKyotoRy; }
Double_t STTrackCandidate::GetKyotoRHitZ()               { return fKyotoRz; }

TVector3 STTrackCandidate::GetKatanaHit()                { return TVector3(fKatanax, fKatanay, fKatanaz); }
Double_t STTrackCandidate::GetKatanaHitX()               { return fKatanax; }
Double_t STTrackCandidate::GetKatanaHitY()               { return fKatanay; }
Double_t STTrackCandidate::GetKatanaHitZ()               { return fKatanaz; }

TVector3 STTrackCandidate::GetMomentum()                 { return TVector3(fPx, fPy, fPz); }
Double_t STTrackCandidate::GetP()                        { return GetMomentum().Mag(); }
Double_t STTrackCandidate::GetPt()                       { return GetMomentum().Perp(); }
Double_t STTrackCandidate::GetPx()                       { return fPx; }
Double_t STTrackCandidate::GetPy()                       { return fPy; }
Double_t STTrackCandidate::GetPz()                       { return fPz; }

Double_t STTrackCandidate::GetTrackLength()              { return fTrackLength; }
std::vector<Double_t> *STTrackCandidate::GetdEdxArray()  { return &fdEdxArray; }

Double_t STTrackCandidate::GetTotaldEdx()                { return fdEdxTotal; }

Double_t STTrackCandidate::GetChi2()                     { return fChi2; }
Int_t STTrackCandidate::GetNDF()                         { return fNDF; }