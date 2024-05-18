import json
import numpy as np


class ReferenceTemplate:

    def __init__(self, passphrase, times):
        self.__passphrase = passphrase
        times = np.array(times)
        self.__mean_times = times.mean(axis=0)
        self.__mad_times = np.median(times - self.__mean_times, axis=0)

    def one2one(self, template):
        if self.__passphrase == template.passphrase:
            return np.abs(((template.times - self.__mean_times) / self.__mad_times)).sum()
        return np.inf

    def serialize(self):
        return json.dumps({
            'passphrase': self.__passphrase,
            'mean_times': list(self.__mean_times),
            'mad_times': list(self.__mad_times),
        }).encode()

    @staticmethod
    def deserialize(data):
        template_data = json.loads(data.decode())
        template = ReferenceTemplate('', [0.])
        template.__passphrase = template_data['passphrase']
        template.__mean_times = np.array(template_data['mean_times'], dtype=np.float64)
        template.__mad_times = np.array(template_data['mad_times'], dtype=np.float64)
        return template
