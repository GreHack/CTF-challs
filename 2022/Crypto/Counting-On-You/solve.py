import socket

class Netcat:

    """ Python 'netcat like' module """

    def __init__(self, ip, port):

        self.buff = ""
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((ip, port))

    def read(self, length = 1024):

        """ Read 1024 bytes off the socket """

        data = self.socket.recv(length).decode('utf-8')

        return data

    def read_until(self, data):

        """ Read data into the buffer until we have data """

        while not data in self.buff:
            self.buff += self.socket.recv(1024).decode('utf-8')

        pos = self.buff.find(data)
        rval = self.buff[:pos + len(data)]
        self.buff = self.buff[pos + len(data):]

        return rval

    def write(self, data):

        self.socket.send(data)

    def close(self):

        self.socket.close()

nc = Netcat('localhost', 4242)

nonces = list()
nonce = 'nothing'

content = 'A'*250
ctr_aes_output = ''

# Read the toolbox choices
print(nc.read())

i = 0
while True:
    i += 1
    if i % 500 == 0: # Should loop until 4064
        print(f"{i}")
    nc.write(b'1\n')
    nc.read_until('>')
    nc.write(bytes(f'{content}\n', "utf-8"))

    message = nc.read()
    encrypted_message = message.splitlines()[0]
    nonce = encrypted_message[:16]
    if nonce in nonces:
        break
    nonces.append(nonce)

    if i == 2:
        ctr_aes_output = bytes(c ^ k for c, k in zip(bytes.fromhex(encrypted_message[16:]), bytes(content, 'utf-8')))

nc.write(b'2\n')
print('2\n')
message = nc.read()
print(bytes(f"{message}\n", 'utf-8'))
encrypted_message = message.splitlines()[0]
print(bytes(c ^ k for c, k in zip(bytes.fromhex(encrypted_message[16:]), ctr_aes_output)))
