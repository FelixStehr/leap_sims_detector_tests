How to use the submit files:

1) If you need a job_list.txt check out the job_list.py skript to generate on
2) Use new_simulation.sh to generate a draft of the .sub .tmp and .sh files from
   a simulation which is already in this directory
   (./new_simulation.sh <oldsimnumber> <newsimnumber>)
3) Edit your files as you like
4) Use move_simulation.sh to move the files to the dust repository, if you do not have a job_list.txt comment it in the script
   (./move_simulation.sh <simnumber>)
5) Use submit_simulation.sh to submit your simulation, results are saved in the dust repository.
   (./submit_simulation.sh <simnumber>)
