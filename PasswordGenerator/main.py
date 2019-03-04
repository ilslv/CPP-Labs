import random
import string
import pyperclip
from colorama import Fore, init
import argparse
init()
parser = argparse.ArgumentParser()
parser.add_argument('-l', '--length', type=int, help='Password length')
parser.add_argument('-d', '--digits', action='store_true', help='Add digits')
parser.add_argument('-p', '--punctuations', action='store_true', help='Add special symbols')
parser.add_argument('-q', '--quantity', type=int, default=1, help='Number of passwords')
args = parser.parse_args()
symbols = string.ascii_letters
if args.digits:
    symbols += string.digits
if args.punctuations:
    symbols += string.punctuation

s = ''
for j in range(args.quantity):
    for i in range(args.length):
        s += random.choice(symbols)
    s += ' '
s = s[:-1]
pyperclip.copy(s)
print('Password {0} copied to clipboard'.format(Fore.YELLOW + s + Fore.RESET))
