#!/usr/bin/env python3

import fileinput
import pdb

with fileinput.input() as f:
    
    data = "".join(f)

s = {}

for l in data.splitlines():
    if "malloc:" in l:
        c = l.split(":")
        s[c[-1].strip()] = l
        # print("malloc:%s" %c[-1].strip())
    if "free:" in l:
        c = l.split(":")
        del s[c[-1].strip()]
        # print("free:%s" %c[-1].strip())
    # print("size: %d" % len(s))

for l in s:
    print(l)     

