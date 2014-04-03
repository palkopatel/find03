#!/bin/sh

# для варианта ./findw_t.exe -i db/ -d
awk -F";" '{if (NF != 3) print NF}' db/3words

# и, например, для варианта ./findw_t.exe -i db/ -d 5
# awk -F";" '{if (NF != 7) print NF}' db/3words
