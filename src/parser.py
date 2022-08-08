from src.util import logging

class Node():
    def __init__(self, name):
        self.name = name
        self.children = []
        pass

    def generate(self, document):
        pass

    def log(self, indent):
        logging.log('[parse]', logging.DEBUG, f'{self.name}:', indent)
        for child in self.children:
            child.log(indent + 1)

class ChunkNode(Node):
    def __init__(self, tokens):
        super().__init__('Chunk')

        while(len(tokens)):
            token = tokens.popleft()
            if token.kind == 'text':
                self.children.append(TextNode(token.data))
            elif token.kind == 'command':
                self.children.append(TextNode(token.data))
            else:
                logging.log('[parse]', logging.ERROR, f'Unexpected terminator')


class TextNode(Node):
    def __init__(self, content):
        super().__init__(f'Text({content})')
        self.content = content

def parse(tokens):
    return ChunkNode(tokens)
