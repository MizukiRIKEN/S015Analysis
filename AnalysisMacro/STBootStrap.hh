#ifndef STBOOTSTRAP_HH
#define STBOOTSTRAP_HH


#include "TRandom3.h"
#include "TVector2.h"
#include "TMath.h"
#include <iostream>

class STBootStrap : public TObject {

public:
  STBootStrap(){};
  STBootStrap(UInt_t ival1, UInt_t ival2, Double_t *sample);
  STBootStrap(UInt_t ival1, UInt_t ival2, TVector2 *sample);
  STBootStrap(UInt_t ival1, vector<TVector2> *sample);
  ~STBootStrap(){};

  void clear();
  


  Double_t GetMean();
  Double_t GetStdDev();
  Double_t GetStdDev2();
  Double_t GetNElem(){return (Double_t)numElements;}

  std::vector< Double_t > GetMeanVector()  {return resMean;}
  std::vector< Double_t > GetStdDevVector(){return resStdv;}

  std::vector< Double_t > GetMeanCnvVector()  {return cnvMean;}
  std::vector< Double_t > GetStdDevCnvVector(){return cnvStdv;}
  std::vector< Double_t > GetStdDev2CnvVector(){return cnvStdv2;}

  void StoreResults(Double_t offset = 0.);
  vector< UInt_t > Resampling(UInt_t ival);
  void BootStrapping(UInt_t ival);
  void SumUpVector();

private:
  std::vector< Double_t > replace;

  std::vector< Double_t > elements;
  std::vector< TVector2 > elementsTV2;

  UInt_t numElements;

  UInt_t nboot = 0;
  std::vector< Double_t > resMean;
  std::vector< Double_t > resStdv;

  std::vector< Double_t > cnvMean;
  std::vector< Double_t > cnvStdv;
  std::vector< Double_t > cnvStdv2;

  TRandom3 rnd;

};

#endif
