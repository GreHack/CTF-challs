import base64
import string
import random
from itertools import cycle
from langdetect import detect_langs

symbols = string.printable
key = "REDACTED"


def rot(n, s):
    encoded = symbols[n:] + symbols[:n]
    lookup = str.maketrans(symbols, encoded)
    return s.translate(lookup)


def is_english(text):
    for lang in detect_langs(text):
        if lang.lang == "en":
            return lang.prob > 0.8

    return False


user_text = input("Please enter your text: ")
if not is_english(user_text):
    print("The input text must be an english sentence.")
    exit(1)

rot_text = rot(random.randint(1, 25), user_text)
xor_text = ''.join(chr(ord(c) ^ ord(k)) for c, k in zip(rot_text, cycle(key)))

b64_bytes = base64.b64encode(bytes(xor_text, 'utf-8'))
print(b64_bytes.decode('utf-8'))
