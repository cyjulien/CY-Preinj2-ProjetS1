DATA ?= ./DATA/data_v0.dat
TYPE ?= histo
ARG ?= max
CC = gcc
HISTO_FILE = ./CODE/histo/histo
HISTO_SRC = ./CODE/histo/histo.c \
            ./CODE/utility/AVL.c \
            ./CODE/utility/utility.c \
            ./CODE/utility/definition.c
LEAKS_FILE = ./CODE/leaks/leaks
LEAKS_SRC = ./CODE/leaks/leaks.c \
            ./CODE/utility/AVL.c \
            ./CODE/utility/utility.c \
            ./CODE/utility/definition.c
COMPILE_MATH = -lm

SCRIPT=script.sh

.PHONY: all histo bash clean

all: bash

histo: $(HISTO_SRC)
	$(CC) -o $(HISTO_FILE) $(HISTO_SRC) $(COMPILE_MATH)

leaks: $(HISTO_SRC)
	$(CC) -o $(LEAKS_FILE) $(LEAKS_SRC) $(COMPILE_MATH)

bash:
	bash $(SCRIPT) $(DATA) $(TYPE) "$(ARG)"

clean:
	rm -f $(HISTO_FILE)
	rm -f $(LEAKS_FILE)
	rm -f ./DATA/*.csv
	rm -f ./DATA/leaks_log.dat
	rm -f ./Histogram/*.png
