#!/bin/bash
filename="$1"

bash mycoolc $filename

echo "--------------------------------"

compiled="$(echo $filename | cut -d '.' -f 1).s"
bash /usr/class/bin/spim $compiled
