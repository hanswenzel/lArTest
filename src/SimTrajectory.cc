/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimTrajectory.cc
 * Author: wenzel
 * 
 * Created on November 29, 2019, 4:47 PM
 */

#include "SimTrajectory.hh"
#include "SimStep.hh"

SimTrajectory::SimTrajectory() : TrackID(0), trajectory(0) {
    trajectory = new std::vector<SimStep*>();
}

SimTrajectory::SimTrajectory(G4int id) {
    TrackID = id;
    trajectory = new std::vector<SimStep*>();
}

SimTrajectory::SimTrajectory(const SimTrajectory& orig) {

}

SimTrajectory::~SimTrajectory() {
    for (auto step = trajectory->begin(); step != trajectory->end(); ++step) {
        delete *step;
    }
    delete trajectory;
}

void SimTrajectory::SetTrackID(G4int id) {
    this->TrackID = id;
}

G4int SimTrajectory::GetTrackID() const {
    return TrackID;
}

std::vector<SimStep*>* SimTrajectory::GetTrajectory() {
    return trajectory;
}

