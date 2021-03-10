# def order():
#     return 1

def filter(x):
    print("filter called")
    print(x)


def detect(x):
    if len(x) < 4: return False

    if x[0] == 3 and x[1] == 0 and x[2] == 0 and x[3] == 19 : return True

    return False




