from binascii import hexlify, unhexlify
from secret import s
from random import choices

def get_challenge(size):
    c_raw = input("Enter your challenge ({} bytes, hex encoded): "
            .format(size))
    if len(c_raw) != size*2:
        print("Wrong length!")
        return None

    try:
        c = int(c_raw, 16)
        return c
    except e:
        print("Wrong challenge format!")
        return None



def send_response(c, s, p):
    s = int.from_bytes(s, "big")
    tmp = c & s
    b = bin(tmp).count('1') % 2
    e = choices([1,0], weights=[p, 1-p])[0]
    
    print(b ^ e)
    return b ^ e


if __name__ == "__main__":
    p = 0.3
    print(p)
    while True:
        c = get_challenge(len(s))

        if c is not None:
            send_response(c, s, p)
