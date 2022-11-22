# XOROT
Category: Crypto  
Points: 100  
Author: Feelzor  
Description:
> To speak securely with the administrator, a toolbox has been made available for you to encrypt a message securely.
> To prove to you that they don't fear any breach, they have made available an encrypted administrator message that you won't be able to read.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{1nsecur3_AES_CTR_w1th_PRNGs}</p>
</details>

## How to run it?
You need to have Docker and Docker Compose installed.
You can run: ``docker compose up`` inside this directory, the challenge will
be available at 127.0.0.1:4242

## Expected resolution steps

The PRNG is weak and cycles every 4064 numbers.

Encrypt a long message (~250 characters)

Remove the 16 first characters (the nonce)

XOR the encrypted message with your message (you retrieve the output of the AES_CTR encryption)

Encrypt 4063 other messages

Retrieve the admin message

XOR the admin message with the result of the previous xor

Automated solve: [solve.py](./solve.py)