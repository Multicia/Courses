import ast

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
            self.listL.append(str(node.func.value.id+"."+node.func.attr))
          except:
            try:
              self.listL.append(str(node.func.value.value.id+"."+node.func.value.attr))
            except:
              pass

    @recursive
    def generic_visit(self,node):
        #print(type(node).__name__)
        pass

class start_end(ast.NodeVisitor):
  listL=[]
  def visit_FunctionDef(self,node):
    """ visit a Function node and visits it recursively"""
    temp = store_method()
    temp.start = node.body[0].lineno-1
    temp.end = node.body[len(node.body)-1].lineno
    temp.name = node.name
    self.listL.append(temp)
    

def get_call_sequence(func_name): 
  x = RecursiveVisitor()
  y = start_end()
  f = open("input1.py","r")
  t = ast.parse(f.read())
  y.visit(t)
  for i in y.listL: 
    print
    print i
    f = open("input1.py","r")
    lines = f.readlines()
    code = [lines[line] for line in xrange(i.start-1,i.end)]
    t = ast.parse(''.join(code))
    x.visit(t)
    print x.listL
    x.clear()  
  #print t.body[0].value

if __name__ == '__main__':
  get_call_sequence("asd")
 
