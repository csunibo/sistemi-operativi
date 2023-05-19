#!/bin/bash

#soluzione scritta dal prof

copyapp() {
  for f in cd $1 ; find . -type f
  do
    mkdir -p dirname $2/$f
    cat $1/$f >> $2/$f
  done
}

mkdir -p $3
copyapp $1 $3
copyapp $2 $3
