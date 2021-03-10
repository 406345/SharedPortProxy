def order():
    return 2

def filter(x):
    print(x)


def detect(x):
    x = ''.join([chr(l) for l in x[:256]])
    return x.find('HTTP/1.1') > 0
