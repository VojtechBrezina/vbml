import colorama
import sys

DEBUG = 0
INFO = 1
WARNING = 2
ERROR = 3

colors = {
    DEBUG: colorama.Fore.GREEN,
    INFO: colorama.Fore.CYAN,
    WARNING: colorama.Fore.YELLOW,
    ERROR: colorama.Fore.RED,
}

def log(tag, level, message, indent=0):
    print(f'{colors[level]}{tag}{colorama.Fore.RESET}: {" " * (4 * indent)}{message}', file=sys.stderr)
