import sys
import time

def fib(n):
	if n < 0:
		return 0
	if n == 0 or n == 1:
		return 1
	else:
		return fib(n-1) + fib(n-2)

f = fib(40)

sys.exit(3)
