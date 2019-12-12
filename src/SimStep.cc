/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimStep.cc
 * Author: wenzel
 * 
 * Created on November 29, 2019, 4:42 PM
 */

#include "SimStep.hh"

SimStep::SimStep() : x(0.0), y(0.0), z(0.0), len(0.0), t(0.0), edep(0.0) {
}

SimStep::SimStep(float xx = 0.0,
        float yy = 0.0,
        float zz = 0.0,
        float ll = 0.0,
        float tt = 0.0,
        float ed = 0.0) : x(xx), y(yy), z(zz), len(ll), t(tt), edep(ed) {

}

SimStep::SimStep(const SimStep& right) {
    x = right.x;
    y = right.y;
    z = right.z;
    len = right.len;
    t = right.t;
    edep = right.edep;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SimStep& SimStep::operator=(const SimStep& right) {
    x = right.x;
    y = right.y;
    z = right.z;
    len = right.len;
    t = right.t;
    edep = right.edep;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool SimStep::operator==(const SimStep& right) const {
    return (this == &right) ? true : false;
}

SimStep::~SimStep() {
}

