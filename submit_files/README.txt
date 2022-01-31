How to use the submit files:

1) Use new_simulation.sh to generate a draft of the .sub .tmp and .sh files from
   a simulation which is already in this directory. There will be a job_list.txt created as well check out the job_list.py script.
   (./new_simulation.sh <oldsimnumber> <newsimnumber>)
2) Edit your files as you like
3) Use move_simulation.sh to move the files to the dust repository, if you do not have a job_list.txt comment it in the script
   (./move_simulation.sh <simnumber>)
4) Use submit_simulation.sh to submit your simulation, results are saved in the dust repository.
   (./submit_simulation.sh <simnumber>)
