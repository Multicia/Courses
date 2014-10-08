from __future__ import division 
import sys 
import random
import math 
import numpy as np
from where_mod import *
sys.dont_write_bytecode = True
rand=random.random()
# consist of dictionary where the index is 
# 100*xblock+yblock and 
dictionary ={} 

def neighbourhood(xblock,yblock):
  print (xblock-1)%8,(yblock-1)%8,energy((xblock-1)%8,(yblock-1)%8)
  print (xblock-1)%8,(yblock),energy((xblock-1)%8,(yblock))
  print (xblock-1)%8,(yblock+1)%8,energy((xblock-1)%8,(yblock+1)%8)
  print (xblock)%8,(yblock-1)%8,energy((xblock)%8,(yblock-1)%8)
  print (xblock)%8,(yblock+1)%8,energy((xblock)%8,(yblock+1)%8)
  print (xblock+1)%8,(yblock-1)%8,energy((xblock+1)%8,(yblock-1)%8)
  print (xblock+1)%8,(yblock)%8,energy((xblock+1)%8,(yblock)%8)
  print (xblock+1)%8,(yblock+1)%8,energy((xblock+1)%8,(yblock+1)%8)
  print "========================"

def stats(listl):
  from scipy.stats import scoreatpercentile
  q1 = scoreatpercentile(listl,25)
  q3 = scoreatpercentile(listl,75)  
  #print "IQR : %f"%(q3-q1)
  #print "Median: %f"%median(listl)
  return median(listl),(q3-q1)

def energy(xblock,yblock):
  tempIndex=100*xblock+yblock
  energy=[]
  try:
    for x in dictionary[tempIndex]:
      energy.append(np.sum(x.obj))      
    median,iqr=stats(energy)
    print "%d, %f, %f"%(len(dictionary[tempIndex]),median,iqr),
  except:
    print "0, Cell Empty",
  return " "
  
def interpolate(xblock,yblock):
  

def decisionMaker(xblock,yblock):
  threshold=4
  index=xblock*100+yblock
  #If number of elements less than a threshold, create new points
  if(len(dictionary[index])<threshold):
    print "Create New Points"

  
def main():
  chessBoard = whereMain()
  x= int(8*random.random())
  y= int(8*random.random()) 
  #print x,y
  for i in range(1,9):
      for j in range(1,9):
          temp=[]
          for x in chessBoard:
              if x.xblock==i and x.yblock==j:
                  temp.append(x)
          if(len(temp)!=0):
            #print "tempList",
            #print temp[0].xblock,temp[0].yblock,len(temp)
            index=temp[0].xblock*100+temp[0].yblock
            dictionary[index] = temp
            assert(len(temp)==len(dictionary[index])),"something"
            #print dictionary[index][0].xblock
  #print (dictionary.keys())
  print "Elements: %d"%len(dictionary[506])
  neighbourhood(5,6)
  #decisionMaker(5,6)

def _neighbourhood():
  neighbourhood(0,0)
  neighbourhood(7,7)
  neighbourhood(0,7)
  neighbourhood(7,0)
  neighbourhood(0,4)
  neighbourhood(4,0)
  neighbourhood(7,4)
  neighbourhood(4,7)
  

if __name__ == '__main__':
  main()
  #_neighbourhood()
