#!/bin/bash

# start Runtime
start=$(date +%s.%N)

validCommand=0
if (( $# < 1 || $# > 3 )); then
  validCommand=-1
fi

if [[ $1 == "--help" ]]; then
  if (( $# != 1 )); then
    validCommand=-1
  else
    echo "Usage: $0 [path] <command> [arguments]"
    echo "$0 [path] histo <max|src|real>"
    echo "$0 [path] leaks \"Facility ID\""
    exit 0
  fi
fi

case $2 in #PARSE FIRST ARGUMENT
  "histo")
    # check file existence
    if [[ -f "./histo/max.c" ]]; then
        gcc -o "histo/max" "histo/max.c" "histo/AVL.c" "utility/utility.c" -lm
    else
        echo "Error: The file histo/max.c does not exist"
        exit 1
    fi
    if (( $# != 3 )); then
      validCommand=-1
    else
      case $3 in #PARSE SECOND ARGUMENT
        "max")
          validCommand=1
          ;;
        "src")
          validCommand=2
          ;;
        "real")
          validCommand=3
          ;;
        *) validCommand=-1 ;;
      esac
    fi
    ;;
  "leaks")
    # check file existence
    if [[ -f "leaks/TreeGen.c" ]]; then
        gcc -o "leaks/TreeGen" "leaks/TreeGen.c" -lm
    else
        echo "Error: The file leaks/TreeGen.c does not exist"
        exit 1
    fi
    if (( $# != 3 )); then
      validCommand=-1
    else
      validCommand=4
    fi
    ;;
  *) validCommand=-1 ;;
esac

case $validCommand in
  1) #MAX COMMAND
    echo "MAX COMMAND"
    grep -E '^-[;][^;]+;-[;][0-9]+;-$' $1 | ./histo/max
    echo "GREP DONE"
    for f in top10 bottom50; do
        gnuplot -e "set datafile separator ','; set terminal pngcairo; set output '../Histogram/${f}.png'; plot 'histo/${f}.csv' using 2:xtic(1) with boxes"
    done
    ;;
  2) #SRC COMMAND
    echo "SRC COMMAND"
    grep -E "^-;" "$1" | './Histo/TreeGen'
    ;;
  3) #REAL COMMAND
    echo "REAL COMMAND"
    ;;
  4) #LEAKS COMMAND
    echo "LEAKS COMMAND"
    ;;
  *) #INVALID COMMAND
    echo $0": Invalid command please use \""$0" --help\" for more information."
    exit 1
    ;;
esac

# end Runtime
end=$(date +%s.%N)

duration=$(echo "($end - $start) * 1000" | bc -l)
LC_ALL=C printf "Runtime : %.3f milliseconds\n" "$duration"
exit 0
