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


def bruteforce():
    flag = 'GH{'
    total = 32 - 1
    chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-~!?#%&@{}'
    while flag[-1:] != '}':
        print(nc.read_until(": "))
        payload = '1' * (total - len(flag))
        for c in chars:
            message = (payload + flag + c + payload + "\n").encode()
            print(message)
            nc.write(message)
            ciphertext = nc.read_until('\n')
            print(ciphertext)
            # Comapare the middle blocks ([32:64]) of each encrypted text
            if ciphertext[32:64] == ciphertext[64+32:64+64]:
                flag += c
                print(f"Found {flag}")
                break


bruteforce()

