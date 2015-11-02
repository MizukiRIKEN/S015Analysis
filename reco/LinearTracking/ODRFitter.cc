/// @brief Orthogonal Distance Regression (ODR) Fitter.
/// @author JungWoo Lee

#include "ODRFitter.hh"

ClassImp(ODRFitter)

ODRFitter::ODRFitter()
{
  fNormal = new TVectorD(3);
  fMatrixA = new TMatrixD(3,3);
  fEigenValues = new TVectorD(3);
  fEigenVectors = new TMatrixD(3,3);

  Reset();
}

ODRFitter::~ODRFitter()
{
}

void ODRFitter::Reset()
{
  fXCentroid = 0;
  fYCentroid = 0;
  fZCentroid = 0;

  fNumPoints = 0;
  fWeightSum = 0;
  fSumOfPC2 = 0;

  for (Int_t i = 0; i < 3; i++)
    for (Int_t j = 0; j < 3; j++)
      (*fMatrixA)[i][j] = 0;

  fRMSLine = -1;
  fRMSPlane = -1;
}

void ODRFitter::SetCentroid(Double_t x, Double_t y, Double_t z)
{
  fXCentroid = x;
  fYCentroid = y;
  fZCentroid = z;
}

void ODRFitter::AddPoint(Double_t x, Double_t y, Double_t z, Double_t w)
{
  Double_t dX = x - fXCentroid;
  Double_t dY = y - fYCentroid;
  Double_t dZ = z - fZCentroid;

  Double_t wx2 = w * dX * dX;
  Double_t wy2 = w * dY * dY;
  Double_t wz2 = w * dZ * dZ;

  (*fMatrixA)[0][0] += wx2;
  (*fMatrixA)[0][1] += w * dX * dY;
  (*fMatrixA)[0][2] += w * dX * dZ;

  (*fMatrixA)[1][1] += wy2;
  (*fMatrixA)[1][2] += w * dY * dZ;

  (*fMatrixA)[2][2] += wz2;

  fSumOfPC2 += wx2 + wy2 + wz2;
  fWeightSum += w;
  fNumPoints++;
}

void ODRFitter::Solve()
{
  (*fMatrixA)[1][0] = (*fMatrixA)[0][1];
  (*fMatrixA)[2][0] = (*fMatrixA)[0][2];
  (*fMatrixA)[2][1] = (*fMatrixA)[1][2];

  (*fEigenVectors) = fMatrixA -> EigenVectors(*fEigenValues);
}

void ODRFitter::ChooseEigenValue(Int_t iEV)
{
  (*fNormal) = TMatrixDColumn((*fEigenVectors), iEV);

  fRMSLine = (fSumOfPC2 - (*fEigenValues)[iEV]) / (fWeightSum - 2*fWeightSum/fNumPoints);
  fRMSLine = TMath::Sqrt(fRMSLine);

  fRMSPlane = (*fEigenValues)[iEV] / (fWeightSum - 2*fWeightSum/fNumPoints);
  fRMSPlane = TMath::Sqrt(fRMSPlane);
}

void ODRFitter::FitLine()
{
  Solve();
  ChooseEigenValue(0);
}

void ODRFitter::FitPlane()
{
  Solve();
  ChooseEigenValue(2);
}

TVector3 ODRFitter::GetCentroid()  { return TVector3(fXCentroid, fYCentroid, fZCentroid); }
TVector3 ODRFitter::GetNormal()    { return TVector3((*fNormal)[0], (*fNormal)[1], (*fNormal)[2]); }
TVector3 ODRFitter::GetDirection() { return TVector3((*fNormal)[0], (*fNormal)[1], (*fNormal)[2]); }
   Int_t ODRFitter::GetNumPoints() { return fNumPoints; }
Double_t ODRFitter::GetWeightSum() { return fWeightSum; }
Double_t ODRFitter::GetRMSLine()   { return fRMSLine; }
Double_t ODRFitter::GetRMSPlane()  { return fRMSPlane; }
