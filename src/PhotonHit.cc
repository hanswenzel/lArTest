/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PhotonHit.cc
 * Author: wenzel
 * 
 * Created on December 10, 2019, 2:55 PM
 */

#include "PhotonHit.hh"


PhotonHit::PhotonHit() : xpos(0.0), ypos(0.0), zpos(0.0), energy(0.0), time(0.0) {
}

PhotonHit::PhotonHit(float xx = 0.0,
        float yy = 0.0,
        float zz = 0.0,
        float en = 0.0,
        float ti = 0.0) : xpos(xx), ypos(yy), zpos(zz), energy(en), time(ti) {

}

PhotonHit::PhotonHit(const PhotonHit& right) {
    xpos = right.xpos;
    ypos = right.ypos;
    zpos = right.zpos;
    energy = right.energy;
    time = right.time;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PhotonHit& PhotonHit::operator=(const PhotonHit& right) {
    xpos = right.xpos;
    ypos = right.ypos;
    zpos = right.zpos;
    energy = right.energy;
    time = right.time;
    return *this;
}



PhotonHit::~PhotonHit() {
}

