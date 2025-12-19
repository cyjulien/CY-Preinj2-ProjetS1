#!/bin/bash

# start Runtime
start=$(date +%s.%N)

gcc -o 'Tree&ABR' 'Tree&ABR.c' -lm
validCommand=0
if (( $# < 1 || $# > 3 )); then
  validCommand=-1
fi

case $2 in #PARSE FIRST ARGUMENT
  "--help")
    if (( $# != 1 )); then
      validCommand=-1
    else
      echo "Usage: $0 [path] <command> [arguments]"
      echo "$0 [path] histo <max|src|real>"
      echo "$0 [path] leaks \"Facility ID\""
      exit 0
    fi
    ;;
  "histo")
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
    exit 0
    ;;
  2) #SRC COMMAND
    echo "SRC COMMAND"
    grep -E "^-;" "$1" | './Tree&ABR'
    exit 0
    ;;
  3) #REAL COMMAND
    echo "REAL COMMAND"
    exit 0
    ;;
  4) #LEAKS COMMAND
    echo "LEAKS COMMAND"
    exit 0
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
