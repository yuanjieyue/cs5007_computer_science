#!/bin/bash

counter=0
# generate 10 MB
num_elements=$(( 10 * ((1024*1024)/4) ))
OUTPUT_FILE="./test_data2.txt"
MAX_VALUE=999

while [ $counter -lt $num_elements ]; do
  echo "$((counter % $MAX_VALUE ))" >> $OUTPUT_FILE
  counter=$(( $counter + 1 ))
done
