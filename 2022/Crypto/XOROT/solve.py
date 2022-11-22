import base64
import string
from itertools import cycle


def rot(n, s):
    encoded = symbols[n:] + symbols[:n]
    lookup = str.maketrans(symbols, encoded)
    return s.translate(lookup)


symbols = string.printable

input_text = input("What is the input text? ")
base64_response = input("What is the b64 response? ")

xored_bytes = base64.b64decode(base64_response)
xored_message = xored_bytes.decode('utf-8')

shift = symbols.index(chr(ord(xored_message[0]) ^ ord("G"))) - symbols.index(input_text[0])
print(f"Identified ROT-{shift}")
rotted_text = rot(shift, input_text)

print(''.join(chr(ord(c) ^ ord(k)) for c, k in zip(rotted_text, cycle(xored_message))))
