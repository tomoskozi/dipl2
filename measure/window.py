#!/usr/bin/python

import sys
import optparse
import csv
import copy
import math

# command line options
parser = optparse.OptionParser(usage="usage: window.py [otpions]")
parser.add_option("-i", "--input",
                  action="store", dest="delay", default="index_delay.csv",
                  help="CSV delay")
parser.add_option("-s", "--symbolSize",
                  action="store", dest="symSz", default=32,
                  help="Symbol Size")
parser.add_option("-b", "--bo",
                  action="store", dest="b", default=100,
                  help="bo")
parser.add_option("-x", "--maxSamples",
                  action="store", dest="max", default=200,
                  help="max samples")
parser.add_option("-o", "--output",
                  action="store", dest="out", default="window_delay.csv",
                  help="delay")
parser.add_option("-u", "--output2",
                  action="store", dest="out2", default="gen_window_delay.csv",
                  help="delay")

(opts, args) = parser.parse_args(sys.argv)

window = { }
delays = { }
packets = { }
samples = 0
i = 0
j = -1
generations = { }

for i in range(0, 32):
    window[i] = [ i, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
    delays[i] = { }
    packets[i] = { }

try:
    with open(opts.delay, "r") as csvfile:
        reader = csv.reader(csvfile, delimiter=';', quotechar='#')

        for row in reader:
            i = int(row[0]) % 32
            #i = (i + 1) % 32
            #print(str(row))
            if int(row[1]) >= int(opts.b):
                continue
            if (int(row[2]) < 0) or (int(row[2]) > int(opts.b)):
                continue
            #if (i % 32) == 0:
            #    samples = samples + 1
            # samples
            #print(window[i][1])
            if window[i][1] < opts.max:
                window[i][1] = window[i][1] + 1
                # delay
                window[i][2] = window[i][2] + int(row[1])
                # packet index delta
                window[i][3] = window[i][3] + float(row[2])
                #print(int(row[2]))
                # lossed packets
                window[i][8] = window[i][8] + (float(row[2]) - 1.0)
                # std deviation
                #window[i][4] = window[i][4] + int(row[1])
                # delays
                delays[i][window[i][1] - 1] = int(row[1])
                # packets
                packets[i][window[i][1] - 1] = int(row[2])

                if int(row[3]) in generations:
                    generations[int(row[3])][0] = generations[int(row[3])][0] + 1
                    generations[int(row[3])][1] = generations[int(row[3])][1] + int(row[1])
                    #print(generations[int(row[3])])
                else:
                    # samples, delay
                    generations[int(row[3])] = [ 1, int(row[1]) ]
except IOError as e:
    print("Unable to open file: " + opts.delay)

# calc average
for i in window:
    window[i][2] = window[i][2] / window[i][1]
    #print(window[i][3])
    window[i][3] = window[i][3] / float(window[i][1])
    window[i][8] = window[i][8] / float(window[i][1])

# average
#avg = 0
#for i in window:
#    avg = avg + window[i][2]
#avg = avg / opts.symSz
# calc sample deviation
for i in delays:
    for j in delays[i]:
        delays[i][j] = (delays[i][j] - window[i][2])**2
        packets[i][j] = (packets[i][j] - window[i][3])**2
# standard deviation
for i in delays:
    s = 0
    z = 0
    for j in delays[i]:
        s = s + delays[i][j]
        z = z + packets[i][j]
    window[i][4] = math.sqrt(s / window[i][1])
    window[i][6] = math.sqrt(z / window[i][1])
#print(window[0][4])
#for i in window:
    #window[i][4] = window[i][2] / window[i][2]
#    window[i][4] = window[i][4] / opts.max

# confidence interval
for i in window:
    window[i][5] = window[i][4] / math.sqrt(window[i][1])
    window[i][5] = window[i][5] * 1.645#1.9720
    window[i][7] = window[i][6] / math.sqrt(window[i][1])
    window[i][7] = window[i][7] * 1.645#1.9720
    #print(window[i][7]) 

f = open(opts.out, "w")
#print(delay)
#for i in window:
#    print(window[i][3])

for i in window:
    f.write(str(window[i][0]) + ";" + str(window[i][1])
        + ";" + str(window[i][2]) + ";" + str(window[i][3])
        + ";" + str(window[i][2] - window[i][5])
        + ";" + str(window[i][2] + window[i][5]) 
        + ";" + str(window[i][3] - window[i][7])
        + ";" + str(window[i][3] + window[i][7])
        + ";" + str(window[i][8]) + ";" + str(window[i][7]) + "\n")
f.close()

#generations
for i in generations:
    #print(generations[i])
    generations[i][1] = float(generations[i][1]) / float(generations[i][0])
f = open(opts.out2, "w")
for i in generations:
    f.write(str(i) + ";" + str(generations[i][0]) + ";" + str(generations[i][1]) + "\n")
f.close()

