def convert_html_entities(text):
    return text \
        .replace('<', '&lt;') \
        .replace('>', '&gt;')

class Document():
    def __init__(self):
        self.content = None
        self.paragraph = None
        self.inside_paragraf = None

    def begin(self):
        self.paragraph = ''
        self.content = \
            '<!doctype html><html><head></head><body>'
        self.inside_paragraf = True

    def end(self):
        self.paragraph_end()
        self.content += '</body></html>'

    def write(self, what, raw):
        if not raw:
            what = convert_html_entities(what)

        if self.inside_paragraf:
            self.paragraph += what
        else:
            self.content += what

    def paragraph_begin(self):
        self.inside_paragraf = True

    def paragraph_end(self):
        self.inside_paragraf = False
        if self.paragraph.strip() != '':
            self.content += f'<p>{self.paragraph}</p>'
        self.paragraph = ''

def generate(tree):
    document = Document()

    document.begin()
    tree.generate(document)
    document.end()

    return document
