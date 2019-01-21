import sys
import xxhash

filename = sys.argv[1]

with open(filename, 'r') as f:
    with open('hash_sorted.dat', 'w') as g:
        for line in f:
            g.write(xxhash.xxh64(line.rstrip()).hexdigest() + "\n")
