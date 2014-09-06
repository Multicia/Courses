'''
Created on Sep 5, 2014

@author: vivek
'''
from __future__ import division 
import sys 
import random
import math 
import pylab as pl
sys.dont_write_bytecode = True

def say(x): 
  sys.stdout.write(str(x)); sys.stdout.flush()
  
class Seive:
    @staticmethod
    def slope(x1,y1,x2,y2):
        print x1,y1,x2,y2
        try:
            return (y2-y1)/(x2-x1)
        except ZeroDivisionError:
            return 0    
    def algorithm(self,data):
       interval = int(len(data)/10)
       arrSlope =[]
       for i in range(0,interval):
           #need to add checks for out of bounds
         arrSlope.append(Seive.slope(data[i*10][0],data[i*10][1],data[(i*10)+1][0],data[(i*10)+1][1]))
       return arrSlope
   
    def slopeCheck(self,arrSlope,data):
        #arrSlope = [0,1,2,3,4,5,6,7,8,9]
        for x in range(0,len(arrSlope)-1):
            if(arrSlope[x]*arrSlope[x+1] <0):
                print "Slope or Hole Detected : ",
                print x
                print data[x*10:(x+1)*10]
                           
       
         

class Data:
    xsort=[]
    def plotting(self,data):
        print 'itstarts'
        x = [row[0] for row in data]
        y = [row[1] for row in data]
        pl.plot(x,y)
        pl.show()
        
    def generation(self):
        startx=starty=0
        endx=100
        endy=10
        coordinates = []
        for x in range(1,100):
            a= 1+random.randint(startx,endx)
            b= starty + (random.random()*(endy-starty))
            coordinates.append([a,b])
        xsort=sorted(coordinates,key=lambda l:l[0])
        return xsort
#         print len(xsort) 
#         print "======================"
#         for i in range(0,len(xsort)):
#             print " %d %d,%f"%(i,xsort[i][0],xsort[i][1])   

def doSomethingCool():
    data = Data()
    x=data.generation()
    s = Seive()
    arrSlope = s.algorithm(x)
    s.slopeCheck(arrSlope,x)
    data.plotting(x)

if __name__ == '__main__': 
  doSomethingCool();
