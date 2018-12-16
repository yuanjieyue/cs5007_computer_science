#!/bin/bash

#constant variables
YEAR=`date +%Y`;
DAY=`date +%-j`;

#do the calculation here
SCORES=$((YEAR/20));
YEARS=$((YEAR%20));
FORTNIGHTS=$((DAY/14));
DAYS=$((DAY%14));

#print out the result
echo "$SCORES score and $YEARS years, $FORTNIGHTS fortnights and $DAYS days."
