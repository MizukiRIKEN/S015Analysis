#include "STBootStrap.hh"
STBootStrap::STBootStrap(UInt_t ival1, UInt_t ival2, Double_t *sample)
{
  clear();

  nboot       = ival1;
  numElements = ival2;


  for(UInt_t i = 0; i < numElements; i++)
    elements.push_back( sample[i] );

  
  for(UInt_t i = 0; i < nboot; i++) {

    vector< UInt_t> rep = Resampling(numElements);

    replace.clear();
    for(std::vector< UInt_t >::iterator it = rep.begin(); it != rep.end(); it++)      
      replace.push_back( elements.at( *it ) );
			 
    StoreResults();
  }
}


STBootStrap::STBootStrap(UInt_t ival1, UInt_t ival2, TVector2 *sample)
{
  clear();
  
  nboot       = ival1;
  numElements = ival2;


  for(UInt_t i = 0; i < numElements; i++)
    elementsTV2.push_back( sample[i] );

  SumUpVector();
}

STBootStrap::STBootStrap(UInt_t ival1, vector<TVector2> *sample)
{
  clear();
  
  nboot       = ival1;
  numElements = sample->size();

  elementsTV2 = *sample;

  SumUpVector();
}

void STBootStrap::SumUpVector()
{

  //calculate R.P. orientation
  TVector2 sum = TVector2(0.,0.);
  for(UInt_t ielm = 0; ielm < numElements; ielm++)
    sum += elementsTV2.at(ielm);
    
  Double_t phi_mean = sum.Phi();

  vector<TVector2> rvec;
  for(UInt_t ielm = 0; ielm < numElements; ielm++) 
    rvec.push_back( elementsTV2.at(ielm) );
  //    rvec.push_back( elementsTV2.at(ielm).Rotate( phi_mean ) );

  for(UInt_t i = 0; i < nboot; i++) {
    vector< UInt_t> rep = Resampling(numElements);
    
    replace.clear();
    TVector2 tsum = TVector2(0,0);
    for(std::vector< UInt_t >::iterator it = rep.begin(); it != rep.end(); it++)      
      tsum += rvec.at( *it );
    
    replace.push_back( tsum.Phi() );

    StoreResults(phi_mean);
  }  
}


void STBootStrap::StoreResults(Double_t offset)
{
  std::vector< Double_t > replace_offset;
  //###########################3//
  for(UInt_t i = 0; i < (UInt_t)replace.size(); i++)
    replace_offset.push_back(replace.at(i) - offset);


  std::vector< Double_t >::iterator ibgn;
  std::vector< Double_t >::iterator iend;
    
  ibgn = replace_offset.begin();
  iend = replace_offset.end();
  resMean.push_back( TMath::Mean(ibgn, iend) );
  resStdv.push_back( TMath::StdDev(ibgn, iend) );

  // final results
  ibgn = resMean.begin();
  iend = resMean.end();
  cnvMean.push_back( TMath::Mean(ibgn, iend));
  cnvStdv.push_back( TMath::StdDev(ibgn, iend) );

  ibgn = resStdv.begin();
  iend = resStdv.end();
  cnvStdv2.push_back( TMath::StdDev(ibgn, iend) );

  //std::cout << " replace " << replace.size() << " cnvsize " << cnvMean.size() << " resstdv size " << resStdv.size() << std::endl;

}

void STBootStrap::clear()
{
  replace.clear();

  elements.clear();
  elementsTV2.clear();

  resMean.clear();
  resStdv.clear();

  cnvMean.clear();
  cnvStdv.clear();
  cnvStdv2.clear();
}

vector< UInt_t > STBootStrap::Resampling(UInt_t ival)
{
  vector< UInt_t > rep;
  
  for(UInt_t j = 0; j < ival; j++)
    rep.push_back( rnd.Integer(ival) );
  
  return rep;
}   

void STBootStrap::BootStrapping(UInt_t ival)
{
  nboot = ival;

  std::vector< Double_t >::iterator ibgn;
  std::vector< Double_t >::iterator iend;

  numElements = elements.size();
  for(UInt_t i = 0; i < nboot; i++){
    
    replace.clear();
    for(UInt_t j = 0; j < numElements; j++){

      Int_t iElement = rnd.Integer(numElements);
      replace.push_back( elements.at(iElement) );

    }
  
    ibgn = replace.begin();
    iend = replace.end();
    resMean.push_back( TMath::Mean(ibgn, iend) );
    resStdv.push_back( TMath::StdDev(ibgn, iend) );


    // final results
    ibgn = resMean.begin();
    iend = resMean.end();
    cnvMean.push_back( TMath::Mean(ibgn, iend) );
    cnvStdv.push_back( TMath::StdDev(ibgn, iend) );


    ibgn = resStdv.begin();
    iend = resStdv.end();
    cnvStdv2.push_back( TMath::StdDev(ibgn, iend) );
    
  }
}

Double_t STBootStrap::GetMean()
{
  if( cnvMean.size() > 0 ) return cnvMean.at( cnvMean.size() - 1 ); 

  else    return -999.;

}

Double_t STBootStrap::GetStdDev()
{
  if( cnvStdv.size() > 0 ) return cnvStdv.at( cnvStdv.size() - 1 ); 

  else    return -999.;

}

Double_t STBootStrap::GetStdDev2()
{
  if( cnvStdv2.size() > 0 ) return cnvStdv2.at( cnvStdv2.size() - 1 ); 

  else    return -999.;

}

