from collections import deque
from math import log2, floor
from copy import copy

def optimize(n):
    if n == 1 or n == 0:
        print(0)
        return
    
    queue = deque()
    queue.append([n])
    
    # breadth first search approach that checks each instance of 2^n + (-1, 0, 1)
    while queue:
        next = queue.popleft()
        
        latest = next[-1]

        p = floor(log2(latest))
        b = 2 ** p


        # if latest is a power of 2
        if b == latest:
            print(next)
            break

        a = b - 1
        c = b + 1

        alist = next
        blist = copy(next)
        clist = copy(next)

        alist[-1] = a
        alist.append(latest - a)
        blist[-1] = b
        blist.append(latest - b)
        clist[-1] = c
        clist.append(latest - c)

        queue.append(blist)
        queue.append(alist)
        queue.append(clist)
optimize(21) 