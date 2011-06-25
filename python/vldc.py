#!/usr/bin/env python

import ctypes as _ctypes
import ctypes.util as _ctypes_util
_path = _ctypes_util.find_library("vldc")
# Hack for debugging on Linux with LD_LIBRARY_PATH set to .
if _path is None: _path = "libvldc.so"
_clib = _ctypes.CDLL(_path)
_clib.vldc.restype = _ctypes.c_char
_clib.vldc_get_float.restype = _ctypes.c_float
_clib.vldc_error_message.restype = _ctypes.c_char_p

def vldc(inString):
	status = _clib.vldc(inString)
	if False: pass
	elif status == "i": rValue = _clib.vldc_get_int()
	elif status == "f": rValue = _clib.vldc_get_float()
	elif status == "m": exception_message = "Out of memory."
	elif status == "e": exception_message = _clib.vldc_error_message()
	else: exception_message = "Unknown status: " + status

	_clib.vldc_clean()

	if status != "i" and status != "f": raise Exception(exception_message)
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

