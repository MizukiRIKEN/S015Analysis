#include "STClusterizer.hh"

#include "STHitClusterRich.hh"

#include "TMath.h"
#include "TVector3.h"

class STClusterizerScan2 : public STClusterizer
{
  public:
    STClusterizerScan2();
    ~STClusterizerScan2();

    void Analyze(STEvent* eventH, STEvent* eventHC);
    void SetParameters(Double_t *par);

    void SetPrimaryVertex(TVector3 vertex);

  private:
    std::vector<STHit *> *fHitArray;
    std::vector<STHitClusterRich *> *fHitClusterFinalArray;
    std::vector<STHitClusterRich *> *fHitClusterTempArray;

    TVector3 fPrimaryVertex; // primary vertex position

    /**
     * Correlator between hit and cluster.
     * Returns true if hit should be added to the cluster.
     */
    Bool_t CorrelateHC(STHit*, STHitClusterRich*);
    Bool_t CorrelateCC(STEvent*, STHitClusterRich*, STHitClusterRich*);
    void   MergeCC(STEvent*, STHitClusterRich*, STHitClusterRich*);

    void AddClusterToEvent(STEvent*, STHitClusterRich*);

    STHitClusterRich* fClusterTemp;

  ClassDef(STClusterizerScan2, 1)
};

class STHitSortR2
{
  public:
    Bool_t operator() (STHit* hit1, STHit* hit2)
    { return (hit1 -> GetPosition()).Mag() < (hit2 -> GetPosition()).Mag(); }
};