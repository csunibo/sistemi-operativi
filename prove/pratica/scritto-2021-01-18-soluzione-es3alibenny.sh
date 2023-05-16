#!/bin/bash

# Get the pattern from command line argument
pattern=$1

# Find all files containing the pattern in the current directory and its subdirectories
# and sort the results by modification time in descending order
find . -type f -exec grep -H "$pattern" {} \; | sort -k3,3 -r