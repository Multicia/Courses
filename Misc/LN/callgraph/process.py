from __future__ import division 
def intermediate_dot():
  '''Generate a dictionary with relationship as key and number of connections'''
  raw_data = {}
  import glob
  files = glob.glob("*.dot")
  for fil in files:
    print fil
    with open(fil) as f:
      for line in f:
        if "->" in line: 
          key = line.split(';')[0]
          if key in raw_data: raw_data[key]+=1
          else:
            raw_data[key]=1
  return raw_data

def generate_new_dot_file(dictionary):
  f = open('new_output.dot','w')
  f.write('digraph G {\n')
  f.write('rankdir=LR\n')
  import operator
  max_val = max(dictionary.iteritems(), key=operator.itemgetter(1))[1]
  print max_val
  for key in dictionary.keys():
    temp = key +' [penwidth='+str(float(dictionary[key]/max_val)*2)+'];\n'
    f.write(temp)
  f.write('}\n')
  f.close()

if __name__ == '__main__':
  inter_data = intermediate_dot()
  generate_new_dot_file(inter_data)
  import os
  os.system('dot -Tpng new_output.dot > new_output.png')

