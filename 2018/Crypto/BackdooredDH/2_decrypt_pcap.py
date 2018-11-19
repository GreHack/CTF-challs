import pyshark
import sys
from binascii import unhexlify, hexlify

from Crypto.Util.number import long_to_bytes
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

from cryptography.hazmat.primitives import hashes

from prf import prf

if len(sys.argv) != 3:
    print("Usage : {} <pcap> <pre-master-secret>".format(sys.argv[0]))
    exit()

pcap = pyshark.FileCapture(sys.argv[1])
pre_masterkey = int(sys.argv[2])

client_hello = pcap[3]
server_hello = pcap[5]

client_random = unhexlify(client_hello.ssl.handshake_random.replace(":", ""))
server_random = unhexlify(server_hello.ssl.handshake_random.replace(":", ""))


secret = long_to_bytes(pre_masterkey)
seed = client_random + server_random
label = b"master secret"
hash_algorithm = hashes.SHA256()

master_key = prf(secret, label, seed, hash_algorithm, 48)

print("Master key : {}".format(hexlify(master_key).decode().upper()))

derivated_keys = prf(master_key, b"key expansion", server_random+client_random, hash_algorithm, 96)

client_MAC_key, server_MAC_key, client_key, server_key  = derivated_keys[:32], derivated_keys[32:64], derivated_keys[64:80], derivated_keys[80:96]

print("client_key : {}".format(hexlify(client_key).decode().upper()))
print("server_key : {}".format(hexlify(server_key).decode().upper()))


for pkt in pcap:
    if 'SSL' in pkt:

        if "app_data" in pkt.ssl.field_names:

            payload = unhexlify(pkt.ssl.app_data.replace(":", ""))
            iv, data_mac = payload[:16], payload[16:]

            def decrypt(aes, data_mac):
                dec = unpad(aes.decrypt(data_mac), AES.block_size)
                return dec[:len(dec) - 33].decode()

            if pkt.tcp.srcport == "443":
                aes_server = AES.new(server_key, AES.MODE_CBC, iv)
                print("Decrypted :\n {}".format(decrypt(aes_server, data_mac)))          
            else:
                aes_client = AES.new(client_key, AES.MODE_CBC, iv)
                print("Decrypted :\n {}".format(decrypt(aes_client, data_mac)))

