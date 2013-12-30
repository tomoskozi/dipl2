#!/usr/bin/python

import subprocess
import sys
import optparse

# command line options
parser = optparse.OptionParser(usage="usage: coding.py [otpions]")
parser.add_option("-n", "--no",
                  action="store", dest="no", default=None,
                  help="CSV delay")
parser.add_option("-i", "--online",
                  action="store", dest="online", default="delay.csv",
                  help="CSV delay")
parser.add_option("-x", "--onlineAdaptive",
                  action="store", dest="onlineAdaptive", default="delay.csv",
                  help="CSV delay")
parser.add_option("-j", "--full",
                  action="store", dest="full", default=None,
                  help="CSV delay")
parser.add_option("-y", "--fullAdaptive",
                  action="store", dest="fullAdaptive", default=None,
                  help="CSV delay")
parser.add_option("-z", "--sw",
                  action="store", dest="sw", default=None,
                  help="CSV delay")
parser.add_option("-s", "--swAdaptive",
                  action="store", dest="swAdaptive", default=None,
                  help="CSV delay")
parser.add_option("-k", "--rs",
                  action="store", dest="rs", default=None,
                  help="CSV delay")
parser.add_option("-l", "--rsAdaptive",
                  action="store", dest="rsAdaptive", default=None,
                  help="CSV delay")
parser.add_option("-t", "--title",
                  action="store", dest="title", default="0% channel loss",
                  help="CSV delay")
parser.add_option("-o", "--output",
                  action="store", dest="out", default="static_delay.eps",
                  help="delay")
parser.add_option("-u", "--output2",
                  action="store", dest="out2", default="dynamic_delay.eps",
                  help="delay2")

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

if opts.sw is not None:
    gp.stdin.write("plot '" + opts.full + "'" 
        + " using 1:3 axes x1y1 title 'Full-Vector Network Coding' with lines,"
        + " '" + opts.online + "'"
        + " using 1:3 axes x1y1 title 'Online Network Coding' with lines,"
        + " '" + opts.sw + "'"
        + " using 1:3 axes x1y1 title 'Sliding Window Newtork Coding' with lines,"
        + " '" + opts.rs + "'"
        + " using 1:3 axes x1y1 title 'Reed-Solomon Coding' with lines,"
        + " '" + opts.no + "'"
        + " using 1:3 axes x1y1 title 'No Coding' with lines\n")

gp.stdin.write("set output '" + opts.out2 + "'\n")

gp.stdin.write("plot '" + opts.fullAdaptive + "'"
    + " using 1:3 axes x1y1 title 'Full-Vector Network Coding' with lines,"
    + " '" + opts.onlineAdaptive + "'"
    + " using 1:3 axes x1y1 title 'Online Network Coding' with lines,"
    + " '" + opts.swAdaptive + "'"
    + " using 1:3 axes x1y1 title 'Sliding-Window Network Coding' with lines,"
    + " '" + opts.rsAdaptive + "'"
    + " using 1:3 axes x1y1 title 'Reed-Solomon Coding' with lines,"
    + " '" + opts.no + "'"
    + " using 1:3 axes x1y1 title 'No coding' with lines\n")

# quit
gp.stdin.write("q\n")
out = gp.communicate()[0]

