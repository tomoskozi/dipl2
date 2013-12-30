#!/usr/bin/python

import subprocess
import sys
import optparse
import csv

# command line options
parser = optparse.OptionParser(usage="usage: avg.py [otpions]")
parser.add_option("-d", "--dir",
                  action="store", dest="dir", default="./",
                  help="")
parser.add_option("-n", "--num",
                  action="store", dest="num", default=10,
                  help="")
parser.add_option("-o", "--output",
                  action="store", dest="out", default="avg_window_delay.csv",
                  help="")
parser.add_option("-u", "--output2",
                  action="store", dest="out2", default="avg_gen_delay.csv",
                  help="")

(opts, args) = parser.parse_args(sys.argv)

files = [ opts.dir + "window_delay00.csv", 
          opts.dir + "window_delay01.csv", 
          opts.dir + "window_delay02.csv", 
          opts.dir + "window_delay03.csv", 
          opts.dir + "window_delay04.csv", 
          opts.dir + "window_delay05.csv", 
          opts.dir + "window_delay06.csv", 
          opts.dir + "window_delay07.csv", 
          opts.dir + "window_delay08.csv", 
          opts.dir + "window_delay09.csv"]

gen_files = [ opts.dir + "gen_delay00.csv", 
          opts.dir + "gen_delay01.csv", 
          opts.dir + "gen_delay02.csv", 
          opts.dir + "gen_delay03.csv", 
          opts.dir + "gen_delay04.csv", 
          opts.dir + "gen_delay05.csv", 
          opts.dir + "gen_delay06.csv", 
          opts.dir + "gen_delay07.csv", 
          opts.dir + "gen_delay08.csv", 
          opts.dir + "gen_delay09.csv"]

avg = { }
for i in range(0, 32):
    avg[i] = [ 0, 0, 0, 0, 0, 0, 0, 0 ]

fCnt = 0
for f in files:
    if fCnt >= int(opts.num):
        break
    else:
        fCnt = fCnt + 1
    try:
        with open(f, "r") as csvfile:
            reader = csv.reader(csvfile, delimiter=';', quotechar='#')

            index = -1
            for row in reader:
                index = index + 1
                for e in range(0, 8):
                    avg[index][e] = avg[index][e] + float(row[e])
            #print(avg[1])
    except IOError as e:
        print("Unable to open file: " + f)

for i in range(0, 32):
    for j in range(0, 8):
        avg[i][j] = avg[i][j] / int(opts.num)

f = open(opts.out, "w")
for i in avg:
    f.write(str(int(avg[i][0])) + ";" + str(int(avg[i][1]))
        + ";" + str(avg[i][2]) + ";" + str(avg[i][3])
        + ";" + str(avg[i][4]) + ";" + str(avg[i][5]) 
        + ";" + str(avg[i][6]) + ";" + str(avg[i][7]) + "\n")
f.close()

fCnt = 0
genAvg = { }
for f in gen_files:
    if fCnt >= int(opts.num):
        break
    else:
        fCnt = fCnt + 1
    try:
        with open(f, "r") as csvfile:
            reader = csv.reader(csvfile, delimiter=';', quotechar='#')

            for row in reader:
                if int(row[0]) in genAvg:
                    genAvg[int(row[0])][0] = genAvg[int(row[0])][0] + int(row[1])
                    genAvg[int(row[0])][1] = genAvg[int(row[0])][1] + float(row[2])
                else:
                    genAvg[int(row[0])] = [ int(row[1]), float(row[2]) ]
    except IOError as e:
        print("Unable to open file: " + f)

f = open(opts.out2, "w")
for i in genAvg:
    f.write(str(i) + ";" 
        + str(float(genAvg[i][0]) / float(opts.num)) + ";" 
        + str(genAvg[i][1] / float(opts.num)) + "\n")
f.close()
