from __future__ import division
import sys
import random
import math
#import numpy as np
from models import *
from searchers import *
from options import *
from utilities import *
from sk import *
sys.dont_write_bytecode = True
#Dr.M
rand=  random.random # generate nums 0..1
any=   random.choice # pull any from list
sqrt= math.sqrt  #square root function

def display(modelName,searcher,runTimes,scores,historyhi=[],historylo=[]):
  assert(len(runTimes) == len(scores)),'Ouch! it hurts'
  print "==============================================================="
  print "Model Name: %s"%modelName
  print "Searcher Name: %s"%searcher
  print "Options Used: ",
  print myoptions[searcher]
  import time
  print ("Date: %s"%time.strftime("%d/%m/%Y"))
  print "Average running time: %f " %mean(runTimes)
  if(len(historyhi)!=0):
    for x in xrange(myModelobjf[modelName]):
      print "Objective No. %d: High: %f Low: %f"%(x+1,historyhi[x],historylo[x])
  #for i in range(0,len(runTimes)):
  #  print "RunNo: %s RunTime: %s Score: %s"%(i+1,runTimes[i],scores[i])
  #print scores
  print xtile(scores,width=25,show=" %1.2f")
  print "==============================================================="



def median(lst):
    sortedLst = sorted(lst)
    lstLen = len(lst)
    index = (lstLen - 1) // 2

    if (lstLen % 2):
        return sortedLst[index]
    else:
        return (sortedLst[index] + sortedLst[index + 1])/2.0

def multipleRun(options):
   def option_update(new):
     print "optionsupdate"
     myoptions['Seive4']['initialpoints'] = int(new[0])
     myoptions['Seive4']['intermaxlimit'] = int(new[1])
     myoptions['Seive4']['extermaxlimit'] = int(new[2])
     myoptions['Seive4']['f'] = new[3]
     myoptions['Seive4']['cf'] = new[4]
     myoptions['Seive4']['interpolationpoint'] = int(new[5])

   option_update(options)
   say("multiplerun")
   from collections import defaultdict
   r = 3
   median_scores = []
   for klass in [DTLZ1]:#,DTLZ2,DTLZ3,DTLZ4,DTLZ5,DTLZ6,DTLZ7]:
     #print "Model Name: %s"%klass.__name__
     eraCollector=defaultdict(list)
     tempC = klass()
     import time
     #print ("Date: %s"%time.strftime("%d/%m/%Y"))
     #bmin,bmax = tempC.baseline(tempC.minR, tempC.maxR) 
     bmin = -266.409
     bmax = 632.653
     #print "Baseline Finished: ",bmin,bmax

     for searcher in [Seive4]:
       n = 0.0
       listTimeTaken = []
       listScores = []
       list_eval = []
       random.seed(6)
       #print searcher.__name__,
       for _ in range(r):
         test = searcher(klass(),"display2",bmin,bmax)
         say(".")
         import time
         t1 = time.time()
         solution,score,model = test.evaluate()
         listScores.append(score)
     median_scores.append(median(listScores))
   return median_scores
     
def trim(minR,maxR,x,i)  : # trim to legal range
    return max(minR[i], min(x, maxR[i]))

def threeOthers(frontier,one):
    say("threeOthers")
    seen = [one]
    def other():
      #print "other"
      for i in xrange(len(frontier)):
        while True:
          k = random.randint(0,len(frontier)-1)
          #print "%d"%k
          if frontier[k] not in seen:
            seen.append(frontier[k])
            break
        return frontier[k]
    this = other()
    that = other()
    then = other()
    return this,that,then


def extrapolate(minR,maxR,frontier,one,f,cf):
    say("Extrapolate")
    two,three,four = threeOthers(frontier,one)
    #print two,three,four
    solution=[]
    for d in xrange(len(two)):
      x,y,z=two[d],three[d],four[d]
      if(random.random() < cf):
        solution.append(trim(minR,maxR,x + f*(y-z),d))
      else:
        solution.append(one[d]) 
    return solution


def update(minR,maxR,f,cf,frontier,total=0.0,n=0):
    def better(old,new):
      assert(len(old)==len(new)),"MOEAD| Length mismatch"
      for i in xrange(len(old)-1): 
        if old[i] >= new[i]: continue
        else: return False
      return True

    newF = []
    total,n=0,0
    for x in frontier:
      say("update")
      s = multipleRun(x)
      new = extrapolate(minR,maxR,frontier,x,f,cf)
      newe = multipleRun(new)
      if better(s,newe) == True:
        newF.append(new)
      else:
        newF.append(x)
    return newF

def evaluate(minR,maxR,repeat=3,np=20,f=0.75,cf=0.3):
    frontier = [[minR[i]+random.random()*(maxR[i]-minR[i]) for i in xrange(len(minR))]for _ in xrange(np)]
    #print frontier
    for i in xrange(repeat):
      frontier = update(minR,maxR,f,cf,frontier)
      print "#######"

    temp_minR = 9e10
    for x in frontier:
      energy = multipleRun(x)
      if(minR>energy):
        minR = energy
        solution=x 
    return solution



def paramenter_tuning():
  np = 20
  minR=[64,1,1,0.1,0.1,1] #classA
  maxR=[5000,30,30,1,0.3,20]  
  return evaluate(minR,maxR)

print paramenter_tuning()



