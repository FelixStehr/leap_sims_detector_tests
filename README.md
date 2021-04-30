# leap_sims

### How to run leap_sims_master  
1.  set the environment:  
  `source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/Geant4-env.sh`  
  `source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh`  

2.  make a build directory and change to the build directory:  
  `mkdir build`  
  `cd build`  

3. build the application:  
  `/cvmfs/sft.cern.ch/lcg/releases/LCG_97/CMake/3.14.3/x86_64-centos7-gcc8-opt/bin/cmake ../`  
  `make`  
4. run it in batch mode with macro:
  `./leap_sims  macfile.mac` 
  
### How to submit loads of jobs to the NAF
1. cd into the submit_jobs directory
2. There are three bash skripts (files with `.sh` ending) to start runs varying the energy of the incident electrons, the core thickness, or the thickness of the converter target. Edit them to set the parameters you want for the run.
3. to submit jobs for a run where the energy varies:
`condor_submit EkinRun.sub`   
4. watch the progress
`watch condor_q`
5. to stop watching simly press `ctrl` `c` 

  
  To run the analysis code you have to
  `source /cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc10-opt/setup.sh`
  when you are running it on the NAF
