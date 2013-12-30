#!/usr/bin/python

import subprocess
import sys
import optparse

# command line options
parser = optparse.OptionParser(usage="usage: generations.py [otpions]")
parser.add_option("-i", "--sw32",
                  action="store", dest="sw32", default="delay.csv",
                  help="CSV delay")
parser.add_option("-j", "--sw8",
                  action="store", dest="sw8", default="delay.csv",
                  help="CSV delay")
parser.add_option("-k", "--sw16",
                  action="store", dest="sw16", default="delay.csv",
                  help="CSV delay")
parser.add_option("-l", "--rs32",
                  action="store", dest="rs32", default="delay.csv",
                  help="CSV delay")
parser.add_option("-m", "--rs8",
                  action="store", dest="rs8", default="delay.csv",
                  help="CSV delay")
parser.add_option("-n", "--rs16",
                  action="store", dest="rs16", default="delay.csv",
                  help="CSV delay")
parser.add_option("-o", "--output",
                  action="store", dest="out", default="static_delay.eps",
                  help="delay")

(opts, args) = parser.parse_args(sys.argv)

# open gnuplot
gp = subprocess.Popen('/usr/bin/gnuplot', stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=None)

# set up plotting
gp.stdin.write("reset\n")
gp.stdin.write("set datafile separator ';'\n")
#gp.stdin.write("set terminal pdf\n")
gp.stdin.write("set terminal postscript eps monochrome dashed 'Helvetica' 16\n")
#gp.stdin.write("set terminal postscript eps colour solid\n")
#self.gp.stdin.write("set grid\n")
gp.stdin.write("set output '" + opts.out + "'\n")    

# set up diagram
#gp.stdin.write("set title 'Decoding delay (" + opts.title  + ")'\n")
gp.stdin.write("set xlabel 'Degrees of freedom'\n")
gp.stdin.write("set ylabel 'Milliseconds'\n")
#gp.stdin.write("set y2label 'Packets'\n")
#gp.stdin.write("set ytics nomirror\n")
#gp.stdin.write("set y2tics\n")
gp.stdin.write("set xrange [0:31]\n")
gp.stdin.write("set yrange [0:70]\n")
#gp.stdin.write("set y2range [0:6]\n")
#gp.stdin.write("set y2tics add ('I slice' 1)\n")
#gp.stdin.write("set y2tics add ('P slice' 0)\n")
gp.stdin.write("set xtics nomirror\n")
gp.stdin.write("set key right under\n")

#if opts.no is None:
#    gp.stdin.write("plot '" + opts.online + "'" 
#        + " using 1:3 axes x1y1 title 'Online NC (5 redundancy)' with lines,"
#        + " '" + opts.onlineAdaptive + "'"
#        + " using 1:3 axes x1y1 title 'Online NC (adaptive)' with lines,"
#        + " '" + opts.full + "'"
#        + " using 1:3 axes x1y1 title 'Full NC (5 redundancy)' with lines\n")
#        #+ " '" + opts.fullAdaptive + "'"
#        #+ " using 1:3 axes x1y1 title 'Full  NC (adaptive)' with lines,"
#        #+ " '" + opts.rs + "'"
#        #+ " using 1:3 axes x1y1 title 'Reed-Solomon (" + opts.redundancy +  ")' with lines\n")
#else:

gp.stdin.write("plot '" + opts.sw8 + "'" 
    + " using 1:3 axes x1y1 title 'Network Coding (8)' with lines,"
    + " '" + opts.sw16 + "'"
    + " using 1:3 axes x1y1 title 'Network Coding (16)' with lines,"
    + " '" + opts.sw32 + "'"
    + " using 1:3 axes x1y1 title 'Network Coding (32)' with lines,"
    + " '" + opts.rs8 + "'"
    + " using 1:3 axes x1y1 title 'Reed-Solomon (8)' with lines,"
    + " '" + opts.rs16 + "'"
    + " using 1:3 axes x1y1 title 'Reed-Solomon (16)' with lines,"
    + " '" + opts.rs32 + "'"
    + " using 1:3 axes x1y1 title 'Reed-Solomon (32)' with lines\n")

# quit
gp.stdin.write("q\n")
out = gp.communicate()[0]

