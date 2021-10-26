#!/bin/bash
outType=bunch
corethick=75
convthick=1.75
eKin=${2}
NBunch=100000
outFile=NBunch_${NBunch}_conv_${convthick}mm_core_${corethick}mm_E_${eKin}MeV_PrId${1}.root

#set the environment
source /cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc10-opt/setup.sh

# change parameters in macro
sed  "s/eKin/${eKin}/g" simulation1.tmp > test${1}.mac
sed -i  "s/corethick/$corethick/g" test${1}.mac
sed -i  "s/convthick/$convthick/g" test${1}.mac
sed -i  "s/NBunch/$NBunch/g" test${1}.mac

# run executable with macro and result file
./leap_sims -m test${1}.mac -f $outFile -t $outType -v PolCal

mv run0_$outFile /nfs/dust/ilc/user/fstehr/leap_sims/results/simulation1/run0_$outFile
mv run1_$outFile /nfs/dust/ilc/user/fstehr/leap_sims/results/simulation1/run1_$outFile

mv test${1}.mac /nfs/dust/ilc/user/fstehr/leap_sims/macros/test${1}.mac
