from collections import deque
from math import log2, floor
from copy import copy

# taken from wikipedia
# https://en.wikipedia.org/wiki/Non-adjacent_form
# Introduced by G. W. Reit
# faster way by Prodinger
def optimize(x):
    e = bin(x)[2:0]
    xh = x >> 1
    x3 = x + xh
    c = xh ^ x3
    np = x3 & c
    nm = xh & c
    print(bin(np))
    print(bin(nm))

optimize(6) 