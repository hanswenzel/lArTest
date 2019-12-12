// Include files
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TKey.h"
#include <vector>
#include <map>
//*****************************************************************************
// To run this macro in cint do (after replacing the location_of_your_libraries below):
// .include  $G4INCLUDE
// gSystem->Load("<location_of_your_libraries>/libExP01ClassesDict.so");
// .L hits.C++
// hits();
//*****************************************************************************
#include "include/SimStep.hh"
#include "include/SimTrajectory.hh"
#include <iostream>

void hits() {
    TFile fo("hits.root");

    std::map<int, SimTrajectory*>* tmap;
    fo.GetListOfKeys()->Print();

    TIter next(fo.GetListOfKeys());
    TKey *key;
    double tot_en;
    while ((key = (TKey*) next())) {
        fo.GetObject(key->GetName(), tmap);

        tot_en = 0;
        cout << "Collection: " << key->GetName() << endl;
        key->Print();
        cout << "Number of trajectories: " << tmap->size() << endl;
        for (auto itr = tmap->begin(); itr != tmap->end(); itr++)
            cout << "ID:  " << itr->first << "  steps:  " << (itr->second)->GetTrajectory()->size() << '\n';
       
    }
}
