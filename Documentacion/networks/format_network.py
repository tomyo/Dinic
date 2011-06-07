#!/usr/bin/env python
# encoding: utf-8

import sys

def to_network(filename, outputname):
    f  = open(filename, "r")
    f2 = open(outputname, "w")

    convert = {}

    a = f.readline()
    while (a != '\n'):
        k, d = a.split(":")
        convert[k] = int(d)
        a = f.readline()

    text = f.read()
    new_text = ""
    for char in text:
        if char in convert:
            new_text+=str(convert[char])
        else:
            new_text+=char

    f2.write(new_text)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print "Usage: python %s path_to_network_to_format result" % sys.argv[0]
    else:
        to_network(sys.argv[1], sys.argv[2])
