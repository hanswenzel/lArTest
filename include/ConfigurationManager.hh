// -----------------------------------------------------
//  _    _        _____         _   
// | |  / \   _ _|_   _|__  ___| |_ 
// | | / _ \ | '__|| |/ _ \/ __| __|
// | |/ ___ \| |   | |  __/\__ \ |_ 
// |_/_/   \_\_|   |_|\___||___/\__|
//                                  
// lArTest: A Geant4 application to study and profile  
//          simulation of physics processes relevant 
//          to liquid Ar TPCs
//
// Author: Hans Wenzel, Fermilab
// -----------------------------------------------------
#ifndef ConfigurationManager_h
#define ConfigurationManager_h 1
class ConfigurationManagerMessenger;

class ConfigurationManager {
private:

    static ConfigurationManager* instance;
    bool stepLimit; // variable determines if step limits are applied
    bool doAnalysis; // variable determines if analysis is done and root file written out
    double limitval; // value of the step limit in mm
    ConfigurationManagerMessenger* confMessenger;
public:
    ConfigurationManager();
    ~ConfigurationManager();
    static ConfigurationManager* getInstance();

    bool GetstepLimit() {
        return stepLimit;
    }

    void SetstepLimit(bool junk) {
        stepLimit = junk;
    }

    bool GetdoAnalysis() {
        return doAnalysis;
    }

    void SetdoAnalysis(bool junk) {
        doAnalysis = junk;
    }

    double Getlimitval() {
        return limitval;
    }

    void Setlimitval(double junk) {
        limitval = junk;
    }
};

#endif /* /CONFIGURATIONMANAGER */

