/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimTrajectory.hh
 * Author: wenzel
 *
 * Created on November 29, 2019, 4:47 PM
 */

#ifndef SIMTRAJECTORY_HH
#define SIMTRAJECTORY_HH
#include <vector>
#include "globals.hh"
class SimStep;

class SimTrajectory {
public:
    SimTrajectory();
    SimTrajectory(G4int id);
    SimTrajectory(const SimTrajectory& orig);
    ~SimTrajectory();
    std::vector<SimStep*>* GetTrajectory();
    void SetTrackID(G4int TrackID);
    G4int GetTrackID() const;
private:
    G4int TrackID;
    std::vector<SimStep*>* trajectory;
};

#endif /* SIMTRAJECTORY_HH */

