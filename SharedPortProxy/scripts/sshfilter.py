def order():
    return 1

def filter(x):
    print("filter called")
    print(x)


def detect(x):
    x = ''.join([chr(l) for l in x[:3]])
    if x == 'SSH': return True
    return False