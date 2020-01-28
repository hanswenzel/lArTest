
/* 
 * File:   SimCluster.cc
 * Author: wenzel
 * 
 * Created on November 29, 2019, 4:47 PM
 */

#include "SimCluster.hh"
#include "SimTrajectory.hh"

SimCluster::SimCluster() {
    tmap = std::map<int, std::vector<SimTrajectory*>*>();
}

SimCluster::SimCluster(int i, std::vector<SimTrajectory*>* vec) {
     tmap[i] =vec;
}

SimCluster::SimCluster(const SimCluster& orig) {

}

SimCluster::~SimCluster() {
    for (auto it = tmap.begin(); it != tmap.end(); it++) {
       delete (it->second); 
       std::cout << it->first << "->" << it->second << std::endl;   
    }
    tmap.clear();
}

std::map<int, std::vector<SimTrajectory*>*> SimCluster::GetCluster() {
    return tmap;
}

