#!/bin/bash
validCommand=0
if (( $# < 1 || $# > 2 )); then
  validCommand=-1
fi

case $1 in #PARSE FIRST ARGUMENT
  "--help")
    if (( $# != 1 )); then
      validCommand=-1
    else
      echo "Usage: $0 <command> [arguments]"
      echo "$0 histo <max|src|real>"
      echo "$0 leaks \"Facility ID\""
      exit 0
    fi
    ;;
  "histo")
    if (( $# != 2 )); then
      validCommand=-1
    else
      case $2 in #PARSE SECOND ARGUMENT
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
    if (( $# != 2 )); then
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
