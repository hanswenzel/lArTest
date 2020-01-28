#ifndef INCLUDE_ROOTIO_HH 
#define INCLUDE_ROOTIO_HH 1

// Include files
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#pragma GCC diagnostic pop
#include "SimStep.hh"
#include "AuxDetHit.hh"
#include "SimTrajectory.hh"
#include "SimEnergyDeposit.hh"
#include "PhotonHit.hh"

class RootIO {
public:
    virtual ~RootIO();

    static RootIO* GetInstance();
    void Write(std::vector<PhotonHit*>* );
    void Write(std::vector<AuxDetHit*>* );
    void Write(std::vector<SimEnergyDeposit*>* );
    void Write(std::map<int,SimTrajectory*>*);
    
    void Close();

protected:
    RootIO();

private:

    TFile* fFile;
    int fNevents;

};
#endif // INCLUDE_ROOTIO_HH
