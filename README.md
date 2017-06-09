# lArTest


                                  
 lArTest: A Geant4 application to study and profile  
          simulation of physics processes relevant 
          to liquid Ar TPCs

 Author: Hans Wenzel, Fermilab


Prerequisites: Geant 4 and ROOT installed and the runtime properly initialized.  
cmake 


Checking the code out from github:
git clone https://hanswenzel@github.com/hanswenzel/lArTest.git

Build Instructions:


For an out of source build do:

mkdir lArTest-build

cd lArTest-build

cmake ../lArTest/ -DCMAKE_INSTALL_PREFIX=../lArTest-install

make install

to run 

cd ../lArTest-install/bin


./lArTest tinylArBox.gdml muons_tiny.in

to analyse the data in the scripts directory you find Ana.C

in root do:

root [0] .L Ana.C

root [1] Ana()



