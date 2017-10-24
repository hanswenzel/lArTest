# lArTest


                                  
 lArTest: A Geant4 application to study and profile  
          simulation of physics processes relevant 
          to liquid Ar TPCs

 Author: Hans Wenzel, Fermilab


Prerequisites: Geant 4 and ROOT installed and the runtime properly initialized,  cmake. 
cmake command for Geant4:
cmake -DCMAKE_INSTALL_PREFIX=~/geant4.10.03.p03_install -DGEANT4_USE_GDML=ON -DGEANT4_INSTALL_DATA=ON  -DGEANT4_USE_QT=ON -DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_USE_OPENGL_X11=ON ~/geant4.10.03.p03

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

lArTest can make use of multithreading in case we are using a geant 4 version which is compiled with multithreading enabled. The number of threads can be controled via the -t command line switch e.g. would run lArTest with 8 threads.

./lArTest tinylArBox.gdml muons_tiny.in -t 8

to analyse the data in the scripts directory you find Ana.C

in root do:

root [0] .L Ana.C

root [1] Ana()



