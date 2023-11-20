# ECB Landing Pad

## Information to set up the chall on CTFd

_Category_: Crypto

_Difficulty_: 5/10

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH{noUseAESECBwithrand0mIV}</p>
</details>

## Description to put on CTFd

You can send messages to the administrator securely. A secret message will be appended, and you will only retrieve a ciphertext.

Don't try to break it. You can't, we use military-grade encryption. You've been warned.

Author: `Feelzor`

## How to access

*File*: [app.py](./app.py)

*Command*: `nc url port` (4242 in container)

## Configuration

`docker build . -t gh23-ecblandingpad && docker run --read-only -p 4242:4242 gh23-ecblandingpad`

## WU

<details>
    <summary>Click to discover/hide</summary>

    You can find a solution [here](./solve.py).

    It uses padding to guess the flag. Two first blocks are used to guess, two last to compare with actual flag concatenated with the message.

</details>
