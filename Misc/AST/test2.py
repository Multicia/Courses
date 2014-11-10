'''
import ast,sys
class v(ast.NodeVisitor):
   def generic_visit(self, node):
     #print type(node).__name__
     ast.NodeVisitor.generic_visit(self, node)

class w(v):
  print
  #def visit_Name(self,node):print "Name: ",node.id
  def visit_FunctionDef(self,node):
    print "Function: ",node.name#args.args#._fields
    yield node
  def visit_Call(self,node):
    try:
      print "Called out: ------------------", node.func.id#__dict__#attr
    except: 
     print 
     pass
    #for property, value in vars(node.func.ctx).iteritems():
    #  print property, ": ", value
'''

import ast

class RecursiveVisitor(ast.NodeVisitor):
    """ example recursive visitor """

    def recursive(func):
        """ decorator to make visitor work recursive """
        def wrapper(self,node):
            func(self,node)
            for child in ast.iter_child_nodes(node):
                self.visit(child)
        return wrapper

    @recursive
    def visit_Assign(self,node):
        """ visit a Assign node and visits it recursively"""
        pass
        #print(type(node).__name__)

    @recursive
    def visit_BinOp(self, node):
        """ visit a BinOp node and visits it recursively"""
        pass
        #print(type(node).__name__)

    @recursive
    def visit_Call(self,node):
        """ visit a Call node and visits it recursively"""
        print(type(node).__name__),
        try:
          print " : ",node.func.id#_fields
        except:
          print "Wrong: ",node.func._fields
        

    @recursive
    def visit_Lambda(self,node):
        """ visit a Function node """
        pass
        #print(type(node).__name__)

    @recursive
    def visit_FunctionDef(self,node):
        """ visit a Function node and visits it recursively"""
        print(type(node).__name__),
        print " : ",node.name
        print 

    @recursive
    def visit_MethodDef(self,node):
        """ visit a Method node and visits it recursively"""
        print(type(node).__name__),
        print " :-------------------------- ",node.name
        print 

    @recursive
    def visit_Module(self,node):
        """ visit a Module node and the visits recursively"""
        pass

    def generic_visit(self,node):
        pass

 
x = RecursiveVisitor()
f = open("main.py","r")
t = ast.parse(f.read())
#print ast.dump(t)
x.visit(t)
