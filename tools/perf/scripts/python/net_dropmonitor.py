# Monitor the system for dropped packets and proudce a report of drop locations and counts

import os
import sys

sys.path.append(os.environ['PERF_EXEC_PATH'] + \
		'/scripts/python/Perf-Trace-Util/lib/Perf/Trace')

from perf_trace_context import *
from Core import *
from Util import *

drop_log = {}
kallsyms = []

def get_kallsyms_table():
	global kallsyms
	try:
		f = open("/proc/kallsyms", "r")
		linecount = 0
		for line in f:
			linecount = linecount+1
		f.seek(0)
	except:
		return


	j = 0
	for line in f:
		loc = int(line.split()[0], 16)
		name = line.split()[2]
		j = j +1
		if ((j % 100) == 0):
			print "\r" + str(j) + "/" + str(linecount),
		kallsyms.append({ 'loc': loc, 'name' : name})

	print "\r" + str(j) + "/" + str(linecount)
	kallsyms.sort()
	return

def get_sym(sloc):
	loc = int(sloc)
<<<<<<< HEAD
	for i in kallsyms:
		if (i['loc'] >= loc):
			return (i['name'], i['loc']-loc)
=======
	for i in kallsyms[::-1]:
		if loc >= i['loc']:
			return (i['name'], loc - i['loc'])
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	return (None, 0)

def print_drop_table():
	print "%25s %25s %25s" % ("LOCATION", "OFFSET", "COUNT")
	for i in drop_log.keys():
		(sym, off) = get_sym(i)
		if sym == None:
			sym = i
		print "%25s %25s %25s" % (sym, off, drop_log[i])


def trace_begin():
	print "Starting trace (Ctrl-C to dump results)"

def trace_end():
	print "Gathering kallsyms data"
	get_kallsyms_table()
	print_drop_table()

# called from perf, when it finds a correspoinding event
def skb__kfree_skb(name, context, cpu, sec, nsec, pid, comm,
<<<<<<< HEAD
			skbaddr, protocol, location):
=======
		   skbaddr, location, protocol):
>>>>>>> 343a5fbeef08baf2097b8cf4e26137cebe3cfef4
	slocation = str(location)
	try:
		drop_log[slocation] = drop_log[slocation] + 1
	except:
		drop_log[slocation] = 1
