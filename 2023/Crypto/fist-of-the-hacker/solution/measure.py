import time
from pynput import keyboard
from template import Template
from threading import Lock


class Measure:

    def __init__(self, passphrase):
        self.__passphrase = passphrase
        self.__lock = Lock()

    def measure(self):
        print('Type the following passphrase:', self.__passphrase)
        self.__measured_passphrase = ''
        while self.__measured_passphrase != self.__passphrase:
            print('\r', 80 * ' ', end='\r')
            self.__start_measure()
            with keyboard.Listener(
                    on_press=self.__on_press,
                    on_release=self.__on_release,
                    suppress=True
            ) as listener:
                listener.join()
        return self.__end_measure()

    def __start_measure(self):
        self.__measured_passphrase = ''
        self.__dwell_times = []
        self.__latencies = []
        self.__press_start = 0
        self.__release_start = 0

    def __end_measure(self):
        print()
        return Template(self.__passphrase, self.__dwell_times + self.__latencies[1:])

    def __on_press(self, key):
        try:
            if key == keyboard.Key.space:
                self.__measured_passphrase += ' '
            else:
                self.__measured_passphrase += key.char
        except AttributeError:
            return False
        else:
            with self.__lock:
                self.__press_start = time.time()
                self.__latencies.append(self.__press_start - self.__release_start)
            print(f'\r{self.__measured_passphrase}', end='', flush=True)

    def __on_release(self, key):
        with self.__lock:
            self.__release_start = time.time()
            self.__dwell_times.append(self.__release_start - self.__press_start)
