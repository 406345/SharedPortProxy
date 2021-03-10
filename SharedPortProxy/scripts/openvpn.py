def order():
    return 2

def filter(x):
    print(x)


def detect(x):
    if len(x) < 4: return False
    if x[0] == 22 and x[1] == 3 and x[2] == 1 and x[3] == 0 : return True
    return False
