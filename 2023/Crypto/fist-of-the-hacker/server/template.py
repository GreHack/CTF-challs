import json
import numpy as np


class Template:

    def __init__(self, passphrase, times):
        self.passphrase = passphrase
        self.times = np.array(times, dtype=np.float64)

    def serialize(self):
        return json.dumps({
            'passphrase': self.passphrase,
            'times': list(self.times),
        }).encode()

    @staticmethod
    def deserialize(data: str):
        template_data = json.loads(data)
        return Template(
            template_data['passphrase'],
            np.array(template_data['times'], dtype=np.float64)
        )
