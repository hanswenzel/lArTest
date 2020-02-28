#!/bin/sh
# 
# File:   scan.sh
# Author: wenzel
#
# Created on Feb 19, 2020, 10:44:45 AM
#
for i in 1 1.2 1.4 1.7 2 2.5 3 3.5 4 4.5 5 5.5 6 7 8 9 10 12 14 17 20 25 30 35 40 45 50 55 60 70 80 90 100 120 140 170 200 250 300 350 400 450 500 550 600 700 800 900 1000 1200 1400 1700 2000 2500 3000 3500 4000 4500 5000 5500 6000 7000 8000 9000 10000 12000 14000 17000 20000 25000 30000 35000 40000 45000 50000 55000 60000 70000 80000 90000 100000 120000 140000 170000 200000 250000 300000 350000 400000 450000 500000 550000 600000 700000 800000 900000 1e+06 1.2e+06 1.4e+06 1.7e+06 2e+06 2.5e+06 3e+06 3.5e+06 4e+06 4.5e+06 5e+06 5.5e+06 6e+06 7e+06 8e+06 9e+06 1e+07 1.2e+07 1.4e+07 1.7e+07 2e+07 2.5e+07 3e+07 3.5e+07 4e+07 4.5e+07 5e+07 5.5e+07 6e+07 7e+07 8e+07 9e+07 1e+08 1.2e+08 1.4e+08 1.7e+08 2e+08 2.5e+08 3e+08 3.5e+08 4e+08 4.5e+08 5e+08 5.5e+08 6e+08 7e+08 8e+08 9e+08 1e+09
do
rm -f run_mu_$i.mac
/bin/cat > run_mu+_$i.mac << EOF+ 
/run/initialize 
/random/setSeeds 7 38 
/gun/particle mu+ 
/gun/energy $i MeV
/gun/position 0. 0. -1. cm
/gun/direction 0. 0. 1.
/run/beamOn 30000
EOF+
./lArTest lAr_paddle.gdml run_mu+_$i.mac
mv hits.root hits_mu+_$i.root
./readHits hits_mu+_$i.root
mv hsimple.root hsimple_mu+_$i.root
done