#! /bin/bash

drop=0
repeat=0
sysOff=""
fbOff=""
repeatAfterOver=""

# process command line arguments
OPTIND=1
while getopts "d:r:hsfa" opt; do
    case $opt in
    d) drop=$OPTARG;;
    r) repeat=$OPTARG;;
    s) sysOff="-s";;
    f) fbOff="-f";;
    a) repeatAfterOver="-a";;
    h) echo "usage: "; exit;;
    esac
done
shift $(($OPTIND - 1))

if [[ $# -gt 0 ]]; then
    args=1
    arg=$@
fi

raspivid --height 1024 --width 1024 -fps 15 -b 10000000 -n -t 0 -o - | sudo testProgs/testH264VideoStreamer -d $drop -r $repeat $sysOff $fbOff $repeatAfterOver

