import json
from template import Template
from reference_template import ReferenceTemplate


class Challenge:

    __FLAG = 'GH{7yp1ng_15_4ll_y0u_n33d}'
    __THRESHOLD = 500.

    def __init__(self):
        with open('reference_template', 'rb') as file:
            self.__reference_template = ReferenceTemplate.deserialize(file.read().strip())

    def run(self):
        try:
            data = input()
        except EOFError:
            self.warn_hacker()
            return
        try:
            template = Template.deserialize(data)
        except:
            self.warn_hacker()
            return
        try:
            score = self.__reference_template.one2one(template)
        except:
            self.warn_hacker()
            return
        authenticated = int(score < self.__THRESHOLD)
        print(json.dumps({
            'score': score,
            'authenticated': authenticated,
            'flag': self.__FLAG if authenticated else 'Nope',
        }))

    def warn_hacker(self):
        print(json.dumps({
            'error': 'No nasty hacker allowed'
        }))


if __name__ == '__main__':
    chall = Challenge()
    chall.run()
