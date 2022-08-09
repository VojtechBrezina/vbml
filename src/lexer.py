import re
import sys
from collections import deque
from . import logging
import colorama

token_kinds = [
    ('terminator', 
        re.compile(r'\[\]', re.S), 
        lambda m: None),
    ('command', 
        re.compile(r'\[[a-z]+?\]', re.S),
        lambda m: m[1:-1]),
    ('text', 
        re.compile(r'(([^\[\]])|(\[\[)|(\]\]))+', re.S),
        lambda m: m.replace('[[', '[').replace(']]', ']')),
]

class Token:
    def __init__(self, kind, data, line, start, end):
        self.kind = kind
        self.data = data
        self.line = line
        self.start = start
        self.end = end

    def __repr__(self):
        result = f'{colorama.Fore.GREEN}{self.kind}{colorama.Fore.RESET}'
        if self.data:
            result += \
                f'({colorama.Fore.YELLOW}{self.data}{colorama.Fore.RESET})'
        return result

def tokenize(code):
    tokens = deque()
    for ln, line in enumerate(code):
        if line.strip() == '':
            line = '[pend]'
        ln += 1
        index = 0
        while index < len(line):
            for kind, pattern, get_data in token_kinds:
                match = pattern.match(line[index:])
                if match:
                    token = Token(
                        kind, 
                        get_data(match[0]), 
                        ln - 1, index, index + match.end()
                    )
                    if kind != 'text':
                        tokens.append(token)
                    else:
                        if len(tokens) and tokens[-1].kind == 'text':
                            tokens[-1].data += ' ' + token.data
                        else:
                            tokens.append(token)
                    index += match.end()
                    break
            else:
                where = logging.highlight(line, colorama.Fore.YELLOW, colorama.Fore.RED, index, index + 1)
                logging.log('[tokenize]', logging.ERROR,
                    f'Unknown token at {ln}:{index + 1}: {where}')
                index += 1

    for token in tokens:
        logging.log('[tokenize]', logging.DEBUG, token)
    return tokens

