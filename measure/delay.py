#!/usr/bin/python

import sys
import optparse
import csv
import copy
import math

# command line options
parser = optparse.OptionParser(usage="usage: coding.py [otpions]")
parser.add_option("-e", "--encode",
                  action="store", dest="enc", default="encoding.csv",
                  help="CSV encode")
parser.add_option("-d", "--decode",
                  action="store", dest="dec", default="decoding.csv",
                  help="CSV decode")
parser.add_option("-b", "--b",
                  action="store", dest="b", default=100,
                  help="b")
parser.add_option("-o", "--output",
                  action="store", dest="out", default="delay.csv",
                  help="delay")
parser.add_option("-u", "--output2",
                  action="store", dest="out2", default="index_delay.csv",
                  help="delay")

(opts, args) = parser.parse_args(sys.argv)

delay = { }
delayByIndex = { }
deltaByIndex = { }
generationByIndex = { }
index = -1
prevSn = -1
delta = 0

avgDelta = 0.0
sumDelta = 0

start = -1
try:
    with open(opts.dec, "r") as csvfile:
        reader = csv.reader(csvfile, delimiter=';', quotechar='#')

        for row in reader:
            if start < 0:
                start = int(row[1])
            #if int(row[1]) != 0:
            delay[int(row[1])] = int(row[2])
except IOError as e:
    print("Unable to open file: " + dec)

try:
    with open(opts.enc, "r") as csvfile:
        reader = csv.reader(csvfile, delimiter=';', quotechar='#')
        
        for row in reader:
            #print(row[1])
            if int(row[1]) != 0:
                #print(row)
                if int(row[1]) in delay:
                    #print(row[2] + "-" + str(delay[int(row[1])]) + "\n")
                    delay[int(row[1])] = delay[int(row[1])] - int(row[2])
                    if index < 0:
                        prevSn = int(row[1])
                        index = 0
                        delta = 0
                    else:
                        if int(row[1]) < start and prevSn > start:
                            delta = (0xffff - prevSn + int(row[1]))
                            index = index + delta
                        else:
                            delta = (int(row[1]) - prevSn)
                            index = index + delta
                            prevSn = int(row[1])
                    avgDelta = avgDelta + delta
                    sumDelta = sumDelta + 1
                    delayByIndex[index] = delay[int(row[1])]
                    generationByIndex[index] = int(row[0])
                    #deltaByIndex[index] = delta
except IOError as e:
    print("Unable to open file: " + enc)

f = open(opts.out, "w")
#print(delay)
for i in delay:
    if delay[i] >= 0:
        f.write(str(i) + ";" + str(delay[i]) + "\n")
f.close()

delta = -1
prev = 0
for i in delayByIndex:
    if delayByIndex[i] >= 0:
        delta = i - prev
        prev = i
        deltaByIndex[i] = delta

f = open(opts.out2, "w")
#print(delayByIndex)
for i in delayByIndex:
    if delayByIndex[i] >= 0 and delayByIndex[i] < opts.b    \
        and deltaByIndex[i] < opts.b and deltaByIndex[i] > 0:
        f.write(str(i) + ";" + str(delayByIndex[i]) + ";" + str(deltaByIndex[i]) 
            + ";" + str(generationByIndex[i]) + "\n")
f.close()

#print(str(avgDelta / sumDelta))

prev = -1
avgDelta = 0.0
sumDelta = 0
for i in delay:
    if prev < 0:
        prev = i
    else:
        avgDelta = avgDelta + (i - prev)
        prev = i
        sumDelta = sumDelta + 1

#print(str(avgDelta / sumDelta))
