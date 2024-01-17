#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: ./experiments.sh <path/to/dataset> <path/to/output-csv>"
    exit 1
fi

DEFAULT_OUTPUT_CSV="experiment_results.csv"

OUTPUT_CSV=$2

: ${OUTPUT_CSV:=$DEFAULT_OUTPUT_CSV}

# Set the path to your binary
BINARY_PATH="./build/app/app"

# Set the path to your dataset
DATASET_PATH=$1

# Write the CSV header
echo "N,K,delta,rho,time_ms,recall" > $OUTPUT_CSV

# Define parameter configurations
K_VALUES=(5 10)
DELTA_VALUES=(0.001 0.01 0.1)
RHO_VALUES=(0.25 0.5 0.75 1)

# Loop through parameter configurations
for K in "${K_VALUES[@]}"; do
    for DELTA in "${DELTA_VALUES[@]}"; do
        for RHO in "${RHO_VALUES[@]}"; do
            # Run your binary and measure execution time
            $BINARY_PATH -q -K $K -d $DELTA -r $RHO $DATASET_PATH >> $OUTPUT_CSV
        done
    done
done