#!/bin/sh
# 
# File:   scan.sh
# Author: wenzel
#
for i in   1.000E+02  1.250E+02  1.500E+02  1.750E+02  2.000E+02  2.250E+02  2.500E+02  2.750E+02  3.000E+02  3.500E+02  4.000E+02  4.500E+02  5.000E+02  5.500E+02  6.000E+02  6.500E+02  7.000E+02  7.500E+02  8.000E+02  8.500E+02  9.000E+02  9.500E+02  1.000E+03  1.500E+03  2.000E+03  2.500E+03  3.000E+03  4.000E+03  5.000E+03  6.000E+03  7.000E+03  8.000E+03  9.000E+03  1.000E+04
do 
rm -f run_p_$i.mac

/bin/cat > run_p_$i.mac << EOF+ 
/run/initialize 
/random/setSeeds 7 38 
/gun/particle proton 
/gun/energy $i MeV
/gun/position 0. 0. 0. cm
/gun/direction 0. 0. 1.
/run/beamOn 10000
EOF+
./lArTest lAr_box.gdml run_p_$i.mac
mv hits.root hits_p_$i.root
./readHits hits_p_$i.root
mv hsimple.root hsimple_p_$i.root
done