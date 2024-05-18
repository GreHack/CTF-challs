from random import SystemRandom
from Crypto.Cipher import AES
from Crypto.Util.number import long_to_bytes , bytes_to_long
from hashlib import sha256
from functools import reduce
from tqdm import tqdm

random = SystemRandom()

def decrypt(cipher, key):
    Ks = [(key >> (12*i)) & (pow(2, 12) - 1) for i in range(4)]
    return reduce(lambda D,K:AES.new(sha256(long_to_bytes(K)).digest(), AES.MODE_ECB).decrypt(D) , Ks[::-1], cipher)


def find_key(P, C):
    U,D = {},{}
    for k1 in tqdm(range(2**12)):
        aes1 = AES.new(sha256(long_to_bytes(k1)).digest(), AES.MODE_ECB)
        up = aes1.encrypt(P)
        down = aes1.decrypt(C)
        for k2 in range(2**12):
            aes2 =  AES.new(sha256(long_to_bytes(k2)).digest(), AES.MODE_ECB)
            up2 = aes2.encrypt(up)
            down2 = aes2.decrypt(down)
            U[up2] = (k1, k2)
            D[down2] = (k2, k1)

            if up2 in D:
                (k3, k4) = D[up2]
                print("D collision key :  {:x}, {:x}, {:x}, {:x}".format(k1, k2, k3, k4))
                return (k1 << 36) | (k2 << 24) | (k3 << 12) | k4
            if down2 in U:
                (k4, k3) = U[down2]
                print("U collision key :  {:x}, {:x}, {:x}, {:x}".format(k1, k2, k3, k4))
                return (k1 << 36) | (k2 << 24) | (k3 << 12) | k4
                   

if __name__ == "__main__":
    plain = b'iloveGH!iloveGH!'
    cipher = bytes.fromhex('0f973d8234b8086847b9d423617a19b0')
    encrypted_flag = bytes.fromhex('bc4daf41f71e4dd958a852bbbbf8dfec')
    retrieved_key = find_key(plain, cipher)
    print("retrieved key : {:x}".format(retrieved_key))
    print("flag : {}".format(decrypt(encrypted_flag, retrieved_key)))
