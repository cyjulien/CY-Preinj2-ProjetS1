#!/bin/bash

# start Runtime
start=$(date +%s.%N)
echo "1:$1 2:$2 3:$3 4:$4"

#Ensure proper file stucture
mkdir -p ./DATA ./Histogram

histoParseData() {
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

extract_top_bottom() {
  {
    head -n 1 ./DATA/all.csv
    tail -n +2 ./DATA/all.csv | sort -t',' -k2,2n | head -n 50
  } > ./DATA/bottom50.csv

  {
    head -n 1 ./DATA/all.csv
    tail -n +2 ./DATA/all.csv | sort -t',' -k2,2n | tail -n 10
  } > ./DATA/top10.csv
}

generate_plot() {
  csv=$1
  png=$2
  title=$3
  col=$4

  gnuplot <<EOF
set datafile separator ','
set datafile columnheaders

set terminal pngcairo size 1600,900
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

generate_plot_all() {

  csv=$1
  png=$2
  title=$3

  gnuplot <<EOF
set datafile separator ','
set datafile columnheaders

set terminal pngcairo size 1600,900
set output '${png}'

set title '${title}'
set xlabel 'Factory'
set ylabel 'Volume (in million cubic meters)'

set xtics rotate by 45 right
set style data histograms
set style histogram rowstacked gap 1
set style fill solid 0.8
set boxwidth 0.8
set key outside right top

set yrange [0:*] 

plot \
  '${csv}' using (\$4/1000):xtic(1) title 'Volume supplied (real)' lc rgb "#4E79A7", \
  '' using ((\$3-\$4)/1000) title 'Lost volume (src - real)' lc rgb "#EB4034", \
  '' using ((\$2-\$3)/1000) title 'Remaining volume (max - src)' lc rgb "#2DBD62"
EOF
}

run_histo_command() {
  dataFile="$1"
  commandName="$2"
  column="$3"
  title="$4"

  echo "${commandName}: command started..."
  histoParseData "$dataFile" "$commandName"
  echo "${commandName}: data parsed, generating additional .csv files..."
  extract_top_bottom
  echo "${commandName}: .csv files generated, generating images..."

  if [[ "$commandName" == "all" ]]; then
    plot_function="generate_plot_all"
  else
    plot_function="generate_plot"
  fi

  for f in top10 bottom50; do
    title="$4"
    if [[ -f "./DATA/${f}.csv" ]]; then
      ${plot_function} \
        "./DATA/${f}.csv" \
        "./Histogram/${commandName}_${f}.png" \
        "${title} (${f})" \
        "${column}"
      echo "${commandName}: generated ${commandName}_${f}.png"
    else
      echo "Warning: No data found for ${f}, the image will not be generated."
    fi
  done

  echo "${commandName}: done."
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
    echo "$0 [path] histo <max|src|real|all>"
    echo "$0 [path] leaks \"Facility ID\""
    exit 0
  fi
fi

if [ ! -f "$1" ]; then
  echo "Error: Data file not found: $1"
  exit 1
fi

case $2 in #PARSE FIRST ARGUMENT
  "histo")
    make histo || exit 1 
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
        "all")
          validCommand=4
          ;;
        *) validCommand=-1 ;;
      esac
    fi
    ;;
  "leaks")
    make leaks || exit 1 
    if (( $# != 3 )); then
      validCommand=-1
    else
      validCommand=5
    fi
    ;;
  *) validCommand=-1 ;;
esac

case $validCommand in
  1) #MAX COMMAND
    run_histo_command "$1" "max" 2 "Maximum volume of water processed for each facility"
    ;;
  2) #SRC COMMAND
    run_histo_command "$1" "src" 3 "Total volume of water captured by the sources of each facility"
    ;;
  3) #REAL COMMAND
    run_histo_command "$1" "real" 4 "Total volume of water processed for each facility accounting for leaks"
    ;;
  4) #ALL COMMAND
    run_histo_command "$1" "all" 0 "Summary of all data"
    ;;
  5) #LEAKS COMMAND
    commandName="leaks"
    echo "${commandName}: command started..."
    awk -F ';' -v plantID="$3" '
    $1 ~ plantID || $2 ~ plantID || $3 ~ plantID
    ' "$1" | ./CODE/leaks/leaks "$3"
    echo "${commandName}: done."
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
