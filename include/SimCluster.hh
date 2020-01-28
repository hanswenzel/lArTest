/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimCluster.hh
 * Author: wenzel
 *
 * Created on November 29, 2019, 4:47 PM
 */

#ifndef SIMCLUSTER_HH
#define SIMCLUSTER_HH
#include <map>
#include <vector>
#include "globals.hh"
class SimTrajectory;

class SimCluster {
public:
    SimCluster();
    SimCluster(int,std::vector<SimTrajectory*>*);
//    SimCluster(G4int id);
    SimCluster(const SimCluster& orig);
    ~SimCluster();
    std::map<int, std::vector<SimTrajectory*>*> GetCluster();
    void SetTrackID(G4int TrackID);
    G4int GetTrackID() const;
private:
    std::map<int, std::vector<SimTrajectory*>*> tmap;
};

#endif /* SIMCLUSTER_HH */

