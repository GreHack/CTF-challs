import string

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import time
from admin import KEY, FLAG


def secure_pseudo_random():
    # Heavily stolen from https://stackoverflow.com/a/72238618
    state = int(time.time())

    while True:
        x = state
        x ^= (x << 13) & 0xffffffff
        x ^= x >> 17
        x ^= (x << 5) & 0xffffffff
        state = x
        yield state


def random_iv(key_len: int):
    randomizer = secure_pseudo_random()
    chars = string.ascii_letters + string.digits + string.punctuation

    while True:
        key = ""
        for i in range(key_len):
            rand = next(randomizer)
            key += chars[rand % len(chars)]

        yield key.encode("utf_8")


BLOCK_SIZE = 16
iv_randomizer = random_iv(BLOCK_SIZE)


def encrypt(plaintext):
    iv = next(iv_randomizer)
    padded = pad(plaintext + FLAG, BLOCK_SIZE)
    padded_iv = iv * int(len(padded) / BLOCK_SIZE)
    cipher = AES.new(KEY, AES.MODE_ECB)
    ciphertext = cipher.encrypt(bytes(a ^ b for (a, b) in zip(padded, padded_iv)))
    return ciphertext.hex()


for i in range(4096):
    message = input(f"{i}. Enter your message: ").encode("utf-8")
    print(encrypt(message))
