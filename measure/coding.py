#!/usr/bin/python

import subprocess
import sys
import optparse

# command line options
parser = optparse.OptionParser(usage="usage: coding.py [otpions]")
parser.add_option("-i", "--input",
                  action="store", dest="input", default="delay.csv",
                  help="CSV delay")
parser.add_option("-j", "--jinput",
                  action="store", dest="jinput", default=None,
                  help="CSV delay")
parser.add_option("-o", "--output",
                  action="store", dest="out", default="delay.eps",
                  help="delay")

(opts, args) = parser.parse_args(sys.argv)

# open gnuplot
gp = subprocess.Popen('/usr/bin/gnuplot', stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=None)

# set up plotting
gp.stdin.write("reset\n")
gp.stdin.write("set datafile separator ';'\n")
#gp.stdin.write("set terminal pdf\n")
gp.stdin.write("set terminal postscript eps monochrome dashed 'Helvetica' 22\n")
#gp.stdin.write("set terminal postscript eps colour solid\n")
#self.gp.stdin.write("set grid\n")
gp.stdin.write("set output '" + opts.out + "\n")    

# set up diagram
#gp.stdin.write("set title 'Network Coding Delay'\n")
gp.stdin.write("set xlabel 'Degrees of freedom'\n")
gp.stdin.write("set ylabel 'Milliseconds'\n")
gp.stdin.write("set y2label 'Sequence number delta'\n")
gp.stdin.write("set ytics nomirror\n")
gp.stdin.write("set y2tics\n")
gp.stdin.write("set xrange [0:31]\n")
gp.stdin.write("set yrange [0:70]\n")
gp.stdin.write("set y2range [0:6]\n")
#gp.stdin.write("set y2tics add ('I slice' 1)\n")
#gp.stdin.write("set y2tics add ('P slice' 0)\n")
gp.stdin.write("set xtics nomirror\n")
gp.stdin.write("set key right under\n")

if opts.jinput is None:
    # plot
    gp.stdin.write("plot '" + opts.input + "'" 
        + " using 1:3:5:6 axes x1y1 title 'Coding delay' with errorlines,"
        + " '" + opts.input + "'"
        + " using 1:4:7:8 axes x1y2 title 'Sequence number delta' with errorlines\n")
else:
    gp.stdin.write("plot '" + opts.input + "'" 
        + " using 1:3:5:6 axes x1y1 title 'Delay (redundancy 5)' with errorlines,"
        + " '" + opts.input + "'"
        + " using 1:4:7:8 axes x1y2 title 'Delta (redundancy 5)' with errorlines,"
        + "'" + opts.jinput + "'" 
        + " using 1:3:5:6 axes x1y1 title 'Delay (redundancy 0)' with errorlines,"
        + " '" + opts.jinput + "'"
        + " using 1:4:7:8 axes x1y2 title 'Delta (redundancy 0)' with errorlines\n")

# quit
gp.stdin.write("q\n")
out = gp.communicate()[0]

