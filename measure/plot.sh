#! /bin/bash

# process command line arguments
directory="./"
clean=0
SAMPLES=10
MAX=200
B=200
while getopts "d:n:b:x:hc" opt; do
    case $opt in
    c) clean=1;;
    d) directory=$OPTARG;;
    n) SAMPLES=$OPTARG;;
    x) MAX=$OPTARG;;
    b) B=$OPTARG;;
    h) echo ""; exit;;
    esac
done
shift $(($OPTIND - 1))

if [[ $# -gt 0 ]]; then
    args=1
    arg=$@
fi

FILES=(
    "00.csv"
    "01.csv"
    "02.csv"
    "03.csv"
    "04.csv"
    "05.csv"
    "06.csv"
    "07.csv"
    "08.csv"
    "09.csv"
)

# clean
if [[ $clean -gt 0 ]]; then
    echo "Removing generated files..."
    for ((i=0; i<$SAMPLES; i+=1)); do
        echo ${directory}"delay"${FILES[$i]}
        rm -f ${directory}"delay"${FILES[$i]}
        echo ${directory}"index_delay"${FILES[$i]}
        rm -f ${directory}"index_delay"${FILES[$i]}
        echo ${directory}"window_delay"${FILES[$i]}
        rm -f ${directory}"window_delay"${FILES[$i]}
        echo ${directory}"gen_delay"${FILES[$i]}
        rm -f ${directory}"gen_delay"${FILES[$i]}
    done
    echo ${directory}"avg_window_delay.csv"
    rm -f ${directory}"avg_window_delay.csv"
    echo ${directory}"avg_gen_delay.csv"
    rm -f ${directory}"avg_gen_delay.csv"
    echo ${directory}"delay.eps"
    rm -f ${directory}"delay.eps"
    echo ${directory}"gen_delay.eps"
    rm -f ${directory}"gen_delay.eps"
# generate
else
    for ((i=0; i<$SAMPLES; i+=1)); do
        # delay
        echo "Computing "${directory}"delay"${FILES[$i]}" & "${directory}"indexed_delay"${FILES[$i]}
        ./delay.py -e ${directory}"encoding"${FILES[$i]} -d ${directory}"decoding"${FILES[$i]} \
            -o ${directory}"delay"${FILES[$i]} -u ${directory}"index_delay"${FILES[$i]} -b $B
        # window
        echo "Computing "${directory}"window_delay"${FILES[$i]}" & "${directory}"gen_delay"${FILES[$i]}
        ./window.py -i ${directory}"index_delay"${FILES[$i]} -b $B -x $MAX \
            -o ${directory}"window_delay"${FILES[$i]} -u ${directory}"gen_delay"${FILES[$i]} 
    done
    # average
    echo "Computing "${directory}"avg_window_delay.csv & "${directory}"avg_gen_delay.csv"
    ./avg.py -d $directory -n $SAMPLES -o ${directory}"avg_window_delay.csv" -u ${directory}"avg_gen_delay.csv"
    # plot
    echo "Plotting "${directory}"delay.eps"
    ./coding.py -i ${directory}"avg_window_delay.csv" -o ${directory}"delay.eps"
    echo "Plotting "${directory}"gen_delay.eps"
    ./gen_plot.py -i ${directory}"avg_gen_delay.csv" -o ${directory}"gen_delay.eps"
fi

