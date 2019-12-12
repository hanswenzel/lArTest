/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimEnergyDeposit.cc
 * Author: wenzel
 * 
 * Created on November 29, 2019, 4:42 PM
 */

#include "SimEnergyDeposit.hh"

SimEnergyDeposit::SimEnergyDeposit() : nelec(0),
tid(0),
x(0.0),
y(0.0),
z(0.0),
xe(0.0),
ye(0.0),
ze(0.0),
t(0.0),
te(0.0),
edep(0.0) {
}

SimEnergyDeposit::SimEnergyDeposit(
        unsigned int znph = 0,
        unsigned int znelec = 0,
        unsigned int ztid = 0,
        float zx = 0.0,
        float zy = 0.0,
        float zz = 0.0,
        float zxe = 0.0,
        float zye = 0.0,
        float zze = 0.0,
        double zt = 0.0,
        double zte = 0.0,
        float zedep = 0.0) :
nph(znph),
nelec(znelec),
tid(ztid),
x(zx),
y(zy),
z(zz),
xe(zxe),
ye(zye),
ze(zze),
t(zt),
te(zte),
edep(zedep) {

}

SimEnergyDeposit::SimEnergyDeposit(const SimEnergyDeposit& right) {
    nph = right.nph;
    nelec = right.nelec;
    tid = right.tid;
    x = right.x;
    y = right.y;
    z = right.z;
    xe = right.xe;
    ye = right.ye;
    ze = right.ze;
    t = right.t;
    te = right.te;
    edep = right.edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SimEnergyDeposit& SimEnergyDeposit::operator=(const SimEnergyDeposit& right) {
    nph = right.nph;
    nelec = right.nelec;
    tid = right.tid;
    x = right.x;
    y = right.y;
    z = right.z;
    xe = right.xe;
    ye = right.ye;
    ze = right.ze;
    t = right.t;
    te = right.te;
    edep = right.edep;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool SimEnergyDeposit::operator==(const SimEnergyDeposit& right) const {
    return (this == &right) ? true : false;
}

SimEnergyDeposit::~SimEnergyDeposit() {
}

