#!/bin/bash

# start Runtime
start=$(date +%s.%N)

#Ensure proper file stucture
if [ ! -d "./DATA" ]; then
    mkdir -p ./DATA
fi
if [ ! -d "./Histogram" ]; then
    mkdir -p ./Histogram
fi

generate_plot() {

  csv=$1
  png=$2
  title=$3
  col=$4

  gnuplot <<EOF
set datafile separator ','
set datafile columnheaders

set terminal pngcairo size 1200,800
set output "${png}"

set title '${title}'
set xlabel 'Factory'
set ylabel 'Volume (in million cubic meters)'

set xtics rotate by 45 right
set style fill solid 0.8
set boxwidth 0.8
set key off

set yrange [0:*] 

plot "${csv}" using (\$$col/1000):xtic(strcol(1)) with boxes lc rgb "#4E79A7"
EOF
}

histo() {
  awk -F ';' '
  #SOURCE -> PLANT
  $1 == "-" &&
  $4 ~ /^[0-9]+$/ &&
  $5 ~ /^[0-9.]+$/ {
      print
      next
  }

  #PLANT
  $1 == "-" &&
  $3 == "-" &&
  $4 ~ /^[0-9]+$/ &&
  $5 == "-" {
      print
  }
  ' "$1" | ./CODE/histo/histo "$2"
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
    if [[ -f "./CODE/histo/histo.c" ]]; then #Check for all the files
        gcc -o "./CODE/histo/histo" "./CODE/histo/histo.c" "./CODE/utility/AVL.c" "./CODE/utility/utility.c" "./CODE/utility/definition.c" -lm
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
    commandName="max"
    column=2 #This is the column that will be looked up in the generated .csv
    echo "${commandName}: command started"
    histo "$1" "$3" #Send the data to the histo program that will generate a .csv file
    echo "${commandName}: data parsed"

    for f in top10 bottom50; do
      if [[ -f "./DATA/${f}.csv" ]]; then
        generate_plot "./DATA/${f}.csv" "./Histogram/${commandName}_${f}.png" "Maximum volume of water processed for each facility (${f})" "${column}"
        echo "${commandName}: generated ${commandName}_${f}.png"
      else
        echo "Warning: No data found for ${f}, the image will not be generated."
      fi
    done
    ;;
  2) #SRC COMMAND
    commandName="src"
    column=3 #This is the column that will be looked up in the generated .csv
    echo "${commandName}: command started"
    histo "$1" "$3" #Send the data to the histo program that will generate a .csv file
    echo "${commandName}: data parsed"

    for f in top10 bottom50; do
      if [[ -f "./DATA/${f}.csv" ]]; then
        generate_plot "./DATA/${f}.csv" "./Histogram/${commandName}_${f}.png" "Total volume of water captured by the sources of each facility (${f})" "${column}"
        echo "${commandName}: generated ${commandName}_${f}.png"
      else
        echo "Warning: No data found for ${f}, the image will not be generated."
      fi
    done
    ;;
  3) #REAL COMMAND
    commandName="real"
    column=4 #This is the column that will be looked up in the generated .csv
    echo "${commandName}: command started"
    histo "$1" "$3" #Send the data to the histo program that will generate a .csv file
    echo "${commandName}: data parsed"

    for f in top10 bottom50; do
      if [[ -f "./DATA/${f}.csv" ]]; then
        generate_plot "./DATA/${f}.csv" "./Histogram/${commandName}_${f}.png" "Total volume of water processed for each facility accounting for leaks (${f})" "${column}"
        echo "${commandName}: generated ${commandName}_${f}.png"
      else
        echo "Warning: No data found for ${f}, the image will not be generated."
      fi
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
