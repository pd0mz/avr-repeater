import sys

chars = {
    'a': 'B101',
    'b': 'B11000',
    'c': 'B11010',
    'd': 'B1100',
    'e': 'B10',
    'f': 'B10010',
    'g': 'B1110',
    'h': 'B10000',
    'i': 'B100',
    'j': 'B10111',
    'k': 'B1101',
    'l': 'B10100',
    'm': 'B111',
    'n': 'B110',
    'o': 'B1111',
    'p': 'B10110',
    'q': 'B11101',
    'r': 'B1010',
    's': 'B1000',
    't': 'B11',
    'u': 'B1001',
    'v': 'B10001',
    'w': 'B1011',
    'x': 'B11001',
    'y': 'B11011',
    'z': 'B11100',
    '0': 'B11111',
    '1': 'B01111',
    '2': 'B00111',
    '3': 'B00011',
    '4': 'B00001',
    '5': 'B00000',
    '6': 'B10000',
    '7': 'B11000',
    '8': 'B11100',
    '9': 'B11110',
    '/': 'B11010',
}

print '// CID for "%s"' % sys.argv[1]
masks = filter(None, map(lambda char: chars.get(char), sys.argv[1]))
print 'const morsemask_t CID[%d] = {' % len(masks)
for mask in masks:
    print '    %s,' % mask
print '};'
