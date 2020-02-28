#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 
#include <cmath>
#include <TCanvas.h> 
#include <TGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TFrame.h>
#include <TFile.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
using namespace std;

//-------------------------------------------------------------------------------
//
// see formula 33.5 in Review of particle physics 2018
// Bethe equation 
//
// m:    mass of particle in MeV
// q:    charge of particle
// plab: momentum
// ro:   density of material in g/cm3
// Z:    Z of material
// A:    Atomic number of material
// I:    mean excitation energy
// 
// the PDG data is taken from:
//  http://pdg.lbl.gov/2019/AtomicNuclearProperties
// For liquid Argon also see: https://lar.bnl.gov/properties/pass.html
//-------------------------------------------------------------------------------

double stoppingPower(double m, double q, double plab, double ro, double Z, double A, double I) {

    const double k = 0.307075;
    double g, b2, T, SP;
    double E = sqrt(plab * plab + m * m) - m;

    //printf("\n|******** Stopping-Power values for this material ********|\n");

    g = 1 + (E / m);
    //printf("\n Gamma  =  %lf", g);

    b2 = 1 - (1 / (g * g));
    //printf("\n Beta squared  =  %lf", b2);

    T = (1.022 * b2 * g * g) / (1 + 2 * g * (0.511 / m) + pow(0.511 / m, 2));
    //printf("\n Maximum kinetic energy transferred  =  %lf MeV ", T);

    SP = ((ro * k * q * q * Z) / (A * b2))*(0.5 * log((1.022 * b2 * g * g * T) / (I * I)) - b2);
    //printf("\n Stopping Power  =  %lf MeV/cm \n", SP);
    //printf("\n Stopping Power  =  %lf MeV cm^2/g \n", SP/ro);
    return SP / ro;
}

void bnl() {
    const double pmass = 0.9382720813e3; // proton mass in MeV
    const Double_t rho = 1.396;

    // liquid Argon properties
    const double density = 1.396; // density of liquid Ar in g/cm^3 
    const double Z = 18.; // Atomic number
    const double A = 39.948; // Atomic mass
    const double I = 0.000188; // Mean excitation energy in MeV
    const double mucharge = 1;
    const double mumass = 105.6583755; // muon mass in MerV


    const Int_t nbnl = 34;
    Double_t pbnl[nbnl];
    Double_t kinEbnl[nbnl] = {
        1.000E+02,
        1.250E+02,
        1.500E+02,
        1.750E+02,
        2.000E+02,
        2.250E+02,
        2.500E+02,
        2.750E+02,
        3.000E+02,
        3.500E+02,
        4.000E+02,
        4.500E+02,
        5.000E+02,
        5.500E+02,
        6.000E+02,
        6.500E+02,
        7.000E+02,
        7.500E+02,
        8.000E+02,
        8.500E+02,
        9.000E+02,
        9.500E+02,
        1.000E+03,
        1.500E+03,
        2.000E+03,
        2.500E+03,
        3.000E+03,
        4.000E+03,
        5.000E+03,
        6.000E+03,
        7.000E+03,
        8.000E+03,
        9.000E+03,
        1.000E+04
    };


    Double_t dEdxbnl[nbnl] = {
        3.66931,
        3.10221,
        2.73619,
        2.43215,
        2.27195,
        2.09963,
        1.97375,
        1.86754,
        1.7785,
        1.63627,
        1.53392,
        1.45346,
        1.38787,
        1.33565,
        1.29155,
        1.25326,
        1.22411,
        1.196,
        1.17551,
        1.15686,
        1.13857,
        1.12452,
        1.11071,
        1.03482,
        1.00639,
        0.990632,
        0.986747,
        0.985095,
        0.989974,
        0.991862,
        0.9939,
        0.995083,
        0.99861,
        1.00003
    };
    // thick target
    Double_t dEdxbnl2[nbnl] = {
        3.68451,
        3.12694,
        2.74474,
        2.47601,
        2.26868,
        2.11378,
        1.98019,
        1.87208,
        1.78999,
        1.65081,
        1.54265,
        1.45833,
        1.39304,
        1.34487,
        1.30154,
        1.27353,
        1.24102,
        1.21491,
        1.19274,
        1.17537,
        1.15506,
        1.15117,
        1.13138,
        1.06139,
        1.02762,
        1.01747,
        1.01215,
        1.01458,
        1.01066,
        1.01824,
        1.01728,
        1.02073,
        1.02302,
        1.03021
    };
    //emz em option
    Double_t dEdx_emz[nbnl] = {
        3.68451,
        3.12694,
        2.74474,
        2.47601,
        2.26868,
        2.11378,
        1.98019,
        1.87208,
        1.78999,
        1.65081,
        1.54265,
        1.45833,
        1.39304,
        1.34487,
        1.30154,
        1.27353,
        1.24102,
        1.21491,
        1.19274,
        1.17537,
        1.15506,
        1.15117,
        1.13138,
        1.06139,
        1.02762,
        1.01747,
        1.01215,
        1.01458,
        1.01066,
        1.01824,
        1.01728,
        1.02073,
        1.02302,
        1.03021
    };
    Double_t dEdx_mup_emz[nbnl] = {
        1.17157,
        1.11392,
        1.08284,
        1.06709,
        1.05962,
        1.05312,
        1.03941,
        1.04291,
        1.03448,
        1.03901,
        1.03714,
        1.04208,
        1.04335,
        1.04469,
        1.05433,
        1.05666,
        1.05308,
        1.05408,
        1.05887,
        1.057,
        1.06637,
        1.06021,
        1.07113,
        1.08,
        1.09643,
        1.098,
        1.09891,
        1.11255,
        1.11221,
        1.12294,
        1.12621,
        1.13127,
        1.12866,
        1.13318
    };
    /*
    Double_t loss_mup_emz[nbnl] = {
        2.44479,
        2.33026,
        2.26103,
        2.21134,
        2.18264,
        2.15278,
        2.14917,
        2.13146,
        2.11831,
        2.11294,
        2.12005,
        2.114,
        2.09819,
        2.12275,
        2.11096,
        2.12833,
        2.12562,
        2.11405,
        2.12487,
        2.12307,
        2.11808,
        2.13384,
        2.1365,
        2.14589,
        2.18515,
        2.18432,
        2.19855,
        2.19707,
        2.21425,
        2.21799,
        2.22692,
        2.23029,
        2.22989,
        2.2401
    };
     
    // primary only
    Double_t loss_mup_emz[nbnl] = {
        2.5372,
        2.23047,
        2.17925,
        2.3572,
        2.12827,
        2.10278,
        2.1101,
        2.10611,
        2.10977,
        2.10492,
        2.12801,
        2.13363,
        2.12956,
        2.14908,
        2.14026,
        2.15943,
        2.16163,
        2.14412,
        2.15585,
        2.17021,
        2.143,
        2.1775,
        2.19367,
        2.2106,
        2.22724,
        2.22757,
        2.27401,
        2.24025,
        2.27935,
        6.31176,
        2.29842,
        2.28171,
        4.98638,
        2.30268
    };
    */
        Double_t loss_mup_emz[nbnl] = {
    2.33644,
2.23298,
2.17515,
2.14066,
2.11962,
2.10701,
2.10248,
2.10406,
2.10694,
2.11299,
2.11336,
2.12812,
2.12655,
2.13776,
2.13811,
2.14306,
2.11282,
2.14412,
2.15401,
2.16576,
2.15677,
2.17764,
2.17979,
2.2063,
2.22711,
2.2456,
2.25909,
2.26393,
2.27984,
2.28003,
2.28989,
2.28597,
2.29926,
2.30583};
    /*  // 1cm paddle
        Double_t loss_p1_emz[nbnl] = {
            7.47642,
            6.32298,
            5.61114,
            5.09167,
            4.68839,
            4.37813,
            4.12989,
            3.91989,
            3.74892,
            3.46864,
            3.27589,
            3.11169,
            2.98551,
            2.88763,
            2.80349,
            2.74622,
            2.68141,
            2.65005,
            2.61372,
            2.56593,
            2.53191,
            2.49593,
            2.49741,
            2.35964,
            2.33139,
            2.34163,
            2.34742,
            2.333,
            2.37566,
            2.37387,
            2.37555,
            2.39937,
            2.41299,
            2.41353
        };
     */
    /*
     * // all losses (including secon)
    Double_t loss_p_emz[nbnl] = {
        3.68915,
        3.13966,
        2.7898,
        2.53251,
        2.33675,
        2.1858,
        2.05619,
        1.95379,
        1.87065,
        1.73177,
        1.63014,
        1.55046,
        1.48516,
        1.43844,
        1.39572,
        1.36057,
        1.32806,
        1.30852,
        1.28756,
        1.27218,
        1.26264,
        1.24046,
        1.21639,
        1.15331,
        1.14616,
        1.16481,
        1.15513,
        1.14683,
        1.1503,
        1.17187,
        1.1963,
        1.18796,
        1.21214,
        1.18851
    };
     
    Double_t loss_p_emz[nbnl] = {
        7.4743,
        6.31121,
        5.54239,
        4.98763,
        4.56534,
        4.23925,
        3.98009,
        3.76315,
        3.58443,
        3.29899,
        3.09777,
        2.932,
        2.80307,
        2.70182,
        2.61642,
        2.55287,
        2.48819,
        2.44741,
        2.40712,
        2.36461,
        2.33048,
        2.29832,
        2.28541,
        2.14498,
        2.11294,
        2.34121,
        2.12535,
        2.11874,
        2.1519,
        2.14628,
        2.14917,
        2.17776,
        2.18301,
        2.1921
    };
     */
    Double_t loss_p_emz[nbnl] = {
        7.4743,
        6.31121,
        5.54239,
        4.98763,
        4.565,
        4.23765,
        3.97667,
        3.76055,
        3.58377,
        3.29982,
        3.0931,
        2.93088,
        2.80483,
        2.70191,
        2.61538,
        2.54872,
        2.48605,
        2.44123,
        2.4046,
        2.36093,
        2.32856,
        2.3012,
        2.28084,
        2.14365,
        2.15194,
        2.10863,
        2.11669,
        2.12515,
        2.13459,
        2.14845,
        2.1479,
        2.16214,
        2.1809,
        2.17722
    };
    for (int i = 0; i < nbnl; i++) {
        dEdxbnl[i] = (dEdxbnl[i]*2.0) / rho;
        dEdxbnl2[i] = (dEdxbnl2[i]*2.0) / rho;
        dEdx_emz[i] = (dEdx_emz[i]*2.0) / rho;
        dEdx_mup_emz[i] = (dEdx_mup_emz[i]*2.0) / rho;
        loss_mup_emz[i] = loss_mup_emz[i] / rho;
        loss_p_emz[i] = loss_p_emz[i] / rho;
        //        pbnl[i] = sqrt((kinEbnl[i] + pmass)*(kinEbnl[i] + pmass) - pmass * pmass);
    }




    const Int_t nr = 128;

    Double_t kinEG4[nr]
            = {12,
        14,
        17,
        20,
        25,
        30,
        35,
        40,
        45,
        50,
        55,
        60,
        70,
        80,
        90,
        100,
        120,
        140,
        170,
        200,
        250,
        300,
        350,
        400,
        450,
        500,
        550,
        600,
        700,
        800,
        900,
        1000,
        1200,
        1400,
        1700,
        2000,
        2500,
        3000,
        3500,
        4000,
        4500,
        5000,
        5500,
        6000,
        7000,
        8000,
        9000,
        10000,
        12000,
        14000,
        17000,
        20000,
        25000,
        30000,
        35000,
        40000,
        45000,
        50000,
        55000,
        60000,
        70000,
        80000,
        90000,
        100000,
        120000,
        140000,
        170000,
        200000,
        250000,
        300000,
        350000,
        400000,
        450000,
        500000,
        550000,
        600000,
        700000,
        800000,
        900000,
        1e+06,
        1.2e+06,
        1.4e+06,
        1.7e+06,
        2e+06,
        2.5e+06,
        3e+06,
        3.5e+06,
        4e+06,
        4.5e+06,
        5e+06,
        5.5e+06,
        6e+06,
        7e+06,
        8e+06,
        9e+06,
        1e+07,
        1.2e+07,
        1.4e+07,
        1.7e+07,
        2e+07,
        2.5e+07,
        3e+07,
        3.5e+07,
        4e+07,
        4.5e+07,
        5e+07,
        5.5e+07,
        6e+07,
        7e+07,
        8e+07,
        9e+07,
        1e+08,
        1.2e+08,
        1.4e+08,
        1.7e+08,
        2e+08,
        2.5e+08,
        3e+08,
        3.5e+08,
        4e+08,
        4.5e+08,
        5e+08,
        5.5e+08,
        6e+08,
        7e+08,
        8e+08,
        9e+08,
        1e+09};

    Double_t dEdxG4[nr]{
        3.99311,
        3.4312,
        2.90324,
        2.55287,
        2.17567,
        1.94061,
        1.76802,
        1.6455,
        1.53773,
        1.4622,
        1.40664,
        1.35243,
        1.27228,
        1.22043,
        1.17006,
        1.14309,
        1.09139,
        1.06337,
        1.03756,
        1.01783,
        0.999097,
        0.992662,
        0.987015,
        0.978872,
        0.984608,
        0.984875,
        0.983612,
        0.986177,
        0.987747,
        0.993511,
        0.990736,
        0.998238,
        1.00614,
        1.01492,
        1.02228,
        1.02411,
        1.02871,
        1.0304,
        1.03738,
        1.04085,
        1.04485,
        1.0478,
        1.04696,
        1.04889,
        1.05181,
        1.05844,
        1.06024,
        1.05815,
        1.05822,
        1.05903,
        1.06535,
        1.06838,
        1.0651,
        1.06711,
        1.06955,
        1.06675,
        1.068,
        1.06735,
        1.06746,
        1.06837,
        1.0686,
        1.06626,
        1.07171,
        1.07583,
        1.0716,
        1.0722,
        1.07313,
        1.07273,
        1.07201,
        1.07208,
        1.06874,
        1.07078,
        1.0703,
        1.06862,
        1.07305,
        1.07003,
        1.07158,
        1.07241,
        1.07404,
        1.07327,
        1.07157,
        1.07739,
        1.07516,
        1.07302,
        1.07369,
        1.07287,
        1.07214,
        1.07242,
        1.07246,
        1.07687,
        1.07775,
        1.07899,
        1.08118,
        1.07674,
        1.07351,
        1.07754,
        1.08245,
        1.08293,
        1.0821,
        1.07901,
        1.07911,
        1.07913,
        1.08221,
        1.07945,
        1.08012,
        1.08255,
        1.08418,
        1.08116,
        1.08326,
        1.08001,
        1.08382,
        1.08286,
        1.08008,
        1.07992,
        1.08784,
        1.08249,
        1.07757,
        1.0798,
        1.08085,
        1.0791,
        1.0769,
        1.08486,
        1.07919,
        1.07891,
        1.07809,
        1.07573,
        1.07743,
        1.07772
    };

    for (int i = 0; i < nr; i++) {
        dEdxG4[i] = (dEdxG4[i]*2.0) / rho;
    }
/*
    TGraph *ve = new TGraph(nr, kinEG4, dEdxG4);
    ve-> SetTitle("Geant4 dEdx ");
    ve->SetLineColor(2);
    ve->SetMarkerColor(2);
    ve->SetMarkerStyle(22);
    ve->SetMarkerSize(2);
    ve->Draw();
*/
    ifstream myfilepdg("muE_liquid_argon.txt");
    bool head = true;
    vector<double> pdgkinE;
    vector<double> pdgmomentum;
    vector<double> pdgion;
    vector<double> pdgbbion; // using Bethe Bloch formula to calculate Ionization loss
    vector<double> pdgbrems;
    vector<double> pdgpair;
    vector<double> pdgphotonuclear;
    vector<double> pdgradloss; // sum: brems+pair+photonuclear
    vector<double> pdgdEdx;
    vector<double> pdgcsda;
    vector<double> pdglength;
    string line;
    string mmi = "Minimum ionization";
    string mce = "Muon critical energy";
    string header = "-----------------------";

    if (myfilepdg.is_open()) {
        while (getline(myfilepdg, line)) {

            if (!head) {
                std::size_t foundmmi = line.find(mmi);
                std::size_t foundmce = line.find(mce);
                if (foundmmi == std::string::npos && foundmce == std::string::npos) {
                    std::istringstream iss(line);
                    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                            std::istream_iterator<std::string>());
                    std::stringstream ss;
                    ss << results.at(0);
                    double val;
                    ss >> val;
                    if (val >= 100. && val <= 10000.) {
                        pdgkinE.push_back(val);
                        cout << val << " ";
                        ss.str("");
                        ss.clear();
                        ss << results.at(1);
                        ss >> val;
                        pdgmomentum.push_back(val);
                        ss.str("");
                        ss.clear();
                        ss << results.at(2);
                        ss >> val;
                        pdgion.push_back(val);
                        ss.str("");
                        ss.clear();
                        ss << results.at(3);
                        ss >> val;
                        pdgbrems.push_back(val);
                        ss.str("");
                        ss.clear();
                        ss << results.at(4);
                        ss >> val;
                        pdgpair.push_back(val);
                        ss.str("");
                        ss.clear();
                        ss << results.at(5);
                        ss >> val;
                        pdgphotonuclear.push_back(val);
                        ss.str("");
                        ss.clear();
                        ss << results.at(6);
                        ss >> val;
                        pdgradloss.push_back(val);
                        ss.str("");
                        ss.clear();
                        ss << results.at(7);
                        ss >> val;
                        pdgdEdx.push_back(val);
                        ss.str("");
                        ss.clear();
                        ss << results.at(8);
                        ss >> val;
                        pdgcsda.push_back(val);
                        pdglength.push_back(val / density);
                    }
                    //		std::cout <<results.at(1)<<"  "<< results.at(11)<<std::endl;
                }
            } else {
                //	    std::cout<<line<<std::endl;
                std::size_t found = line.find(header);
                if (found != std::string::npos) {
                    head = false;
                    //std::cout << "'-----------------------' also found at: " << found << '\n';
                }
            }
        }
    } else {
        cout << "Unable to open file";
        exit(EXIT_FAILURE);
    }
    myfilepdg.close();
    double pdgmomentumarr[pdgmomentum.size()];
    std::copy(pdgmomentum.begin(), pdgmomentum.end(), pdgmomentumarr);
    double pdgkinEarr[pdgkinE.size()];
    std::copy(pdgkinE.begin(), pdgkinE.end(), pdgkinEarr);
    double pdgionarr[pdgion.size()];
    std::copy(pdgion.begin(), pdgion.end(), pdgionarr);
    double pdgbremsarr[pdgbrems.size()];
    std::copy(pdgbrems.begin(), pdgbrems.end(), pdgbremsarr);
    double pdgpairarr[pdgpair.size()];
    std::copy(pdgpair.begin(), pdgpair.end(), pdgpairarr);
    double pdgphotonucleararr[pdgphotonuclear.size()];
    std::copy(pdgphotonuclear.begin(), pdgphotonuclear.end(), pdgphotonucleararr);
    double pdgradlossarr[pdgradloss.size()];
    std::copy(pdgradloss.begin(), pdgradloss.end(), pdgradlossarr);
    double pdgdEdxarr[pdgdEdx.size()];
    std::copy(pdgdEdx.begin(), pdgdEdx.end(), pdgdEdxarr);
    double pdgcsdaarr[pdgcsda.size()];
    std::copy(pdgcsda.begin(), pdgcsda.end(), pdgcsdaarr);
    double pdglengtharr[pdglength.size()];
    std::copy(pdglength.begin(), pdglength.end(), pdglengtharr);



    //    string header = "-----------------------";
    //    
    //  pstar predictions from https://physics.nist.gov/PhysRefData/Star/Text/PSTAR.html
    //
    ifstream myfilenist("proton_argon.txt");
    head = true;
    vector<double> pkinE;
    vector<double> pestop;
    vector<double> pnstop;
    vector<double> ptstop;
    vector<double> pcdsa;
    /*
        // liquid Argon properties
        const double density = 1.396; // density of liquid Ar in g/cm^3 
        const double Z = 18.; // Atomic number
        const double A = 39.948; // Atomic mass
        const double I = 0.000188; // Mean excitation energy in MeV
        const double mucharge = 1;
        charge 105.6583755
     */
    //    string line;
    if (myfilenist.is_open()) {
        while (getline(myfilenist, line)) {

            if (head) {
                std::size_t found = line.find(header);
                if (found != std::string::npos) {
                    head = false;
                }
            } else {
                std::istringstream iss(line);
                std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                        std::istream_iterator<std::string>());
                std::stringstream ss;
                ss << results.at(0);
                double val;
                ss >> val;
                if (val >= 100.) {
                    pkinE.push_back(val);
                    cout << val << " ";
                    ss.str("");
                    ss.clear();
                    ss << results.at(1);
                    ss >> val;
                    pestop.push_back(val);
                    //                cout<<val<<endl;
                    ss.str("");
                    ss.clear();
                    ss << results.at(2);
                    ss >> val;
                    pnstop.push_back(val);
                    ss.str("");
                    ss.clear();
                    ss << results.at(3);
                    ss >> val;
                    ptstop.push_back(val);
                    ss.str("");
                    ss.clear();
                    ss << results.at(4);
                    ss >> val;
                    pcdsa.push_back(val);
                }
                //		std::cout <<results.at(1)<<"  "<< results.at(11)<<std::endl;
            }

        }
        myfilenist.close();
    } else {
        cout << "Unable to open file";
        exit(EXIT_FAILURE);
    }
    vector<double> bbion;
    vector<double> bbionp;
    for (std::vector<double>::iterator it = pkinE.begin(); it != pkinE.end(); ++it) {
        // convert to momentum
        double pb = sqrt((*it + mumass)*(*it + mumass) - mumass * mumass);
        //std::cout << *it << "    " << stoppingPower(mumass, mucharge, *it, density, Z, A, I) << endl;
        bbion.push_back(stoppingPower(mumass, mucharge, pb, density, Z, A, I));
        pb = sqrt((*it + pmass)*(*it + pmass) - pmass * pmass);
        bbionp.push_back(stoppingPower(pmass, mucharge, pb, density, Z, A, I));
    }
    double pkinEarr[pkinE.size()];
    std::copy(pkinE.begin(), pkinE.end(), pkinEarr);
    double pestoparr[ptstop.size()];
    std::copy(pestop.begin(), pestop.end(), pestoparr);
    double ptstoparr[ptstop.size()];
    std::copy(ptstop.begin(), ptstop.end(), ptstoparr);
    double bbionarr[pkinE.size()];
    std::copy(bbion.begin(), bbion.end(), bbionarr);
    double bbionparr[pkinE.size()];
    std::copy(bbionp.begin(), bbionp.end(), bbionparr);
    TCanvas* c = new TCanvas("dEdx canvas", "dE/dx", 200, 10, 700, 500);
    c->SetGrid();
    c->GetFrame()->SetBorderSize(12);
    c->SetLogx();
    c->SetLogy();
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("stopping power;kinetic Energy [MeV/c^{2}];-dE/dx[MeVg^{-1}cm^{2}]");
    mg->GetXaxis()->SetLimits(100, 10000.);
    mg->SetMinimum(1.);
    mg->SetMaximum(6.);
    TGraph* dEdxgr = new TGraph(pkinE.size(), pkinEarr, ptstoparr);
    dEdxgr->SetTitle("proton NIST (PSTAR) liquid Ar dEdx prediction");
    dEdxgr->SetName("dEdxgr");
    dEdxgr->SetMarkerColor(4);
    dEdxgr->SetLineColor(4);
    dEdxgr->SetMarkerStyle(20);
    dEdxgr->SetMarkerSize(1);
    dEdxgr->SetLineWidth(1);
    //dEdxgr->GetXaxis()->SetTitle("E [MeV/c^{2}]");
    //dEdxgr->GetYaxis()->SetTitle("dEdx [Me   mg->SetMinimum(0.);
    // mg->SetMaximum(10.);V cm^{2}/g]");
    //dEdxgr->Draw();
    mg->Add(dEdxgr);
    TGraph* radlossgr = new TGraph(nbnl, kinEbnl, dEdxbnl2);
    radlossgr->SetTitle("proton Geant4 standard em physics (visible)");
    radlossgr->SetName("radlossgr");
    radlossgr->SetMarkerColor(8);
    radlossgr->SetLineColor(8);
    radlossgr->SetMarkerStyle(25);
    radlossgr->SetMarkerSize(1);
    radlossgr->SetLineWidth(1);
    //    radlossgr->Draw("SAME");
    mg->Add(radlossgr);
    TGraph* pestopgr = new TGraph(pkinE.size(), pkinEarr, dEdx_emz);
    pestopgr->SetTitle("proton Geant4 high precision em physics (visible)");
    pestopgr->SetName("");
    pestopgr->SetMarkerColor(6);
    pestopgr->SetLineColor(6);
    pestopgr->SetMarkerStyle(21);
    pestopgr->SetMarkerSize(1);
    pestopgr->SetLineWidth(1);
    mg->Add(pestopgr);
    TGraph* protonbb = new TGraph(pkinE.size(), pkinEarr, bbionparr);
    protonbb->SetTitle("proton Bethe Bloch prediction");
    protonbb->SetName("");
    protonbb->SetMarkerColor(1);
    protonbb->SetLineColor(1);
    protonbb->SetMarkerStyle(22);
    protonbb->SetMarkerSize(1);
    protonbb->SetLineWidth(1);
    mg->Add(protonbb);
    TGraph* lossp = new TGraph(pkinE.size(), pkinEarr, loss_p_emz);
    lossp->SetTitle("proton Geant4 high precision em physics (all losses)");
    lossp->SetName("");
    lossp->SetMarkerColor(9);
    lossp->SetLineColor(9);
    lossp->SetMarkerStyle(24);
    lossp->SetMarkerSize(1);
    lossp->SetLineWidth(1);
    mg->Add(lossp);
    //   pestopgr->GetXaxis()->SetTitle("E [MeV/c^{2}]");scanploss2.log
    //   pestopgr->GetYaxis()->SetTitle("dEdx [MeV cm^{2}/g]");
    //pestopgr->Draw("SAME");
    TGraph* mup = new TGraph(pkinE.size(), pkinEarr, dEdx_mup_emz);
    mup->SetTitle("#mu^{+} Geant4 high precision em physics (visible)");
    mup->SetName("");
    mup->SetMarkerColor(7);
    mup->SetLineColor(7);
    mup->SetMarkerStyle(22);
    mup->SetMarkerSize(1);
    mup->SetLineWidth(1);
    mg->Add(mup);
    //
    TGraph* lossmup = new TGraph(pkinE.size(), pkinEarr, loss_mup_emz);
    lossmup->SetTitle("#mu^{+} Geant4 high precision em physics (all losses)");
    lossmup->SetName("");
    lossmup->SetMarkerColor(16);
    lossmup->SetLineColor(16);
    lossmup->SetMarkerStyle(22);
    lossmup->SetMarkerSize(1);
    lossmup->SetLineWidth(1);
    mg->Add(lossmup);
    //

    //
    TGraph* mupbb = new TGraph(pkinE.size(), pkinEarr, bbionarr);
    mupbb->SetTitle("#mu^{+} Bethe Bloch prediction");
    mupbb->SetName("");
    mupbb->SetMarkerColor(2);
    mupbb->SetLineColor(2);
    mupbb->SetMarkerStyle(22);
    mupbb->SetMarkerSize(1);
    mupbb->SetLineWidth(1);
    mg->Add(mupbb);
    //
    TGraph* muppdg = new TGraph(pdgkinE.size(), pdgkinEarr, pdgdEdxarr);
    muppdg->SetTitle("#mu^{+} PDG");
    muppdg->SetName("");
    muppdg->SetMarkerColor(3);
    muppdg->SetLineColor(3);
    muppdg->SetMarkerStyle(23);
    muppdg->SetMarkerSize(1);
    muppdg->SetLineWidth(1);
    mg->Add(muppdg);



    mg->Draw("AC*");
    TLegend *leg = c->BuildLegend(.55, .55, 0.9, .85);
    leg->Draw();
}
