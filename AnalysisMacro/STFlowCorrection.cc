#include "STFlowCorrection.hh"
void STFlowCorrection::Initialize(TObjArray *chele, UInt_t ival1, UInt_t ival2)
{
  ChEle = chele;
  Initialize(ival1, ival2);
}

void STFlowCorrection::Initialize(UInt_t ival1, UInt_t ival2)
{
  harm  = ival1;
  charm = harm;

  irm   = ival2;

  Init();
}

void STFlowCorrection::Init()
{
  indx   = new UInt_t[harm];
  An     = new Double_t[harm];
  Bn     = new Double_t[harm];
  An_rms = new Double_t[harm];
  Bn_rms = new Double_t[harm];

  if(ChEle != NULL)  SetFileName();  

  vphi.clear();

}

void STFlowCorrection::SetFileName()
{
  if( irm == 1 )
    fname = "cfMix";
  else 
    fname = "cfReal";

  TIter nnext((TCollection*)ChEle);
  TString sval = ((TFile)nnext()->GetTitle()).GetName();
  //  cout << sval << endl;
    
  Ssiz_t ifnd = sval.First("_")-7;
  fname += sval(ifnd,sval.Length()-ifnd-4);
}

void STFlowCorrection::SetFileName(TString sval)
{
  fname = sval;
  cout << fname << " in defined. " << endl;
}

UInt_t STFlowCorrection::GetCorrectionFactor(TString comm)
{

  cout << "STFlowCorrection::GetCorrectionFactor " << comm << endl;

  TString header;
  if(irm  == 1) header = "1->,";
  else          header = "0->,";

  Ssiz_t ifnd = comm.First(":");
  if(ifnd != 0) {
    fname += comm(0,ifnd);
    fname += ".txt";
  }

  fstream fin;
  fin.open(fname, fstream::in);
  if(fin == NULL) {
    std::cout << "A file " << fname << " was not found " << endl;
    return 0;
  }


  TString sget;
  
  fin >> sget;
  harm = atoi(sget);

  if(charm > harm) charm = harm;

  cout << "harm " << harm << " charm " << charm << endl;
  Init();


  Int_t j = 0;
  while(!fin.eof()){
    fin >> sget;

    if(sget == "pz<"){
      fin >> sget;
      pzmax = atof(sget);
    }
    
    else if(sget == header){
      fin >> sget;
      //      cout << j << " ->  indx[j] " << sget << endl;
      indx[j] = atoi(sget);

      fin >> sget;
      //      cout << " Bn[j] " << sget << endl;
      Bn[j] = atof(sget);

      fin >> sget;
      //      cout << " Bn_rms[j] " << sget << endl;
      Bn_rms[j] = atof(sget);

      fin >> sget;
      //      cout << " An[j] " << sget << endl;
      An[j] = atof(sget);

      fin >> sget;
      //      cout << " An_rms[j] " << sget << endl;
      An_rms[j] = atof(sget);

      j++;
    }
  }

  for(UInt_t k = 0; k < charm; k++){ 
    cout << "->, " << setw(5) << indx[k] << ", "
   	 << scientific << setprecision(5) << right
   	 << setw(20) << Bn[k] << ", " << Bn_rms[k] << ", "
   	 << setw(20) << An[k] << ", " << An_rms[k]
   	 << endl;
  }
   
  std::cout << fname << " was loaded." << std::endl;
  return 1;
}


void STFlowCorrection::GetCorrection(vector<Double_t> &val)
{

  for(Int_t i = 0; i < (Int_t)val.size(); i++)
    val.at(i) = GetCorrection(val.at(i));

}

Double_t STFlowCorrection::GetCorrection(Double_t val)
{
  Double_t cpphi = val;

  for(UInt_t k = 0; k < charm; k++){
    cpphi += An[k]*cos((Double_t)(k+1) * val);
    cpphi += Bn[k]*sin((Double_t)(k+1) * val);
  } 
  
  return cpphi;
  
}

Double_t  *STFlowCorrection::GetAverageCosin(Int_t ival, vector<Double_t> &val)
{
  vector<Double_t> fvCos;

  fvCos.clear();

  Double_t findx = (Double_t)ival;
  for(Int_t i = 0; i < (Int_t)val.size(); i++)
    fvCos.push_back(cos(findx * val.at(i)));
  

  vector<Double_t>::iterator ibgn = fvCos.begin();
  vector<Double_t>::iterator iend = fvCos.end();

  Double_t *vcos;
  vcos = new Double_t[2];
  vcos[0]  =  TMath::Mean(ibgn, iend);
  vcos[1]  =  TMath::RMS(ibgn, iend);

  return vcos;

}

vector<Double_t> STFlowCorrection::FourierCorrection()
{
  vector<Double_t> bvec = vphi;
  FourierCorrection(bvec);
  
  return bvec;
}

void STFlowCorrection::FourierCorrection(vector<Double_t> &val)
{

  std::cout << " harm = " << charm <<  "val.size " << val.size() << endl;
  vector<Double_t> fvCos;
  vector<Double_t> fvSin;

  for(UInt_t ihm = 0; ihm < charm; ihm++){ 

    fvCos.clear();
    fvSin.clear();

    Double_t findx = (Double_t)(ihm+1);

    for(Int_t ival = 0; ival < (Int_t)val.size(); ival++){

      fvCos.push_back(cos(findx * val.at(ival)));
      fvSin.push_back(sin(findx * val.at(ival)));

    }

    vector<Double_t>::iterator ibgn = fvCos.begin();
    vector<Double_t>::iterator iend = fvCos.end();

    Bn[ihm]     =  2./findx * TMath::Mean(ibgn, iend); 
    Bn_rms[ihm] =  2./findx * TMath::RMS(ibgn, iend); 


    ibgn = fvSin.begin();
    iend = fvSin.end();

    An[ihm]     = -2./findx * TMath::Mean(ibgn, iend);  
    An_rms[ihm] =  2./findx * TMath::RMS(ibgn, iend);  
  }


  for(UInt_t k = 0; k < charm; k++){
    Double_t findx = (Double_t)(k+1);
    
    std::cout << setw(5) << noshowpos << k+1 
	      << scientific << setprecision(5)  << right //<< showpos
	      << setw(6) << " Bn<cos> : " <<  setw(15) << Bn[k]  << " rms " << Bn_rms[k]
	      << "    " 
	      << setw(6) << " An<sin> : " <<  setw(15) << An[k]  << " rms " << An_rms[k]
	      << endl;
    
  }

  GetCorrection(val);
}

UInt_t STFlowCorrection::SaveCorrectionFactor(TString comm)
{
  
  fstream fout;

  Ssiz_t ifnd = comm.First(":");
  fname += comm(0,ifnd);
  fname += ".txt";

  fout.open(fname,fstream::out);
  fout << charm << endl;

  fout << "comment : " << comm << endl;

  TChainElement *ele = 0;
  UInt_t ith = 0;
  TIter nnext((TCollection*)ChEle);

  while(( ele = (TChainElement*)nnext() )){
    TFile f(ele->GetTitle());
    fout << ith << " : " << f.GetName() << endl;
    ith++;
  }

  fout << "nth  "  << "Bn<cos>        " << "rms        " <<"An<sin>        " << "rms" << endl;

  for(UInt_t k = 0; k < charm; k++){
    fout << irm <<"->, " << setw(5) << k+1 << ", "
   	 << scientific << setprecision(5) << right
   	 << setw(20) << Bn[k] << ", " << Bn_rms[k] << ", "
   	 << setw(20) << An[k] << ", " << An_rms[k]
   	 << endl;
  }  

  fout.close();
  std::cout << fname << " is created."<< endl;

  return 1;
}




#if !defined(__CINT__)
ClassImp(STFlowCorrection);
#endif





