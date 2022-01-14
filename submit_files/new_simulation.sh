#!/bin/bash
#-------------input parameters--------------------------------------------------
cp simulation${1}.sh ./simulation${2}.sh
cp simulation${1}.sub ./simulation${2}.sub
cp simulation${1}.tmp ./simulation${2}.tmp

echo "Simulation draft ${2} has been created"
