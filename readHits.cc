
// Include files
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
//
#include <iostream>
#include <map>
#include <string>
#include "include/SimStep.hh"
#include "include/SimTrajectory.hh"
#include "include/AuxDetHit.hh"

int main(int argc, char** argv) {
    // initialize ROOT
    TH1F *hpx = new TH1F("hpx", "CRT MIP energy deposit distribution", 100, 0, 3);
    TH1F *ez = new TH1F("ez", "entry z", 100, -1, 10.);
    TH1F *exz = new TH1F("exz", "exit z", 100, -1, 10.);
    TSystem ts;
    gSystem->Load("liblArTestClassesDict");
    if (argc < 2) std::cout << "Missing name of the file to read!" << std::endl;

    TFile fo(argv[1]);
    std::cout << "reading:  " << argv[1] << std::endl;
    std::map<int, SimTrajectory*>* tmap;
    std::vector<SimStep*>* trajectory;
    std::vector<AuxDetHit*>* hitmap;
    fo.GetListOfKeys()->Print();
    TIter next(fo.GetListOfKeys());
    TKey *key;
    //double tot_en;
    while ((key = (TKey*) next())) {
        //        fo.GetObject(key->GetName(), hits);

        //tot_en = 0;
        //      std::std::cout << "Collection: " << key->GetName() << std::endl;
        // std::std::cout << "Number of hits: " << hits->size() << std::endl;
        // for (size_t i=0;i!=hits->size();i++)
        // {
        //   (*hits)[i]->Print();
        // }   

        fo.GetObject(key->GetName(), tmap);

        std::cout << "Collection: " << key->GetName() << std::endl;
        key->Print();

        const char* Collectionname = key->GetName();
        std::string s(Collectionname);
        std::string s2("Simtrajectory");
        std::size_t found = s.find(s2);
        //s.compare(0, 13, s)
        if (found == 0) {
            std::cout << "Number of trajectories: " << tmap->size() << std::endl;
            for (auto itr = tmap->begin(); itr != tmap->end(); itr++) {
                std::cout << "ID:  " << itr->first << "  steps:  " << (itr->second)->GetTrajectory()->size() << '\n';
                trajectory = (itr->second)->GetTrajectory();
                for (auto vitr = trajectory->begin(); vitr != trajectory->end(); vitr++) {
                    SimStep* st = *vitr;
                    std::cout << " Edep:  " << st->GetEdep()
                            << "  Len:  " << st->GetLen()
                            << "  Time: " << st->GetT()
                            << "  X:    " << st->GetX()
                            << "  Y:    " << st->GetY()
                            << "  Z:    " << st->GetZ()
                            << std::endl;
                }
            }
        }
        fo.GetObject(key->GetName(), hitmap);
        std::cout << "Collection: " << key->GetName() << std::endl;
        key->Print();

        Collectionname = key->GetName();
        std::string s3(Collectionname);
        std::string s4("AuxDetHit");
        found = s3.find(s4);
        //s.compare(0, 13, s)
        if (found == 0) {
            std::cout << "Number of AuxDetHits: " << hitmap->size() << std::endl;
            for (auto itr = hitmap->begin(); itr != hitmap->end(); itr++) {
                hpx->Fill((*itr)->GetEnergyDeposited());
                ez->Fill((*itr)->GetEntryZ());
                exz->Fill((*itr)->GetExitZ());
                /*
                std::cout
                        << "geoID: " << ((*itr))->GetID() << " track ID: " << ((*itr))->GetTrackID()
                        << " Edep:  " << (*itr)->GetEnergyDeposited()
                        << " Entry z:  " << (*itr)->GetEntryZ()
                        << " Exit z:  " << (*itr)->GetExitZ()
                        << " exit Time:  " << (*itr)->GetExitT() << std::endl;
                 */
            }
        }
    }
    TFile hfile("hsimple.root", "RECREATE", "Demo ROOT file with histograms");
    hpx->Write();
    ez->Write();
    exz->Write();
    hfile.Write();
    hfile.Close();

}

