#include "DetHit.hh"

// allocator
G4Allocator<DetHit> DetHitAllocator;

DetHit::DetHit()
:fX(0), fY(0), fZ(0), fT(0), fE(0)
{
}

DetHit::DetHit(G4double x, G4double y, G4double z, G4double t, G4double edep)
:fX(x), fY(y), fZ(z), fT(t), fE(edep)
{
}

DetHit::~DetHit()
{
}
