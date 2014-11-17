from __future__ import division
import sys
import random
import math

def get_list(filename):
  f = open(filename,"r")
  for line in f:
    print line.split(' ')
    break
