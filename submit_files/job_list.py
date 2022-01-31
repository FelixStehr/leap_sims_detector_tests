# -*- coding: utf-8 -*-

#import ROOT
import sys
# import numpy as np
# import pandas as pd
# from os import listdir
import csv

name = f"job_list_sim{sys.argv[1]}.txt"

Xstart = -60
Xend = 60
dx = 5
Status = [["true"], ["false"]]
#Status=[["true","true"],["false", "false"],["true", "false"],["false", "true"]]
with open(name, "w") as f:
    writer = csv.writer(f)
    for n in range(len(Status)):
        for x in range(Xstart, Xend+1, dx):
            # Here I had to make x to an List do add it without changing the Status list ....
            X = [x]
            writer.writerow((Status[n]+X))
f.close()


# #write the shot information to one file without ziped list
# with open("TestCSV", "w") as f:
#     writer = csv.writer(f)
#     writer.writerow(("Nr", "Date", "Time", "Bunch Charge "))
#     for i in range(len(shotN)):
#         writer.writerow((shotN[i], Date[i], Time[i]), Charge)
# f.close()


# print(shotN)
# print(Date)
# print(Time)
