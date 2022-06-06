#!/bin/bash

dest="renzo"

sed 's/^/-->/' << ENDTAGXXX
caro $dest
buona giornata
saluti a $dest
ENDTAGXXX

echo ho finito

exit 42
