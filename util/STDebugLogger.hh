#ifndef STDEBUGLOGGER
#define STDEBUGLOGGER

// Debug List
//#define DEBUGRIEMANNCUTS

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TObject.h"
#include "TStopwatch.h"

#include <map> 
#include <vector> 
#include <iostream>

class STDebugLogger
{
  public:
    static STDebugLogger* Create(TString name = "st_debug_logger");
    static STDebugLogger* Instance();

    STDebugLogger();
    STDebugLogger(TString name);
    ~STDebugLogger();

    void Write();

    //////////////////////////////////////////////////////////////////////////////
    void FillHist1(TString name, Double_t val,
                   Int_t nbins = 100, Double_t min = 0., Double_t max = 100.);

    void FillHist1Step(TString name, Double_t val,
                       Int_t nbins = 100, Double_t min = 0., Double_t max = 100.);

    void FillHist2(TString name, Double_t xval, Double_t yval,
                   Int_t xnbins = 100, Double_t xmin = 0., Double_t xmax = 100.,
                   Int_t ynbins = 100, Double_t ymin = 0., Double_t ymax = 100.);

    TFile* GetOutFile();
    TH1D*  GetHist1(TString name);
    TH2D*  GetHist2(TString name);
    //////////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////////////
    void FillTree(TString name, Int_t nVal, Double_t *val, TString *bname = NULL);
    //////////////////////////////////////////////////////////////////////////////


    //////////////////////////
    void TimerStart(TString);
    void TimerStop(TString);
    //////////////////////////


    ///////////////////////////////////////////////
    void SetObject(TString name, TObject* object);
    TObject* GetObject(TString name);
    ///////////////////////////////////////////////


    /////////////////////////////////////////////
    void Print(TString message);
    void Print(TString header, TString message);
    /////////////////////////////////////////////

  private:
    ////////////////////////////////////
    TFile* fOutFile;
    std::map<TString, TH1D*> fMapHist1;
    std::map<TString, TH2D*> fMapHist2;
    ////////////////////////////////////


    ////////////////////////////////////////
    std::map<TString, TTree*> fMapTree;
    std::map<TString, Int_t> fMapBranchIdx;
    std::vector<Double_t*> fMapBranchVal;
    Int_t fMaxBranchIdx;
    ////////////////////////////////////////


    ///////////////////////////////////////
    std::map<TString, TStopwatch*> fTimer;
    ///////////////////////////////////////


    ////////////////////////////////////////
    std::map<TString, TObject*> fMapObject;
    ////////////////////////////////////////

    static STDebugLogger* fInstance;

  ClassDef(STDebugLogger, 1)
};

#endif
