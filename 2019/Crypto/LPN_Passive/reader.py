from binascii import hexlify
from secret import s
from random import randint


def send_challenge(size):
    c = 2**randint(size, (size*2)) - 1
    byte_index = randint(0, size*8 - 1)
    c = (c << byte_index & 2**(8*size) - 1) | (c >> size*8 - byte_index)
    c = hexlify(c.to_bytes(size, byteorder="big"))
    
    print(c)
    return c

def get_response():
    r = int(input("Response: "))

    return r

if __name__ == "__main__":
    count = [0, 0]
    p = 0.3
    for i in range(65536):
        c = int(send_challenge(len(s)), 16)
        r = get_response()
        
        s_int = int.from_bytes(s, "big")
        tmp = c & s_int
        b = bin(tmp).count('1') % 2
        e = r ^ b
        count[e] += 1

    res = count[1]/(count[1] + count[0])
    if res < 0.32:
        print("Welcome home!")
        exit(0)
    else:
        print("Wroooong!")
        exit(1)
