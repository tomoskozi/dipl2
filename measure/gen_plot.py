#!/usr/bin/python

import subprocess
import sys
import optparse

# command line options
parser = optparse.OptionParser(usage="usage: gen_plot.py [otpions]")
parser.add_option("-i", "--input",
                  action="store", dest="input", default="gen_delay00.csv",
                  help="CSV delay")
parser.add_option("-o", "--output",
                  action="store", dest="out", default="gen_delay.eps",
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
gp.stdin.write("set xlabel 'Generation IDs'\n")
gp.stdin.write("set ylabel 'Milliseconds'\n")
gp.stdin.write("set y2label 'Decoded symbols'\n")
gp.stdin.write("set ytics nomirror\n")
gp.stdin.write("set y2tics\n")
gp.stdin.write("set xrange [0:50]\n")
gp.stdin.write("set yrange [0:]\n")
gp.stdin.write("set y2range [0:]\n")
#gp.stdin.write("set y2tics add ('I slice' 1)\n")
#gp.stdin.write("set y2tics add ('P slice' 0)\n")
gp.stdin.write("set xtics nomirror\n")
gp.stdin.write("set key right under\n")

#plot
gp.stdin.write("plot '" + opts.input + "'" 
    + " using 1:3 axes x1y1 title 'Coding delay' with lines,"
    + " '" + opts.input + "'"
    + " using 1:2 axes x1y2 title 'Decoded symbols' with lines\n")

# quit
gp.stdin.write("q\n")
out = gp.communicate()[0]

