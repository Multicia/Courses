from __future__ import division 
def intermediate_dot(fil):
  '''Generate a dictionary with relationship as key and number of connections'''
  raw_data = {}
  with open(fil) as f:
    for line in f:
      if "->" in line: 
        key = line.split(';')[0]
        if key in raw_data: raw_data[key]+=1
        else:
          raw_data[key]=1
  return raw_data

#TODO: Need to remove the dictionary. It's redundant
def generate_new_dot_file(dot_file,dictionary,style,colors=False):
  print dot_file
  if colors == True:
    fil = open(dot_file,"r")
    lines = fil.readlines()
    fil.close()
  f = open('new_output.dot','w')
  f.write('digraph G {\n')
  f.write('rankdir=LR\n')
  if colors == True:
    print "COLORS introduced"
    color_data = color_processing(lines)
    print color_data
    for key in color_data.keys():
      incmng = color_data[key]/10000
      outgng = color_data[key]%10000
      ratio = (incmng + outgng)**-1 * incmng
      strg = key + "[label=\"%s\", shape=\"egg\" style=\"radial\" fillcolor=\"green;%f:red\"]"%(key,ratio)
      f.write(strg)
  import operator
  max_val = max(dictionary.iteritems(), key=operator.itemgetter(1))[1]
  for key in dictionary.keys():
    if "print" in key: continue
    if style == "penwidth":
      temp = key.replace("\n","") 
      temp += ' [penwidth='+str(float(dictionary[key]/max_val)*2)+'];\n'
    else: temp = key + ";\n"
    f.write(temp)
  f.write('}\n')
  f.close()

def dot_processing(dot_file,style="",color=False):
  inter_data = intermediate_dot(dot_file)
  generate_new_dot_file(dot_file,inter_data,style,color)

def color_processing(lines):
  def remove_penwidth(line):
    '''removes pen width from the line'''
    spos = line.index('[')
    epos = line.index(']')
    return line[:spos]
  color_data = {}
  #with open(dot_file) as f:
  for line in lines:
      if "->" in line: 
        key = line.split(";")[0]
        if '[' in key: key = remove_penwidth(key)
        to = key.split("->")
        if len(to) > 2: print "complain" #potential bug: there can be lines with multuple '->' another bug!!
        if to[0] in color_data: color_data[to[0]]+=10000
        else: color_data[to[0]]=10000
        if to[1] in color_data: color_data[to[1]]+=1
        else: color_data[to[1]]=1
        
  return color_data

if __name__ == '__main__':
  inter_data = intermediate_dot(dot_file)
  generate_new_dot_file(inter_data)
  import os
  os.system('dot -Tpng new_output.dot > new_output.png')
  #color_processing("new_output.dot")


