'''py_function.py - Python source designed to '''
'''demonstrate the use of python embedding'''

def multiply():
    c = 12345*6789
    print 'The result of 12345 x 6789 :', c
    return c


# Fibonacci numbers module

def fib(n):    # write Fibonacci series up to n
    a, b = 0, 1
    while b < n:
        print b,
        a, b = b, a+b

def fib2(n): # return Fibonacci series up to n
    result = []
    a, b = 0, 1
    while b < n:
        result.append(b)
        a, b = b, a+b
    return result

import math
def ex1():
    return math.cos(math.pi / 4.0)

