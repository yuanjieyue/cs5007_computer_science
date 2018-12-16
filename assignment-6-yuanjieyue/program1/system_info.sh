#! /bin/bash

# Get the infos of the system:
# 1. the number of processors in the system
# 2. the free memory and total memory of the system
# 3. the free storage and total storage of the system

NUM_OF_CORES=$(nproc)
FREE_MEMORY=$(free -m | awk 'NR==2{printf "%d", $4}')
TOTAL_MEMORY=$(free -m | awk 'NR==2{printf "%d", $2}')
FREE_STORAGE=$(df -h | awk 'NR==3{printf "%d", $4}')
TOTAL_STORAGE=$(df -h | awk 'NR==3{printf "%d", $2}')

echo "cores: <${NUM_OF_CORES}>"
echo "memory: <${FREE_MEMORY} MB>/<$TOTAL_MEMORY MB>"
echo "storage: <${FREE_STORAGE} GB>/<${TOTAL_STORAGE} GB>"
