#!/bin/bash

# energy of e-beam

PI=100
offset=5
eKin=100
Espread=0.1 #FWHM Energy spread
eneType=Gauss # Gauss for Gaussian energy distribution, Mono for mono-energetic
if  [ "$eneTyp" = "Mono" ]
then
  sigmaE=1
else
  sigmaE= bc <<< "${eKin}*${Espread}"
fi

# beam geometry
sourceType=Point #Beam or Point
if [ "$sourceType" = "Beam" ]
then
  spotSize=0.001 # mm
  div=0.01 # radian
else
  spotSize=0.0 # mm
  div=0.0 # radian
fi


PInew= bc <<< "$PI + $offset"
echo $spotSize
echo $div
echo $sigmaE
echo $PInew
