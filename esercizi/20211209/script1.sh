#!/bin/bash

dest="renzo"

echo "caro $dest" > file$$
echo "buona giornata" >> file$$
echo "saluti a $dest" >> file$$

cat file$$ | sed 's/^/-->/' 

rm file$$
