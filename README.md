# lArTes
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

Prerequisites: Geant 4 and ROOT installed and the runtime properly initialized.  
cmake 


Checking the code out from github:
git clone https://hanswenzel@github.com/hanswenzel/lArTest

Build Instructions:
For an out of source build do:

mkdir lArTest-build
cd lArTest-build
cmake ../lArTest/ -DCMAKE_INSTALL_PREFIX=../lArTest-install
make install

to run 
cd ../lArTest-install/bin
./lArTest muons.in

to analyse the data in the scripts directory you find Ana.C
in root:
root [0] .L Ana.C
root [1] Ana()



