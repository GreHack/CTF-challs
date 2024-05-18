from random import SystemRandom
from Crypto.Cipher import AES
from Crypto.Util.number import long_to_bytes , bytes_to_long
from hashlib import sha256
from functools import reduce

random = SystemRandom()

def encrypt(plain, key):
    Ks = [(key >> (12*i)) & (pow(2, 12) - 1) for i in range(4)]
    return reduce(lambda D,K:AES.new(sha256(long_to_bytes(K)).digest(), AES.MODE_ECB).encrypt(D) , Ks, plain)

if __name__ == "__main__":
    key = random.getrandbits(48) 
    flag = b"**redacted**"
    assert (len(flag) == 16)
    plain = b"iloveGH!"*2
    print("plain = '{}'".format(plain.decode()))
    print("cipher = '{}'".format(encrypt(plain, key).hex()))
    print("encrypted_flag = '{}'".format(encrypt(flag, key).hex()))
