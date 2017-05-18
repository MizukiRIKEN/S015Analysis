void FourierCorrection(const Int_t &ival, vector<Double_t> &val)
{
  const Int_t harm = ival;
  Double_t An[harm];
  Double_t Bn[harm];
  Double_t An_rms[harm];
  Double_t Bn_rms[harm];

  cout << "val.size " << val.size() << endl;
  vector< vector<Double_t> > fvCos(harm);
  vector< vector<Double_t> > fvSin(harm);

  for(Int_t in = 0; in < harm; in++){
    fvCos[in].clear();
    fvSin[in].clear();
  }

  for(Int_t ival = 0; ival < val.size(); ival++){
    for(Int_t k = 0; k < harm; k++){

      Double_t findx = (Double_t)(k+1);

      fvCos[k].push_back(cos(findx *val.at(ival)));
      fvSin[k].push_back(sin(findx *val.at(ival)));
    }
  }


  for(Int_t k = 0; k < harm; k++){
    Double_t findx = (Double_t)(k+1);

    
    vector<Double_t>::iterator ibgn = fvCos[k].begin();
    vector<Double_t>::iterator iend = fvCos[k].end();

    Bn[k]     =  2./findx * TMath::Mean(ibgn, iend); 
    Bn_rms[k] =  2./findx * TMath::RMS(ibgn, iend); 


    ibgn = fvSin[k].begin();
    iend = fvSin[k].end();
    An[k]     = -2./findx * TMath::Mean(ibgn, iend);  
    An_rms[k] =  2./findx * TMath::RMS(ibgn, iend);  

   
    cout << setw(5) << noshowpos << k+1 
	 << scientific << setprecision(5)  << right //<< showpos
	 << setw(6) << " Bn<cos> : " <<  setw(15) << Bn[k]  << " rms " << Bn_rms[k]
	 << "    " 
	 << setw(6) << " An<sin> : " <<  setw(15) << An[k]  << " rms " << An_rms[k]
	 << endl;
  }
 


  for(Int_t i = 0; i < val.size(); i++){

    Double_t cpphi = val.at(i);
    for(Int_t k = 0; k < harm; k++){
      cpphi += An[k]*cos((Double_t)(k+1) * val.at(i));
      cpphi += Bn[k]*sin((Double_t)(k+1) * val.at(i));
    } 

    val.at(i) = cpphi;


  }
}
