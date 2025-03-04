#!/bin/bash
#-------------input parameters--------------------------------------------------
cp previous_files/simulation${1}.sh ./simulation${2}.sh
cp previous_files/simulation${1}.sub ./simulation${2}.sub
cp previous_files/simulation${1}.tmp ./simulation${2}.tmp

sed -i  "s/${1}/${2}/g" simulation${2}.sh
sed -i  "s/${1}/${2}/g" simulation${2}.sub

python3 job_list.py ${2}

echo "Simulation draft ${2} has been created"
