from binascii import hexlify, unhexlify
from secret import s
from random import randint
import tag_gen


def send_challenge(size):
    c = 2**randint(size, (size*2)) - 1
    byte_index = randint(0, size*8 - 1)
    c = (c << byte_index & 2**(8*size) - 1) | (c >> size*8 - byte_index)
    c = hexlify(c.to_bytes(size, byteorder="big"))
    #print(count, c.decode(), resp)
    return c

def get_response(c, s, p):
#    r = int(input("Response: "))
    r = tag_gen.send_response(c, s, p)

    print("Response: {}".format(r))
    return r

if __name__ == "__main__":
    count = [0, 0]
    p = 0.3
    print("p = {}".format(p))
    for i in range(65536):
        c = send_challenge(len(s))
        print("Enter your challenge ({} bytes, hex encoded): {}"
                .format(len(s), c.decode()))
        c = int(c, 16)
        r = get_response(c, s, 0.3)
        
        s_int = int.from_bytes(s, "big")
        tmp = c & s_int
        b = bin(tmp).count('1') % 2
        e = r ^ b
        count[e] += 1

    res = count[1]/(count[1] + count[0])
    if res < 0.32:
        print("Welcome home!")
    else:
        print("Wroooong!")
    #print(count[1]/(count[1] + count[0]))
