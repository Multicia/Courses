from __future__ import division 
import sys 
import random
import math 
import numpy as np
from where_mod import *
sys.dont_write_bytecode = True
rand=random.random
# consist of dictionary where the index is 
# 100*xblock+yblock and 
dictionary ={} 


def neighbourhood(xblock,yblock):
  def neighr(xblock,yblock):
    index=xblock*100+yblock
    try:
      #only return the neighbours who has threshold number of
      #elements in it.
      if(len(dictionary[index])>3):
        print "blah"
        return index
    except:
      return None
  neigbour =[] #typo
  neigbour.append(neighr((xblock-1)%8,(yblock-1)%8))
  neigbour.append(neighr((xblock-1)%8,(yblock)))
  neigbour.append(neighr((xblock-1)%8,(yblock+1)%8))
  neigbour.append(neighr((xblock)%8,(yblock-1)%8))
  neigbour.append(neighr((xblock)%8,(yblock+1)%8))
  neigbour.append(neighr((xblock+1)%8,(yblock-1)%8))
  neigbour.append(neighr((xblock+1)%8,(yblock)%8))
  neigbour.append(neighr((xblock+1)%8,(yblock+1)%8))
  print "========================"
  return neigbour

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

def interpolate(lx,ly,cr=1,fmin=0,fmax=1):
  assert(len(lx)==len(ly))
  genPoint=[]
  for i in xrange(len(lx)):
    x,y=lx[i],ly[i]
    rand = random.random()
    if rand < cr:
      probEx = fmin +(fmax-fmin)*random.random()
      new = min(x,y)+probEx*abs(x-y)
    else:
      new = y
    genPoint.append(new)
  return genPoint
  
#There are three points and I am trying to extrapolate.
def extrapolate(lx,ly,lz,cr=1,fmin=0.9,fmax=2):
  def lo(m)      : return 0.0
  def hi(m)      : return  5.0
  def trim(x)  : # trim to legal range
    return max(lo(x), x%hi(x))
  assert(len(lx)==len(ly)==len(lz))
  genPoint=[]
  for i in xrange(len(lx)):
    x,y,z = lx[i],ly[i],lz[i]
    rand = random.random()

    if rand < cr:
      probEx = fmin + (fmax-fmin)*random.random()
      print probEx
      new = trim(x + probEx*(y-z))
    else:
      new = y #Just assign a value for that decision
    genPoint.append(new)
  return genPoint



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
  print neighbourhood(5,6)
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

def _extrapolate():
  print extrapolate([2,2,2],[3,3,3],[1,1,1],fmin=0,fmax=0.1)

def _interpolate():
  print interpolate([3,3,3],[2,2,2])



if __name__ == '__main__':
  _interpolate()
  main()
  #_extrapolate()
  #_neighbourhood()
