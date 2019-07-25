#!/bin/bash
filename="$1"

bash /usr/class/bin/coolc -gt $filename

echo "--------------------------------"

compiled="$(echo $filename | cut -d '.' -f 1).s"
bash /usr/class/bin/spim $compiled
