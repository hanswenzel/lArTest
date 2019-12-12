/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimEnergyDeposit.hh
 * Author: wenzel
 *
 * Created on November 29, 2019, 4:42 PM
 */

#ifndef SimEnergyDepositSD_HH
#define SimEnergyDepositSD_HH

class SimEnergyDeposit {
public:

    const SimEnergyDeposit& operator=(const SimEnergyDeposit&);
    bool operator==(const SimEnergyDeposit&) const;
    SimEnergyDeposit();
    SimEnergyDeposit(unsigned int znph,
            unsigned int znelec,
            unsigned int ztid,
            float zx,
            float zy,
            float zz,
            float zxe,
            float zye,
            float zze,
            double zt,
            double zte,
            float zedep);
    SimEnergyDeposit(const SimEnergyDeposit& orig);
    ~SimEnergyDeposit();
    void SetEdep(float edep);

    inline float GetEdep() const {
        return edep;
    };
    void SetT(float t);

    inline float GetT() const {
        return t;
    };
    void SetZ(float z);

    inline float GetZ() const {
        return z;
    };
    
    void SetY(float y);

    inline float GetY() const {
        return y;
    };
    void SetX(float x);

    inline float GetX() const {
        return x;
    };
private:
    
    unsigned int nph;
    unsigned int nelec;
    unsigned int tid;
    float x;
    float y;
    float z;
    float xe;
    float ye;
    float ze;
    double t;
    double te;
    float edep;
};
#endif /* SimEnergyDepositSD_HH */

