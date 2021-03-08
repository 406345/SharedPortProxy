def order():
    return 2

def filter(x):
    print("filter called")
    print(x)


def detect(x):
    x = ''.join([chr(l) for l in x[:256]])
    print(x)
    return x.find('HTTP/1.1') > 0
