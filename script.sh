#!/bin/bash

# start Runtime
start=$(date +%s.%N)

generate_plot() {

  if [ ! -d "./Histogram" ]; then
      mkdir -p ./Histogram
  fi

  local csv_file=$1
  local png_file=$2
  local title=$3

  gnuplot <<EOF
set datafile separator ','
set terminal pngcairo size 1200,800
set output '${png_file}'

set title '${title}'
set xlabel 'factory identifier'
set ylabel 'million cubic meters'

set xtics rotate by 45 right
set style fill solid 0.8
set boxwidth 0.8

set key outside right top

plot '${csv_file}' using (\$2/1000):xtic(1) with boxes title '${title}'
EOF
}

histo() {
  awk -F';' '
  #FACILITY
  $1=="-" && $3=="-" && $4 ~ /^[0-9]/ && $2 ~ /^(Plant|Unit|Module) / {
      print
  }

  #SOURCE -> FACILITY
  $1=="-" &&
  $2 ~ /^(Source|Spring|Well|Well field|Fountain|Resurgence) / &&
  $3 ~ /^(Plant|Unit|Module) / &&
  $4 ~ /^[0-9]/ && $5 ~ /^[0-9]/ {
      print
  }
  ' "$1" | ./CODE/histo/histo $2
}

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
    #Check file existence
    if [[ -f "./CODE/histo/histo.c" ]]; then
        gcc -o "./CODE/histo/histo" "./CODE/histo/histo.c" "./CODE/utility/AVL.c" "./CODE/utility/utility.c" -lm
    else
        echo "Error: The file /CODE/histo/histo.c does not exist"
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
    if [[ -f "./CODE/leaks/TreeGen.c" ]]; then
        gcc -o "./CODE/leaks/TreeGen" "./CODE/leaks/TreeGen.c" -lm
    else
        echo "Error: The file /CODE/leaks/TreeGen.c does not exist"
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
    histo "$1" "$3"
    echo "LOOKING DONE"

    for f in top10 bottom50; do
        generate_plot "./DATA/histo/${f}.csv" "./Histogram/max_${f}.png" "Maximum processing volume for each (${f}) facility"
    done
    ;;
  2) #SRC COMMAND
    echo "SRC COMMAND"
    histo "$1" "$3"
    echo "LOOKING DONE"

    for f in top10 bottom50; do
        generate_plot "./DATA/histo/${f}.csv" "./Histogram/src_${f}.png" "Total volume captured by the sources of (${f}) facility"
    done
    ;;
  3) #REAL COMMAND
    histo "$1" "$3"
    echo "LOOKING DONE"

    for f in top10 bottom50; do
        generate_plot "./DATA/histo/${f}.csv" "./Histogram/real_${f}.png" "Total volume actually processed for (${f}) facility"
    done
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
