from . import logging
from collections import namedtuple
import colorama

class Node():
    def __init__(self, name, params = []):
        self.name = f'{colorama.Fore.GREEN}{name}{colorama.Fore.RESET}'
        if len(params):
            self.name += '('
            self.name += ', '.join([
                f'{colorama.Fore.YELLOW}{param}{colorama.Fore.RESET}'
                for param in params
            ])
            self.name += ')'
        self.name += ':'
        self.children = []
        pass

    def generate(self, document):
        pass

    def log(self, indent):
        logging.log('[parse]', logging.DEBUG, self.name, indent)
        for child in self.children:
            child.log(indent + 1)

class CommandNode(Node):
    def __init__(self, name, param_info, tokens, code):
        super().__init__('Command', [name])
        for type in param_info:
            if type == 'text':
                token = tokens.popleft()
                if token.kind == 'text':
                    self.children.append(TextNode(token.data))
                else:
                    line = logging.highlight(
                        code[token.line],
                        colorama.Fore.YELLOW, colorama.Fore.RED,
                        token.start, token.end
                    )
                    logging.log(
                        '[parse]',
                        logging.ERROR,
                        f'Unexpected {token.kind} at {token.line + 1}:{token.start + 1}: {line}'
                    )
                    self.children.append(TextNode('ERROR'))
                token = tokens.popleft()
                if token.kind != 'terminator':
                    line = logging.highlight(
                        code[token.line],
                        colorama.Fore.YELLOW, colorama.Fore.RED,
                        token.start, token.end
                    )
                    logging.log(
                        '[parse]',
                        logging.ERROR,
                        f'Unexpected {token.kind} at {token.line + 1}:{token.start + 1}: {line}'
                    )
                    self.children.append(TextNode('ERROR'))

            elif type == 'chunk':
                self.children.append(ChunkNode(tokens, code, True))

class BCommandNode(CommandNode):
    def __init__(self, tokens, code):
        super().__init__('b', ['chunk'], tokens, code)

    def generate(self, document):
        document.write('<b>', True)
        self.children[0].generate(document)
        document.write('</b>', True)

class ICommandNode(CommandNode):
    def __init__(self, tokens, code):
        super().__init__('i', ['chunk'], tokens, code)

    def generate(self, document):
        document.write('<i>', True)
        self.children[0].generate(document)
        document.write('</i>', True)

class IcCommandNode(CommandNode):
    def __init__(self, tokens, code):
        super().__init__('ic', ['chunk'], tokens, code)

    def generate(self, document):
        document.write('<code>', True)
        self.children[0].generate(document)
        document.write('</code>', True)

class BrCommandNode(CommandNode):
    def __init__(self, tokens, code):
        super().__init__('br', [], tokens, code)

    def generate(self, document):
        document.write('<br>', True)

class LineCommandNode(CommandNode):
    def __init__(self, tokens, code):
        super().__init__('line', [], tokens, code)

    def generate(self, document):
        document.write('<hr>', True)

class HCommandNode(CommandNode):
    def __init__(self, tokens, code):
        super().__init__('h', ['text', 'chunk'], tokens, code)

    def generate(self, document):
        document.paragraph_end()
        document.write(f'<h{self.children[0].content}>', True)
        self.children[1].generate(document)
        document.write(f'</h{self.children[0].content}>', True)
        document.paragraph_begin()

class PendCommandNode(CommandNode):
    def __init__(self, tokens, code):
        super().__init__('pend', [], tokens, code)

    def generate(self, document):
        document.paragraph_end()
        document.paragraph_begin()

def parse(tokens, code):
    tree = ChunkNode(tokens, code, False)
    tree.log(0)
    return tree

commands = {
    'b': BCommandNode,
    'i': ICommandNode,
    'ic': IcCommandNode,
    'br': BrCommandNode,
    'line': LineCommandNode,
    'h': HCommandNode,
    'pend': PendCommandNode,
}

class ChunkNode(Node):
    def __init__(self, tokens, code, terminated):
        super().__init__('Chunk')

        while(len(tokens)):
            token = tokens.popleft()
            if token.kind == 'text':
                self.children.append(TextNode(token.data))
            elif token.kind == 'command':
                if token.data in commands:
                    self.children.append(commands[token.data](tokens, code))
                else:
                    line = logging.highlight_error(
                        code[token.line], token.start, token.end
                    )
                    logging.log(
                        '[parse]', logging.ERROR, 
                        f'Unknown command {colorama.Fore.RED}\'{token.data}\'{colorama.Fore.RESET} at {token.line + 1}:{token.start + 1}: {line}'
                    )
            else:
                if terminated:
                    break

                line = logging.highlight_error(
                    code[token.line], token.start, token.end
                )
                logging.log(
                    '[parse]', logging.ERROR,
                    f'Unexpected terminator at {token.line + 1}:{token.start + 1}: {line}'
                )

    def generate(self, document):
        for child in self.children:
            child.generate(document)

class TextNode(Node):
    def __init__(self, content):
        super().__init__('Text', [content])
        self.content = content

    def generate(self, document):
        document.write(self.content, False)
