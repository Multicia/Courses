from __future__ import division 
import sys
sys.dont_write_bytecode = True


def unique_function_call(file):
  f = open(file,"r")
  calls = set()
  for line in f:
    line = line.replace(" ","").replace("\n","").replace(";","")
    #if line == "digraphG{" or line == "}" or line == "rankdir=LR": pass
    if "None" in line:pass
    elif "{" in line: pass
    elif "=" in line:pass
    elif "}" in line:pass
    elif "->" not in line: calls.add(line)
    else:
      func1,func2=line.split("->")
      calls.add(func1)
      calls.add(func2)
  f.close()
  return calls

if __name__ == '__main__':
  calls = unique_function_call("input1.dot")
