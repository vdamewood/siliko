#!/usr/bin/env python

from ctypes import *
from sys import argv

if __name__ == "__main__":
	libvldc = CDLL("libvldc.so")
	print "Value:", libvldc.vldc(argv[1])
