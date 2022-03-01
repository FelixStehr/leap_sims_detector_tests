#!/bin/bash
#-------------input parameters--------------------------------------------------
cp simulation${1}.sh /nfs/dust/ilc/user/fstehr/leap_sims/submit_files
cp simulation${1}.sub /nfs/dust/ilc/user/fstehr/leap_sims/submit_files
cp simulation${1}.tmp /nfs/dust/ilc/user/fstehr/leap_sims/submit_files
cp job_list_sim${1}.txt /nfs/dust/ilc/user/fstehr/leap_sims/submit_files
cp ../build/leap_sims /nfs/dust/ilc/user/fstehr/leap_sims/submit_files

mv simulation${1}.sh ./previous_files
mv simulation${1}.sub ./previous_files
mv simulation${1}.tmp ./previous_files
mv job_list_sim${1}.txt ./previous_files

echo "Simulation ${1} has been moved as well as leap_sims executable"

# cd /nfs/dust/ilc/user/fstehr/leap_sims/submit_files
# condor_submit simulation${1}.sub
