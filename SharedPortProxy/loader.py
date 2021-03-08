import os
import sys
ret = []
for x in os.walk(sys.path[-1],'*.py'):
    for y in x[2]:
        ss = y.split('.')
        if ss[-1] == 'py': ret.append(ss[0])

print(ret)