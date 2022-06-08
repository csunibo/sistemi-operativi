#!/bin/bash

FMT="%-30s%-30s%-30s\n"

printf $FMT "Pid" "Name" "VmSize"

for file in /proc/*/status; do
  pid=$(echo $file | cut -d'/' -f3)
  name=$(cat $file | grep Name: | cut -f2) # cut uses tabs by default
  vmsize=$(cat $file | grep VmSize: | cut -f2 | tr -d '\n') # cut uses tabs by default
  # ignoro deliberatamente i processi di cui non posso vedere il nome o la memoria
  if [[ "$name" = "" ]] || [[ "$vmsize" = "" ]]; then
    continue
  fi

  printf $FMT "$pid" "$name" "$vmsize"
done
