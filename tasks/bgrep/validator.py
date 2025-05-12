#!/usr/bin/python3
import sys
import subprocess

output = sys.argv[1]
binary = sys.argv[2]
args_file = sys.argv[3]

with open(args_file) as fin:
    args = fin.readline().strip().split('|')
args = [binary] + args

actual = subprocess.check_output(args).decode('utf-8')

with open(output) as fin:
    expected = fin.read()
print("Got result")
print(actual)

print()
print("Expected")
print(expected)
assert(actual == expected)
