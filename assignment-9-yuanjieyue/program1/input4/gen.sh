#!/bin/bash

counter=0
# generate 2 MB
num_elements=$((( 2 * (1024 * 1024))/ 4))
OUTPUT_FILE="./test_data.txt"
MAX_VALUE=1000

while [ $counter -lt $num_elements ]; do
  value=$(($counter % $MAX_VALUE ))
  if [ $counter -eq "261763" ]; then
    echo "763" >> $OUTPUT_FILE 
  elif [ $value -eq "763" ]; then
    echo "0" >> $OUTPUT_FILE
  else
    echo "$(($counter % $MAX_VALUE ))" >> $OUTPUT_FILE
  fi
  counter=$(( $counter + 1 ))
done
