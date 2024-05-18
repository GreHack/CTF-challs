import json
import socket
from measure import Measure


class Client:

    def __init__(self, host, port) -> None:
        self.__measure = Measure('Strong authentication')
        self.__host = host
        self.__port = port

    def normal_authenticate(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((self.__host, self.__port))
            template = self.__measure.measure()
            sock.sendall(template.serialize() + b'\n')
            data = sock.recv(1024)
            resp = json.loads(data)
            if 'error' in resp:
                raise Exception(f'An error occurred: {resp["error"]}')
            else:
                if resp['authenticated']:
                    print(resp['flag'])
                else:
                    print('Authentication failed')
            return template, resp['score']

    def optimize(self, template, score):
        # Optimization step: if it takes forever, increase the step
        step = min(template.times) / 10
        # Best score
        min_score = score
        # Index of the value to change in the template
        index = 0
        # The sign of the step
        plus = True
        # Wether the sign of the step has already been changed for this index
        changed = False
        while True:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.connect((self.__host, self.__port))
                # Mutate the template and send it to the server
                if plus:
                    template.times[index] += step
                else:
                    template.times[index] -= step
                sock.sendall(template.serialize() + b'\n')
                data = sock.recv(1024)
                resp = json.loads(data)
                if 'error' in resp:
                    raise Exception(f'An error occurred: {resp["error"]}')
                else:
                    # Print the flag if the authentication is successful
                    if resp['authenticated']:
                        print(resp['flag'])
                        break
                    else:
                        score = resp['score']
                        print(f'score = {round(score)}', end='\r')
                        # If the score decreases, save the new minnimal score
                        # and continue the optimization process
                        if score < min_score:
                            min_score = score
                        # Otherwise, if the sign of the step was already
                        # changed for this index, revert the last mutation and
                        # increment the index
                        elif changed:
                            if plus:
                                template.times[index] -= step
                            else:
                                template.times[index] += step
                            print(f'{index = } done')
                            index += 1
                            if index == 42:
                                break
                            changed = False
                        # Finally, if the sign was not changed for this index,
                        # revert the last mutation and change it
                        else:
                            plus = not plus
                            if plus:
                                template.times[index] += step
                            else:
                                template.times[index] -= step
                            changed = True


if __name__ == "__main__":
    client = Client('localhost', 1234)
    # Get a base template with its associated score
    template, score = client.normal_authenticate()
    # Minimize the score
    client.optimize(template, score)
