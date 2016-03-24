// =================================================
//  STEvent Class                          
//                                                  
//  Description:                                    
//    Container for a event data
//                                                  
//  Genie Jhang ( geniejhang@majimak.com )          
//  2014. 05. 19                                    
// =================================================

#include <iostream>
#include <iomanip>

#include "STEvent.hh"

ClassImp(STEvent);

STEvent::STEvent(Bool_t isClustered, Bool_t isTracked, Bool_t isChanged)
:TNamed("STEvent", "Event container")
{
  fEventID = -1;

  fIsClustered = isClustered;
  fIsTracked = isTracked;
  fIsChanged = isChanged;

  fIsGood = kFALSE;
}

STEvent::STEvent(STEvent *object)
:TNamed("STEvent", "Event container")
{
  fEventID = object -> GetEventID();

  fIsClustered = object -> IsClustered();
  fIsTracked = object -> IsTracked();
  fIsChanged = object -> IsChanged();

  fHitArray = *(object -> GetHitArray());

  if (IsClustered())
    fClusterArray = *(object -> GetClusterArray());
  
  fIsGood = object -> IsGood();
  
  fVertexArray = *(object -> GetVertexArray());
  fFitTrackArray = *(object -> GetFitTrackArray());

}

STEvent::~STEvent()
{
}

void STEvent::SetIsClustered(Bool_t value)   { fIsClustered = value; }
void STEvent::SetIsTracked(Bool_t value)     { fIsTracked = value; }
void STEvent::SetIsChanged(Bool_t value)     { fIsChanged = value; }

void STEvent::SetIsGood(Bool_t value)        { fIsGood = value; }

Bool_t STEvent::IsClustered()                { return fIsClustered; }
Bool_t STEvent::IsTracked()                  { return fIsTracked; }
Bool_t STEvent::IsChanged()                  { return fIsChanged; }

Bool_t STEvent::IsGood()                     { return fIsGood; }

Int_t STEvent::GetNumClusters()              { return fClusterArray.size(); }
Int_t STEvent::GetNumVertex()                { return fVertexArray.size(); }
Int_t STEvent::GetNumFitTracks()             { return fFitTrackArray.size(); }

// setters
void STEvent::SetEventID(Int_t evtid)                             { fEventID = evtid; } 
void STEvent::AddHit(STHit *hit)                                  { fHitArray.push_back(*hit); } 
void STEvent::SetHitArray(vector<STHit> *hitArray)                { fHitArray = *hitArray; } 
void STEvent::AddCluster(STHitCluster *cluster)                   { fClusterArray.push_back(*cluster); } 
void STEvent::SetClusterArray(vector<STHitCluster> *clusterArray) { fClusterArray = *clusterArray; }
void STEvent::AddVertex(genfit::GFRaveVertex* vertex)             { fVertexArray.push_back(*vertex); } 
void STEvent::SetVertexArray(vector<genfit::GFRaveVertex> *vertexArray) { fVertexArray = *vertexArray; }
void STEvent::AddFitTrack(genfit::Track* track)                   { fFitTrackArray.push_back(*track); } 
void STEvent::SetFitTrackArray(vector<genfit::Track> *fitTrackArray) { fFitTrackArray = *fitTrackArray; }

// getters
Int_t STEvent::GetEventID() { return fEventID; }
Int_t STEvent::GetNumHits() { return fHitArray.size(); }

STHit *STEvent::GetHit(Int_t hitNo)
{
  return (hitNo < GetNumHits() ? &fHitArray[hitNo] : NULL);
}

void STEvent::RemoveHit(Int_t hitNo)
{
  if (!(hitNo < GetNumHits()))
    return;

  fHitArray.erase(fHitArray.begin() + hitNo);
}

vector<STHit> *STEvent::GetHitArray()
{
  return &fHitArray;
}

STHitCluster *STEvent::GetCluster(Int_t clusterNo)
{
  if (!(clusterNo < GetNumClusters()) || !IsClustered())
    return NULL;

  return &fClusterArray[clusterNo];
}

void STEvent::RemoveCluster(Int_t clusterNo)
{
  if (!(clusterNo < GetNumClusters()) || !IsClustered())
    return;

  fClusterArray.erase(fClusterArray.begin() + clusterNo);
}

vector<STHitCluster> *STEvent::GetClusterArray()
{
  return &fClusterArray;
}

// vertex stuff

genfit::GFRaveVertex *STEvent::GetVertex(Int_t vertexNo)
{
  if (!(vertexNo < GetNumVertex()))
    return NULL;
  
  return &fVertexArray[vertexNo];
}

void STEvent::RemoveVertex(Int_t vertexNo)
{
  if (!(vertexNo < GetNumVertex()))
    return;

  fVertexArray.erase(fVertexArray.begin() + vertexNo);
}

vector<genfit::GFRaveVertex> *STEvent::GetVertexArray()
{
  return &fVertexArray;
}

// genfit track stuff

genfit::Track *STEvent::GetFitTrack(Int_t fitTrackNo)
{
  if (!(fitTrackNo < GetNumFitTracks()))
    return NULL;
  
  return &fFitTrackArray[fitTrackNo];
}

void STEvent::RemoveFitTrack(Int_t fitTrackNo)
{
  if (!(fitTrackNo < GetNumFitTracks()))
    return;

  fFitTrackArray.erase(fFitTrackArray.begin() + fitTrackNo);
}

vector<genfit::Track> *STEvent::GetFitTrackArray()
{
  return &fFitTrackArray;
}
