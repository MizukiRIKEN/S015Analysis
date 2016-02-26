/**
 * @brief   Pattern recognition (track finding) 
 *          in the SPiRIT-TPC using the Riemann track fit.
 *
 * @author  Sebastian Neubert (TUM) -- original author for FOPIROOT
 * @author  Johannes Rauch -- FOPIROOT
 * @author  Felix Boehmer  -- FOPIROOT
 * @author  Genie Jhang (Korea University) -- implementation for SpiRITROOT
 * @author  JungWoo Lee (Korea University) -- implementation for SpiRITROOT
 */

#define PRETRACKING

// SpiRITROOT classes
#include "STRiemannTrackingTask.hh"
#include "STRiemannHit.hh"
#include "STDebugLogger.hh"
#include "STProximityHTCorrelator.hh"
#include "STHelixHTCorrelator.hh"
#include "STDipTTCorrelator.hh"
#include "STRiemannTTCorrelator.hh"
#include "STEvent.hh"
#include "STGlobal.hh"

// FairROOT classes
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairField.h"
#include "FairMCPoint.h"
#include "FairRunAna.h"

// STL
#include <map>
#include <cmath>

// ROOT classes
#include "TFile.h"
#include "TMath.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "TDatabasePDG.h"


ClassImp(STRiemannTrackingTask);

STRiemannTrackingTask::STRiemannTrackingTask()
:FairTask("ST Pattern Reconstruction")
{
  fIsPersistence = kFALSE;

  fLogger = FairLogger::GetLogger();

  UseDefaultParameterSet();
}

STRiemannTrackingTask::~STRiemannTrackingTask()
{
}

// Simple setter methods --------------------------------------------------------------------------------
void STRiemannTrackingTask::SetPersistence(Bool_t value)             { fIsPersistence = value; }
void STRiemannTrackingTask::SetMaxRMS(Double_t value)                { fMaxRMS = value; }
void STRiemannTrackingTask::SetMergeTracks(Bool_t mergeTracks)       { fMergeTracks = mergeTracks; }
void STRiemannTrackingTask::SetRiemannScale(Double_t riemannScale)   { fRiemannScale = riemannScale; }
void STRiemannTrackingTask::SkipCrossingAreas(Bool_t value)          { fSkipCrossingAreas = value; }

void 
STRiemannTrackingTask::UseDefaultParameterSet()
{
  fSortingMode  = kTRUE;
  fSorting      = STRiemannSort::kSortR;
  fInteractionZ = 42.78;

  fMinPoints    = 3;
  fProxCut      = 20.;
  fProxZStretch = 1.0;
  fHelixCut     = 5.;
  fMaxRMS       = 5.;

  fMergeTracks = kTRUE;
  fTTProxCut   = 30.;
  fTTDipCut    = 20.;
  fTTHelixCut  = 20.;
  fTTPlaneCut  = 20.;

  fRiemannScale = 25.0;

  fMergeCurlers = kTRUE;
  fBlowUp = 1.;

  fSkipCrossingAreas = kTRUE;

  fDoMultiStep = kTRUE;
  fMinHitsZ    = 2;
  fMinHitsR    = 2;
  fMinHitsPhi  = 2;
}

void 
STRiemannTrackingTask::UsePANDAParameterSet()
{
  fMinPoints    = 3;
  fProxCut      = 19.;
  fProxZStretch = 1.6;
  fHelixCut     = 2.;
  fMaxRMS       = 1.5;

  fMergeTracks = kTRUE;
  fTTProxCut   = 15.;
  fTTDipCut    = 2.;
  fTTHelixCut  = 5.;
  fTTPlaneCut  = 3.;

  fRiemannScale = 24.6;
}

void 
STRiemannTrackingTask::UseFOPIParameterSet()
{
  fMinPoints    = 3;
  fProxCut      = 2.0;
  fProxZStretch = 16.;
  fHelixCut     = 4.;
  fMaxRMS       = 3.;

  fMergeTracks = kTRUE;
  fTTProxCut   = 15.;
  fTTDipCut    = 2.;
  fTTHelixCut  = 5.;
  fTTPlaneCut  = 3.;

  fRiemannScale = 8.6;
}

void 
STRiemannTrackingTask::SetSortingParameters(Bool_t sortingMode, Int_t sorting, Double_t interactionZ)
{
  fSortingMode = sortingMode;
  fSorting = sorting;
  fInteractionZ = interactionZ;
}

void 
STRiemannTrackingTask::SetMultistepParameters
(Bool_t doMultistep, UInt_t minHitsZ, UInt_t minHitsR, UInt_t minHitsPhi)
{
  fDoMultiStep = doMultistep;
  fMinHitsZ = minHitsZ;
  fMinHitsR = minHitsR;
  fMinHitsPhi = minHitsPhi;
}

void 
STRiemannTrackingTask::SetTrkFinderParameters
(Double_t proxcut, Double_t helixcut, UInt_t minpointsforfit, Double_t zStretch)
{
  fProxCut = proxcut;
  fHelixCut = helixcut;
  fMinPoints = minpointsforfit;
  fProxZStretch = zStretch;
}

void 
STRiemannTrackingTask::SetTrkMergerParameters
(Double_t TTproxcut, Double_t TTdipcut, Double_t TThelixcut, Double_t TTplanecut)
{
  fTTProxCut = TTproxcut;
  fTTDipCut = TTdipcut;
  fTTHelixCut = TThelixcut;
  fTTPlaneCut = TTplanecut;
}

void 
STRiemannTrackingTask::SetMergeCurlers(Bool_t mergeCurlers, Double_t blowUp)
{
  fMergeCurlers = mergeCurlers;
  fBlowUp = blowUp;
}

InitStatus
STRiemannTrackingTask::Init()
{
  FairRootManager *ioMan = FairRootManager::Instance(); 
  if (ioMan == 0){
    fLogger -> Error(MESSAGE_ORIGIN, "Cannot find FairRootManager!");
    return kERROR;
  }
  
  fEventHCMArray = (TClonesArray *) ioMan -> GetObject("STEventHCM");
  if (fEventHCMArray == 0) {
    fLogger -> Error(MESSAGE_ORIGIN, "Cannot find STEventHCM array!");
    return kERROR;
  }

#ifdef PRETRACKING
  fPreTrackArray = (TClonesArray *) ioMan -> GetObject("STCurveTrack");
#endif

  fRiemannTrackArray = new TClonesArray("STRiemannTrack");
  ioMan -> Register("STRiemannTrack", "SPiRIT", fRiemannTrackArray, fIsPersistence);

  fRiemannHitArray = new TClonesArray("STRiemannHit");
  ioMan -> Register("STRiemannHit", "SPiRIT", fRiemannHitArray, kFALSE);

  fTrackFinder = new STRiemannTrackFinder();
  fTrackFinder -> SetSorting(fSorting);
  fTrackFinder -> SetInteractionZ(fInteractionZ);
  fTrackFinder -> SetSortingMode(fSortingMode);
  fTrackFinder -> SetMinHitsForFit(fMinPoints);
  fTrackFinder -> InitTracks(kFALSE);
  fTrackFinder -> SkipCrossingAreas(fSkipCrossingAreas);
  fTrackFinder -> SetSkipAndDelete(kFALSE);
  fTrackFinder -> SetScale(fRiemannScale);
  fTrackFinder -> SetProxCut(fProxCut);
  fTrackFinder -> SetTTProxCut(fTTProxCut);

  // Hit-Track Correlators
  fTrackFinder -> AddHTCorrelator(new STProximityHTCorrelator(fProxCut, fProxZStretch, fHelixCut));
  fTrackFinder -> AddHTCorrelator(new STHelixHTCorrelator(fHelixCut));

  // Track-Track Correlators
  fTrackFinder -> AddTTCorrelator(new STDipTTCorrelator(fTTProxCut, fTTDipCut, fTTHelixCut));
  fTrackFinder -> AddTTCorrelator(new STRiemannTTCorrelator(fTTPlaneCut, fMinPoints));

  // for merging curling tracks with increased TT helixcut
  fTrackFinderCurl = new STRiemannTrackFinder();
  fTrackFinderCurl -> SetSorting(fSorting);
  fTrackFinderCurl -> SetSortingMode(fSortingMode);
  fTrackFinderCurl -> SetMinHitsForFit(fMinPoints);
  fTrackFinderCurl -> SetScale(fRiemannScale);
  fTrackFinderCurl -> SetMaxNumHitsForPR(fMinPoints);

  fTrackFinderCurl -> SetProxCut(fProxCut);
  fTrackFinderCurl -> SetTTProxCut(30.);

  // Track-Track Correlators
  fTrackFinderCurl -> AddTTCorrelator(new STDipTTCorrelator(30., fBlowUp*fTTDipCut, fBlowUp*fTTHelixCut));
  fTrackFinderCurl -> AddTTCorrelator(new STRiemannTTCorrelator(1.5*fTTPlaneCut, 20));

  fHitBuffer = new std::vector<STHit *>;
  fHitBufferTemp = new std::vector<STHit *>;

  return kSUCCESS;
}


void
STRiemannTrackingTask::SetParContainers()
{
  FairRun *run = FairRun::Instance();
  if (!run) fLogger -> Fatal(MESSAGE_ORIGIN, "Cannot find analysis run!");

  FairRuntimeDb *db = run -> GetRuntimeDb();
  if (!db) fLogger -> Fatal(MESSAGE_ORIGIN, "Cannot find runtime database!");

  fPar = (STDigiPar *) db -> getContainer("STDigiPar");
  if (!fPar) fLogger -> Fatal(MESSAGE_ORIGIN, "Cannot find STDigiPar!");
}


void
STRiemannTrackingTask::Exec(Option_t *opt)
{
#ifdef TASKTIMER
  STDebugLogger::Instance() -> TimerStart("RiemannTrackingTask");
#endif
  // Clear buffers --------------------------------------------------------------------------------------
  if (fRiemannTrackArray == 0)
    fLogger -> Fatal(MESSAGE_ORIGIN, "Cannot find RiemannTrackArray!");
  fRiemannTrackArray -> Delete();

  if (fRiemannHitArray == 0)
    fLogger -> Fatal(MESSAGE_ORIGIN, "Cannot find RiemannHitArray!");
  fRiemannHitArray -> Delete();

  if (fEventHCMArray -> GetEntriesFast() == 0)
    return;

  for (Int_t i = 0; i < fRiemannList.size(); ++i){
    if (fRiemannList[i] != NULL) {
      fRiemannList[i] -> DeleteHits();
      delete fRiemannList[i];
    }
  }
  fRiemannList.clear();
  fHitBuffer -> clear();



  // Initialize -----------------------------------------------------------------------------------------
  STEvent *eventHCM = (STEvent *) fEventHCMArray -> At(0);
  fLogger -> Debug(MESSAGE_ORIGIN, "Fetching hits from hit branch...");

  UInt_t numHit = eventHCM -> GetNumClusters(); // TODO
  if (numHit == 0)
    fLogger -> Info(MESSAGE_ORIGIN, Form("Event #%d : Bad event. No hits to build tracks.", eventHCM -> GetEventID()));

#ifndef PRETRACKING
  for (UInt_t iHit = 0; iHit < numHit; iHit++) {
    STHit *hit = (STHit *) eventHCM -> GetCluster(iHit); // TODO
    fHitBuffer -> push_back(hit);
  }
#endif

  std::vector<STRiemannTrack *> riemannTemp; // temporary storage
  fLogger -> Debug(MESSAGE_ORIGIN, "Starting Pattern Reco...");



  // Pre Build & Merge ----------------------------------------------------------------------------------
#ifdef PRETRACKING
  Int_t numPreTracks = fPreTrackArray -> GetEntriesFast();
  for (Int_t iPreTrack = 0; iPreTrack < numPreTracks; iPreTrack++)
  {
    STCurveTrack *track = (STCurveTrack *) fPreTrackArray -> At(iPreTrack);

    fHitBufferTemp -> clear();
    Int_t numClusters = track -> GetNumClusters();
    for (Int_t iCluster = 0; iCluster < numClusters; iCluster++) 
    {
      Int_t clusterID = track -> GetClusterID(iCluster);
      STHit *hit = (STHit *) eventHCM -> GetCluster(clusterID);
      fHitBufferTemp -> push_back(hit);
    }

    BuildTracks(fTrackFinder, fHitBufferTemp, &riemannTemp, STRiemannSort::kSortR, fMinHitsR, fMaxRMS);

    Int_t numClustersLeft = fHitBufferTemp -> size();
    for (Int_t iCluster = 0; iCluster < numClustersLeft; iCluster++) {
      fHitBuffer -> push_back(fHitBufferTemp -> at(iCluster));
    }
  }
#endif




  // 1st Build & Merge ----------------------------------------------------------------------------------
  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortR, fMinHitsR, fMaxRMS);
  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortZ, fMinHitsZ, 0.7*fMaxRMS);
  riemannTemp.clear();

  MergeTracks();



  // 2nd Build & Merge ----------------------------------------------------------------------------------
  UInt_t nTracks = fRiemannList.size();
  for (UInt_t iTrack = 0; iTrack < nTracks; iTrack++)
    riemannTemp.push_back(fRiemannList[iTrack]);

  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortPhi, fMinHitsPhi, fMaxRMS);
  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortReversePhi, fMinHitsPhi, fMaxRMS);
  riemannTemp.clear();

  MergeTracks();



  // 3rd Build & Merge ----------------------------------------------------------------------------------
  nTracks = fRiemannList.size();
  for (UInt_t iTrack = 0; iTrack < nTracks; iTrack++)
    riemannTemp.push_back(fRiemannList[iTrack]);

  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortR, fMinPoints+3, fMaxRMS);
  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortZ, fMinPoints+1, fMaxRMS*1.5);
  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortPhi, fMinPoints+1, fMaxRMS*1.5);
  BuildTracks(fTrackFinder, fHitBuffer, &riemannTemp, STRiemannSort::kSortReversePhi, fMinPoints+1, fMaxRMS*1.5);
  riemannTemp.clear();

  MergeTracks();



  // Merge Curlers --------------------------------------------------------------------------------------
  MergeTracks();
  MergeCurlers();




  // Clear small tracklets with < 2 hits ----------------------------------------------------------------
  for (UInt_t iTrack = 0; iTrack < fRiemannList.size(); iTrack++) {
    if (fRiemannList[iTrack] -> GetNumHits() < 2){
      fRiemannList.erase(fRiemannList.begin() + iTrack);
      iTrack++;
    }
  }



  // Store ----------------------------------------------------------------------------------------------
  STRiemannTrack *track;
  UInt_t foundTracks = fRiemannList.size();
  UInt_t numUsedHit = 0;
  UInt_t numHits;

  for (UInt_t iTrack = 0; iTrack < foundTracks; iTrack++){
    track = fRiemannList[iTrack];
    numHits = track -> GetNumHits();
    numUsedHit += numHits;

    new((*fRiemannTrackArray)[fRiemannTrackArray -> GetEntriesFast()]) STRiemannTrack(*track);
    for (UInt_t iHit = 0; iHit < numHits; iHit++){
      STRiemannHit *hit = track -> GetHit(iHit);
      new ((*fRiemannHitArray)[fRiemannHitArray -> GetEntriesFast()]) STRiemannHit(*hit);
    }
  }



  fLogger -> Info(MESSAGE_ORIGIN, Form("Event #%d : Found %d tracks, used %d(/%d) hits.", 
                       eventHCM -> GetEventID(), foundTracks, numUsedHit, numHit));
#ifdef TASKTIMER
  STDebugLogger::Instance() -> TimerStop("RiemannTrackingTask");
#endif
}


void STRiemannTrackingTask::BuildTracks(STRiemannTrackFinder *trackfinder,
                                        std::vector<STHit *> *hitbuffer,
                                        std::vector<STRiemannTrack *> *trackletList,
                                        Int_t sorting,
                                        UInt_t minHits,
                                        Double_t maxRMS,
                                        Bool_t skipCrossingAreas,
                                        Bool_t skipAndDelete)
{
  Int_t nHitIn = hitbuffer -> size();
  if (nHitIn == 0)
    return;

  Int_t nTracksIn = trackletList -> size();

  fLogger -> Debug(MESSAGE_ORIGIN, Form("... building tracks from %lu hits", hitbuffer -> size()));

  trackfinder -> SetSorting(sorting);
  trackfinder -> SetMinHits(minHits);
  trackfinder -> SkipCrossingAreas(skipCrossingAreas);
  trackfinder -> SetSkipAndDelete(skipAndDelete);

  std::vector<STRiemannTrack *> trackletListCopy = *trackletList;

  STRiemannTrack *LastTrackIn;
  if (nTracksIn > 0)
    LastTrackIn = trackletList -> back();

  trackfinder -> BuildTracks(*hitbuffer, *trackletList);

  UInt_t nGoodTrks = 0, nErasedCl = 0, nHits;
  STRiemannTrack *trk;

  for (UInt_t i = 0; i < trackletList -> size(); i++) 
  {
    trk = (*trackletList)[i];

    nHits = trk -> GetNumHits();
    fLogger -> Debug(MESSAGE_ORIGIN, Form("   %d hits in tracklet.", nHits));

    if (trk -> DistRMS() < maxRMS && (nHits >= minHits || trk -> IsGood())) 
    {
#ifdef DEBUGRIEMANNCUTS // STDebugLogger.hh
      STDebugLogger::Instance() -> FillHist1("rms", trk -> DistRMS(), 100, 0, 30);
#endif
      trk -> SetFinished(kFALSE);
      trk -> SetGood();

      // clear hits from good tracklets
      for (UInt_t iHit = 0; iHit < nHits; iHit++)
        hitbuffer -> erase(remove(hitbuffer -> begin(), 
                                      hitbuffer -> end(), 
                                      trk -> GetHit(iHit) -> GetHit()), 
                                      hitbuffer -> end());
     
      nGoodTrks++;
      fLogger -> Debug(MESSAGE_ORIGIN, "======================================================== good Track!");

      //push back unique track to riemannlist
      if (std::find(trackletListCopy.begin(), trackletListCopy.end(), trk) == trackletListCopy.end()) 
        fRiemannList.push_back(trk);

    } 
    else // delete bad tracklets 
    {
      if (trk -> IsGood()) {
      // track has been found before ( -> hits were taken out) 
      // but does not pass quality criteria anymore -> fill hits back into buffer
        for (UInt_t iHit = 0; iHit < nHits; iHit++) {
          hitbuffer -> push_back(trk -> GetHit(iHit) -> GetHit());
        }
      }

      // delete hits and track
      trk -> DeleteHits();
      delete trk;
      trackletList -> erase(trackletList -> begin() + i);

      // also has to be removed from fRiemannList
      fRiemannList.erase(remove(fRiemannList.begin(), fRiemannList.end(), trk), fRiemannList.end());
      i--;
    }
  }

  Int_t hitBufferSize = hitbuffer -> size();

  fLogger -> Debug(MESSAGE_ORIGIN, 
                   Form("   nTracksIn: %d,  nHitIn: %d, hitBufferSize: %d", 
                        nTracksIn, nHitIn, hitBufferSize));

  fLogger -> Debug(MESSAGE_ORIGIN, 
                   Form("   found good tracks: %d, reuduced nHit by %d", 
                        nGoodTrks - nTracksIn, nHitIn - hitBufferSize));
}

void STRiemannTrackingTask::MergeTracks()
{
  if (!fMergeTracks) 
   return;

  fLogger -> Debug(MESSAGE_ORIGIN, 
                   Form("merge of fRiemannList: merge %lu tracks ...", fRiemannList.size()));

  fTrackFinder -> MergeTracks(fRiemannList);

  fLogger -> Debug(MESSAGE_ORIGIN, 
                   Form(" done - created %lu merged tracks", fRiemannList.size()));
}

void STRiemannTrackingTask::MergeCurlers()
{
  if (!fMergeCurlers) 
    return;

  std::vector<STRiemannTrack *> riemannTempCurl;

  for (UInt_t iTrack = 0; iTrack < fRiemannList.size(); iTrack++) 
  {
    if (   fRiemannList[iTrack] -> IsFitted() 
        && fRiemannList[iTrack] -> GetNumHits() > 5
        && fRiemannList[iTrack] -> GetR() < 30. 
        && fabs(fRiemannList[iTrack] -> GetM()*1.57) < 120)  // Pi/2
    {
      riemannTempCurl.push_back(fRiemannList[iTrack]);
      fRiemannList.erase(fRiemannList.begin() + iTrack);
      iTrack--;
    }
  }

  fLogger -> Debug(MESSAGE_ORIGIN, Form("merge curlers: merge %lu tracks ... ", riemannTempCurl.size()));

  fTrackFinderCurl -> MergeTracks(riemannTempCurl);

  fLogger -> Debug(MESSAGE_ORIGIN, Form(" done - created %lu merged tracks", riemannTempCurl.size()));

  for (UInt_t iTrack = 0; iTrack < riemannTempCurl.size(); iTrack++)
    fRiemannList.push_back(riemannTempCurl[iTrack]);
}
