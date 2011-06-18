#!/usr/bin/env python

import ctypes as _ctypes
import ctypes.util as _ctypes_util
_path = _ctypes_util.find_library("vldc")
# Hack for debugging on Linux with LD_LIBRARY_PATH set to .
if _path is None: _path = "libvldc.so"
_clib = _ctypes.CDLL(_path)
_clib.vldc_error_message.restype = _ctypes.c_char_p

def vldc(inString):
	rValue = _clib.vldc(inString)
	state = _clib.vldc_error_status()
	exception_message = None
	if state == 1:
		exception_message = "Error making calculation. Also can't get error message."
	if state == 2:
		exception_message = _clib.vldc_error_message()
	_clib.vldc_clean()
	if state != 0:
		raise Exception(exception_message)
	return rValue

if __name__ == "__main__":
	from sys import argv, stdin
	if len(argv) == 2:
		try:
			print "Value:", vldc(argv[1])
		except Exception as e:
			print "Error:", e
	elif len(argv) == 1:
		keeplooping = True
		istty = stdin.isatty()
		while keeplooping:
			try:
				if istty: print ">",
				value = vldc(raw_input())
				print "Value:", value
			except EOFError as e:
				if istty: print
				keeplooping = False
			except Exception as e:
				print "Error:", e

