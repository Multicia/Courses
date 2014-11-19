import ast
from construct_call_graph import generate_call_sequence

class store_method():
  def __init__(self):
    self.name = None
    self.start = -1
    self.end = -1
    self.calls = []
  def __str__(self):
    return "Name: %s \n Start: %s \n End: %s \n" % (self.name, self.start,self.end)

class RecursiveVisitor(ast.NodeVisitor):
    """ example recursive visitor """
    listL=[]
    def clear(self): self.listL = []
    def recursive(func):
        """ decorator to make visitor work recursive """
        def wrapper(self,node):
            func(self,node)
            for child in ast.iter_child_nodes(node):
                self.visit(child)
        return wrapper

    @recursive
    def visit_Call(self,node):  
        try:
          self.listL.append(node.func.id)
        except:
          try:
            self.listL.append(str(node.func.value.id+"_"+node.func.attr))
          except:
            try:
              self.listL.append(str(node.func.value.value.id+"_"+node.func.value.attr))
            except:
              pass

    @recursive
    def generic_visit(self,node):
        #print(type(node).__name__)
        pass

class start_end(ast.NodeVisitor):
  listL=[]
  def clear(self): self.listL = []
  def visit_FunctionDef(self,node):
    """ visit a Function node and visits it recursively"""
    temp = store_method()
    temp.start = node.body[0].lineno-1
    temp.end = node.body[len(node.body)-1].lineno
    temp.name = node.name
    self.listL.append(temp)
    pass
    

def get_call_sequence(input_file,func_name): 
  x = RecursiveVisitor()
  y = start_end()
  y.clear()
  f = open(input_file,"r")
  t = ast.parse(f.read())
  y.visit(t)
  for i in y.listL: 
    if i.name == func_name:
      f1 = open("input1.py","r")
      lines = f1.readlines()
      code = [lines[line] for line in xrange(i.start-1,i.end)]
      t = ast.parse(''.join(code))
      x.clear() 
      x.visit(t)
      i.calls = x.listL[:]
      i.calls.insert(0,i.name)
      i.calls.insert(len(i.calls),i.name)
      return i
  #print t.body[0].value

def read_from_file(input_file):
  """reads a file(.dot file) and returns a list"""
  assert(input_file.split(".")[1] == "dot"),"not a dot file"
  f = open(input_file,"r")
  listL = f.readlines()
  del listL[-1]
  return listL

def unique_function_call(input_file):
  """returns unique calls from a .dot file"""
  assert(input_file.split(".")[1] == "dot"),"not a dot file"
  f = open(input_file,"r")
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

def append_calls_file(calls,source_file):
  from itertools import tee, izip
  def pairwise(iterable):
    "s -> (s0,s1), (s1,s2), (s2, s3), ..."
    a, b = tee(iterable)
    next(b, None)
    return izip(a, b)
  returnList = []
  assert(source_file.split(".")[1] == "py"),"not a py file"

  for call in calls:    
    call_sub = get_call_sequence(source_file,call) #call_sub is of type store_method()
    if len(call_sub.calls) == 0 : continue
    for cfrom,cto in pairwise(call_sub.calls):
      temp = cfrom + "->" +cto
      #print cfrom,cto
      #print temp
      returnList.append(temp)
  return returnList

 
def remove_node(node_name,dot_file):
  """Removes """
  def modify_file(node_name,dot_file,add_str):
    f = open(dot_file,"r")
    lines = f.readlines()
    f.close()
    f = open(dot_file,"w")
    for line in lines:
      if node_name in line: continue
      elif "}" in line: f.write(add_str)
      else: f.write(line)
    f.close() 
  
  def combine_funcs(to_fns,frm_fns):
    ret_str = ""
    import itertools
    tempL = list(itertools.product(to_fns,frm_fns))
    for item in tempL:ret_str += item[0] + "->" + item[1] +";\n"
    return ret_str + "}\n"

  listL = read_from_file(dot_file)  
  to_fns = []
  frm_fns = []
  for item in listL:
    item = item.replace(";\n","")
    if "->" not in item: continue
    to_fn,frm_fn = item.split("->")
    if(to_fn == frm_fn): continue  #cases where there is a self loop
    if(to_fn == node_name): frm_fns.append(frm_fn)
    if(frm_fn == node_name): to_fns.append(to_fn)
  modify_file(node_name,dot_file,combine_funcs(to_fns,frm_fns))


def wrapper_remove_node(dot_file,tfidf_list):
  assert(dot_file.split(".")[1] == "dot"),"not a dot file"
  flist = unique_function_call(dot_file)
  for item in tfidf_list: flist.remove(item)
  for item in flist: remove_node(item,dot_file)

if __name__ == '__main__':
  #remove_node("pylab_plot","input1.dot")
  #print unique_function_call("input1.dot")
  wrapper_remove_node("input1.dot",["acc","abs","len"])
  """
  #get_call_sequence("test")
  source_file = "input1.py"
  dot_file = generate_call_sequence(source_file)
  #print "Source File: ",source_file," DOT file: ",dot_file
  call_list = read_from_file(dot_file)
  call_list_str = ''.join(call_list)
  list_L = unique_function_call(dot_file)
  new_list = append_calls_file(list_L,source_file)
  print new_list
  new_list_str = ';\n'.join(new_list)
  call_list_str= call_list_str+new_list_str+"\n}"
  with open(dot_file, "w") as myfile:
    myfile.write(call_list_str)
  import os
  os.system("cat input1.dot | dot -Tpng > %s.png"%source_file.split(".")[0])
  """
 
