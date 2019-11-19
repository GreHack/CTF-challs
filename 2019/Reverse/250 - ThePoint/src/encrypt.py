#!/usr/bin/env python3

"""
This script is used to xor some strings and copy paste them in the source code
TODO: Generating a .h would be smarter
"""


StrWelcome = 'Welcome to BoredInstaller'
StrWelcome2 = 'To extract the data, please provide the correct password.'
StrPassword = 'Password: '
StrWrong = 'Wrong password!'
StrCorrect = 'Correct! You can use this input to validate the challenge!'


def cipher(s, cst):
    b = ''
    for c in s:
        b += chr(ord(c) ^ cst)
    return b


def str2hex(s):
    b = ''
    for c in s:
        b += '\\x' + hex(ord(c))[2:]
    return '"' + b + '"'


c = cipher(StrWelcome, 0x41)
print(str2hex(c))
c = cipher(StrWelcome2, 0x42)
print(str2hex(c))
c = cipher(StrPassword, 0x43)
print(str2hex(c))
c = cipher(StrWrong, 0x13)
print(str2hex(c))
c = cipher(StrWrong, 0x78)
print(str2hex(c))
c = cipher(StrCorrect, 0x99)
print(str2hex(c))
