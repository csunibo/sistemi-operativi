#!/bin/bash

if cmp -s $1 $2
then
	echo uguali
else
	echo diversi
fi
