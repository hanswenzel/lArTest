
#ifndef LARTESTCLASSES_H
#define LARTESTCLASSES_H

#include "SimTrajectory.hh"
#include "SimEnergyDeposit.hh"
#include <vector>
#include <map>
#include "SimStep.hh"
#include "PhotonHit.hh"
#include "AuxDetHit.hh"
std::vector<SimEnergyDeposit*>* sed;
std::vector<SimStep*>* a;
std::map<int, SimTrajectory*>*t;
std::vector<PhotonHit*>* pv;
std::vector<AuxDetHit*>* av;
#undef __G4String

#endif	// LARTESTCLASSES_H

