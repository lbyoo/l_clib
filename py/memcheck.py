#!/usr/bin/env python3

"""
Usage: program | ./memcheck.py
"""
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

print("以下内存申请可能未释放，请检查：")
for l in s:
    print(s[l])
else:
    print("没有需要处理的")



