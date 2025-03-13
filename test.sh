#!/bin/bash
# A Test script
# Run chmod +x test.sh if permission denied

# Run as ./test.sh num_thread matrix_size

if [ $# -lt 2 ]; then
    echo "Usage: ./test.sh num_thread matrix_size"
    exit 1  # Changed from -1 to 1 for standard error exit code
fi

nt=${1}
ms=${2}

# Clean previous in/out files
make cleanall

# Compiles the  programs
make

clear

echo "Start..."
ATTEMPT=0
while [[ $ATTEMPT -ne 100 ]]; do
    let ATTEMPT+=1
    echo "Attempt ${ATTEMPT} started."

    # Run the datagen, main, and serial executables
    ./datagen -s $ms
    if [ $? -ne 0 ]; then
        echo "Error: datagen failed!"
        exit 1
    fi

    ./main $nt
    if [ $? -ne 0 ]; then
        echo "Error: main failed!"
        exit 1
    fi

    ./serial
    if [ $? -ne 0 ]; then
        echo "Error: serial failed!"
        exit 1
    fi

    # Compare outputs with diff
    diff serial_out.txt main_out.txt
    if [ $? -ne 0 ]; then
        echo "Attempt ${ATTEMPT} failed."
    else
        echo "Attempt ${ATTEMPT} success."
    fi
done
