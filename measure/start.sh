#! /bin/bash

# process command line arguments
clean=0
cleanCmd=""
plotOnly=0
samples=2
while getopts "cp" opt; do
    case $opt in
    c) clean=1; cleanCmd="-c";;
    p) plotOnly=1;;
    h) echo ""; exit;;
    esac
done
shift $(($OPTIND - 1))

if [[ $# -gt 0 ]]; then
    args=1
    arg=$@
fi

M_CNT=5
MESUREMENTS=(
    "00"
    "01"
    "05"
    "10"
    "20"
)

for ((i=0; i<$M_CNT; i+=1)); do
    if [[ $plotOnly -eq 0 ]]; then
        # online
        ./plot.sh -d online/${MESUREMENTS[$i]}rxx/ $cleanCmd -n $samples
        ./plot.sh -d online/${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        ./plot.sh -d online/o${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        # full vector
        ./plot.sh -d full_vector/${MESUREMENTS[$i]}rxx/ $cleanCmd -n $samples
        ./plot.sh -d full_vector/${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        ./plot.sh -d full_vector/o${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        # sliding window
        ./plot.sh -d sliding_window/${MESUREMENTS[$i]}rxx/ $cleanCmd -n $samples
        ./plot.sh -d sliding_window/${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        ./plot.sh -d sliding_window/o${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        # reed-solomon
        ./plot.sh -d reed_solomon/${MESUREMENTS[$i]}rxx/ $cleanCmd -n $samples
        ./plot.sh -d reed_solomon/${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        ./plot.sh -d reed_solomon/o${MESUREMENTS[$i]}r05/ $cleanCmd -n $samples
        # no coding
        ./plot.sh -d no_coding/${MESUREMENTS[$i]}r00/ $cleanCmd -n $samples
    fi

    if [[ $clean -gt 0 ]]; then
        rm -f "results/static_delay"${MESUREMENTS[$i]}".eps"
        rm -f "results/dynamic_delay"${MESUREMENTS[$i]}".eps"
        rm -f "results/in_order_delay"${MESUREMENTS[$i]}".eps"
    else
        echo "Plotting results/static_delay"${MESUREMENTS[$i]}".eps & results/dynamic_delay"${MESUREMENTS[$i]}".eps"
        ./coding3.py --online "online/"${MESUREMENTS[$i]}"r05/avg_window_delay.csv"             \
                     --onlineAdaptive "online/"${MESUREMENTS[$i]}"rxx/avg_window_delay.csv"     \
                     --full "full_vector/"${MESUREMENTS[$i]}"r05/avg_window_delay.csv"          \
                     --fullAdaptive "full_vector/"${MESUREMENTS[$i]}"rxx/avg_window_delay.csv"  \
                     --sw "sliding_window/"${MESUREMENTS[$i]}"r05/avg_window_delay.csv"         \
                     --swAdaptive "sliding_window/"${MESUREMENTS[$i]}"rxx/avg_window_delay.csv" \
                     --rs "reed_solomon/"${MESUREMENTS[$i]}"r05/avg_window_delay.csv"           \
                     --rsAdaptive "reed_solomon/"${MESUREMENTS[$i]}"rxx/avg_window_delay.csv"   \
                     --no "no_coding/"${MESUREMENTS[$i]}"r00/avg_window_delay.csv"              \
                     -o "results/static_delay"${MESUREMENTS[$i]}".eps"                          \
                     -u "results/dynamic_delay"${MESUREMENTS[$i]}".eps"                         \
                     -t ${MESUREMENTS[$i]}"% losses"

        ./plot_in_order.py --online "online/"o${MESUREMENTS[$i]}"r05/avg_window_delay.csv"      \
                     --full "full_vector/"o${MESUREMENTS[$i]}"r05/avg_window_delay.csv"         \
                     --sw "sliding_window/"o${MESUREMENTS[$i]}"r05/avg_window_delay.csv"        \
                     --rs "reed_solomon/"o${MESUREMENTS[$i]}"r05/avg_window_delay.csv"          \
                     -o "results/in_order_delay"${MESUREMENTS[$i]}".eps"                        \
                     -t ${MESUREMENTS[$i]}"% losses"
    fi 
done


#

#./coding3.py --online 01r05/avg_window_delay.csv  --onlineAdaptive online/01rxx/avg_window_delay.csv --full full/s01r05/avg_window_delay.csv --fullAdaptive full2/01rxx/avg_window_delay.csv --sw slide_red/01r05/avg_window_delay.csv --swAdaptive slide/01rxx/avg_window_delay.csv --rs rrs/01r05/avg_window_delay.csv --rsAdaptive ars2/01rxx/avg_window_delay.csv --no no/01r00/avg_window_delay.csv -t "1% losses" -o delay_adaptive_01.eps

#./coding3.py --online 05r05/avg_window_delay.csv  --onlineAdaptive online/05rxx/avg_window_delay.csv --full full/s05r05/avg_window_delay.csv --fullAdaptive full2/05rxx/avg_window_delay.csv --sw slide_red/05r05/avg_window_delay.csv --swAdaptive slide/05rxx/avg_window_delay.csv --rs rrs/05r05/avg_window_delay.csv --rsAdaptive ars2/05rxx/avg_window_delay.csv --no no/05r00/avg_window_delay.csv -t "5% losses" -o delay_adaptive_05.eps

#./coding3.py --online 10r05/avg_window_delay.csv  --onlineAdaptive online/10rxx/avg_window_delay.csv --full full/s10r05/avg_window_delay.csv --fullAdaptive full2/10rxx/avg_window_delay.csv --sw slide_red/10r05/avg_window_delay.csv --swAdaptive slide/10rxx/avg_window_delay.csv --rs rrs/10r05/avg_window_delay.csv --rsAdaptive ars2/10rxx/avg_window_delay.csv --no no/10r00/avg_window_delay.csv -t "10% losses" -o delay_adaptive_10.eps

#./coding3.py --online 20r05/avg_window_delay.csv  --onlineAdaptive online/20rxx/avg_window_delay.csv --full full/s20r05/avg_window_delay.csv --fullAdaptive full2/20rxx/avg_window_delay.csv --sw slide_red/20r05/avg_window_delay.csv --swAdaptive slide/20rxx/avg_window_delay.csv --rs rrs/20r05/avg_window_delay.csv --rsAdaptive ars2/20rxx/avg_window_delay.csv --no no/20r00/avg_window_delay.csv -t "20% losses" -o delay_adaptive_20.eps

