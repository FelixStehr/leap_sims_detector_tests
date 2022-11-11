#!/bin/bash
#-------------input parameters--------------------------------------------------
#simulation number
simNo=65
# output Type
outType=bunch #single or bunch
BeamlineStatus=off
#polarimeter geometry
SetUp=Cal # Pol/Cal/PolCal; Polarimeter, Calorimeter, or both
#corethick=75 # thickness of magnet core in mm
#convthick=1.75 # thickness of converter target in mm
# energy of e-beam
# ypos=195.0055
rCollimator=5
CaloDistance=56
SFStatus=false  # is the Scintillator finger in the the beam or not? true/false
StrawStatus=false
LanexStatus=false
Xposition=0.0
eKin=${2}
Espread=0.1 #FWHM Energy spread
eneType=Gauss # Gauss for Gaussian energy distribution, Mono for mono-energetic
#if [ "$eneType" = "Mono" ]
#then
#  sigmaE=0
#else
  #sigmaE= bc <<< "${eKin}*${Espread}"
#fi
sigmaE=0.0    #2.547
# beam geometry
NBunch=${3}
sourceType=Point #Beam or Point
if [ "$sourceType" = "Beam" ]
then
  spotSize=0.001 # mm
  div=0.003
else
  spotSize=0.0 # mm
  div=0.0 # radian
fi

#beam polarization
ePol=1
#-------------------------------------------------------------------------------

#Name of root output file
#outFile=${SetUp}_${outType}_NBunch_${NBunch}_E_${eKin}_pm_${sigmaE}MeV_sz_${spotSize}_mm_div_${div}_rad_ePol_${ePol}_conv_${convthick}mm_core_${corethick}mm_rCollimator_${rCollimator}_mm_CaloDistance_${CaloDistance}_cm_PrId${1}.root
outFile=${SetUp}_${outType}_NBunch_${NBunch}_E_${eKin}_pm_${sigmaE}MeV_sz_${spotSize}_mm_div_${div}_rad_ePol_PdID${1}
#set the environment
source /cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc10-opt/setup.sh

# change parameters in macro
sed  "s/eKin/${eKin}/g" simulation${simNo}.tmp > test${1}.mac
#sed -i  "s/ypos/$ypos/g" test${1}.mac
#sed -i  "s/corethick/$corethick/g" test${1}.mac
#sed -i  "s/convthick/$convthick/g" test${1}.mac
sed -i  "s/SFStatus/$SFStatus/g" test${1}.mac
sed -i  "s/LanexStatus/$LanexStatus/g" test${1}.mac
sed -i  "s/StrawStatus/$StrawStatus/g" test${1}.mac

sed -i  "s/Xposition/$Xposition/g" test${1}.mac
sed -i  "s/NBunch/$NBunch/g" test${1}.mac
sed -i  "s/sigmaE/$sigmaE/g" test${1}.mac
#sed -i  "s/polDeg/$ePol/g" test${1}.mac
sed -i  "s/spotSize/$spotSize/g" test${1}.mac
sed -i  "s/rCollimator/$rCollimator/g" test${1}.mac
sed -i  "s/CaloDistance/$CaloDistance/g" test${1}.mac
sed -i  "s/div/$div/g" test${1}.mac

# run executable with macro and result file
./leap_sims -m test${1}.mac -f $outFile.root -t $outType -v $SetUp -b $BeamlineStatus

mkdir /nfs/dust/ilc/user/fstehr/leap_sims/results/simulation_$simNo
mv run0_$outFile.root /nfs/dust/ilc/user/fstehr/leap_sims/results/simulation_$simNo/run0_${outFile}.root
# mv run0_$outFile.root.gdml /nfs/dust/ilc/user/fstehr/leap_sims/results/simulation_$simNo/run0_${outFile}.gdml

#mv run1_$outFile /nfs/dust/ilc/user/fstehr/leap_sims/results/simulation_$simNo/run1_$outFile

mv test${1}.mac /nfs/dust/ilc/user/fstehr/leap_sims/macros/test${1}.mac
