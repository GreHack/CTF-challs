import json
import socket
from measure import Measure


class Client:

    def __init__(self, host, port) -> None:
        self.__measure = Measure('Strong authentication')
        self.__host = host
        self.__port = port

    def authenticate(self) -> None:
        print('Authenticate with keystroke dynamics!')
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((self.__host, self.__port))
            template = self.__measure.measure()
            sock.sendall(template.serialize() + b'\n')
            data = sock.recv(1024)
            resp = json.loads(data)
            if 'error' in resp:
                print(f'An error occurred: {resp["error"]}')
            else:
                if resp['authenticated']:
                    print('The flag is:', resp['flag'])
                else:
                    print('Authentication failed')


if __name__ == "__main__":
    client = Client('localhost', 1234)
    client.authenticate()
