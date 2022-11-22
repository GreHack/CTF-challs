import time
import os
from Crypto.Cipher import AES
from admin import admin_message


def gen_random():
    state = int(time.time())

    while True:
        x = state
        x ^= (x << 42) & 0xffffffffffffffff
        x ^= x >> 31
        x ^= (x << 19) & 0xffffffffffffffff
        state = x
        yield state


random = gen_random()
key = os.urandom(32)


def encrypt_message(message):
    nonce = next(random).to_bytes(8, byteorder="little")
    cipher = AES.new(key, AES.MODE_CTR, nonce=nonce)
    ciphertext = cipher.encrypt(bytes(message, 'utf-8'))
    return nonce.hex().upper() + ciphertext.hex().upper()


def encrypt_user_message():
    print("What message would you like to encrypt?")
    to_encrypt = input("> ")
    return encrypt_message(to_encrypt)


ENCRYPT = 1
ADMIN_MESSAGE = 2
LEAVE = 3

choice = 0

while choice != LEAVE:
    print(f"----- AES Encryption Toolbox -----\n"
          f"{ENCRYPT}. Encrypt a new message\n"
          f"{ADMIN_MESSAGE}. Read the administrator message\n"
          f"{LEAVE}. Leave. Goodbye.\n")
    choice = int(input("Enter your choice. "))

    if choice == ENCRYPT:
        print(encrypt_user_message())
    elif choice == ADMIN_MESSAGE:
        print(encrypt_message(admin_message))

# rand = gen_random()
# a = list()
#
# for i in range(100000):
#     n = next(rand)
#
#     if n in a:
#         print(f"Found {n} (index {i})")
#         break
#
#     a.append(n)
#
#     print(n)


# def gen_random():
#     state = 63
#
#     while True:
#         x = state
#         x ^= (x << 11) & 0xffffff
#         x ^= x >> 14
#         x ^= (x << 3) & 0xffffff
#         state = x
#         yield state
