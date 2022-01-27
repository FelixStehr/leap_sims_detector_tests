#!/bin/bash
#-------------input parameters--------------------------------------------------
cp previous_files/simulation${1}.sh ./simulation${2}.sh
cp previous_files/simulation${1}.sub ./simulation${2}.sub
cp previous_files/simulation${1}.tmp ./simulation${2}.tmp

echo "Simulation draft ${2} has been created"
