
/* 
 * File:   PhotonHit.hh
 * Author: wenzel
 *
 * Created on December 10, 2019, 2:55 PM
 */

#ifndef PHOTONHIT_HH
#define PHOTONHIT_HH

class PhotonHit {
public:
    PhotonHit();
    PhotonHit(float xp,
            float yp,
            float zp,
            float en,
            float ti);
    PhotonHit(const PhotonHit& orig);
    virtual ~PhotonHit();
    const PhotonHit& operator=(const PhotonHit&);
    bool operator==(const PhotonHit&) const;
    
    void SetTime(float t) {
        this->time = t;
    }

    float GetTime() const {
        return time;
    }

    void SetEnergy(float e) {
        this->energy = e;
    }

    float GetEnergy() const {
        return energy;
    }

    void SetZpos(float z) {
        this->zpos = z;
    }

    float GetZpos() const {
        return zpos;
    }

    void SetYpos(float y) {
        this->ypos = y;
    }

    float GetYpos() const {
        return ypos;
    }

    void SetXpos(float x) {
        this->xpos = x;
    }

    float GetXpos() const {
        return xpos;
    }

private:
    float xpos;
    float ypos;
    float zpos;
    float energy;
    float time;
};

#endif /* PHOTONHIT_HH */

