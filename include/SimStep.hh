/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimStep.hh
 * Author: wenzel
 *
 * Created on November 29, 2019, 4:42 PM
 */

#ifndef SIMSTEP_HH
#define SIMSTEP_HH

class SimStep {
public:

    const SimStep& operator=(const SimStep&);
    bool operator==(const SimStep&) const;
    SimStep();
    SimStep(float xx,
            float yy,
            float zz,
            float ll,
            float tt,
            float ed);
    SimStep(const SimStep& orig);
    ~SimStep();
    void SetEdep(float edep);
    inline float GetEdep() const {return edep;};
    void SetT(float t);
    inline float GetT() const {return t;};
    void SetLen(float len);
    inline float GetLen() const {return len;};
    void SetZ(float z);
    inline float GetZ() const {return z;};
    void SetY(float y);
    inline float GetY() const {return y;};
    void SetX(float x);
    inline float GetX() const {return x;};
private:
    float x;
    float y;
    float z;
    float len;
    float t;
    float edep;
};
#endif /* SIMSTEP_HH */

