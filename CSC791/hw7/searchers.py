from __future__ import division 
import sys 
import random
import math 
import numpy as np
from models import *
from options import *
from utilities import *
sys.dont_write_bytecode = True

#say = Utilities().say

class SearchersBasic():
  tempList=[]
  def display(self,score,printChar=''):
    self.tempList.append(score)
    if(self.displayStyle=="display1"):
      print(printChar),
  
  def display2(self):
    if(self.displayStyle=="display2"):
      #print xtile(self.tempList,width=25,show=" %1.6f")
      self.tempList=[]

class MaxWalkSat(SearchersBasic):
  model = None
  minR=0
  maxR=0
  random.seed(40)
  def __init__(self,modelName,displayS):
    self.model=modelName
    self.displayStyle=displayS

      

  def evaluate(self):
    model = self.model
    #print "Model used: %s"%model.info()
    minR=model.minR
    maxR=model.maxR
    maxTries=int(myoptions['MaxWalkSat']['maxTries'])
    maxChanges=int(myoptions['MaxWalkSat']['maxChanges'])
    n=model.n
    threshold=float(myoptions['MaxWalkSat']['threshold'])
    probLocalSearch=float(myoptions['MaxWalkSat']['probLocalSearch'])
    bestScore=100
    bestSolution=[]


    #print "Value of p: %f"%probLocalSearch
   # model = Fonseca()
    model.baseline(minR,maxR)
    #print model.maxVal,model.minVal
    
    for i in range(0,maxTries): #Outer Loop
      solution=[]
      for x in range(0,n):
        solution.append(minR[x] + random.random()*(maxR[x]-minR[x]))
      #print "Solution: ",
      #print solution  
      for j in range(1,maxChanges):      #Inner Loop
         score = model.evaluate(solution)
         #print score
         # optional-start
         if(score < bestScore):
           bestScore=score
           bestSolution=solution
           
         # optional-end
         if(score < threshold):
           #print "threshold reached|Tries: %d|Changes: %d"%(i,j)
           self.display(".",score),
           self.display2()
           self.model.evalBetter()    
           revN = model.maxVal-model.minVal
           return bestSolution,bestScore,self.model
         
         if(random.random() > probLocalSearch):
             c = int((self.model.n)*random.random())
             solution[c]=model.neighbour(minR,maxR,c)
             self.display(score,"+"),
         else:
             tempBestScore=score
             tempBestSolution=solution             
             c = int(self.model.n*random.random())
             interval = (maxR[c]-minR[c])/10
             for itr in range(0,10):                
                solution[c] = minR[c] + (itr*interval)*random.random()
                tempScore = model.evaluate(solution)
                if(tempBestScore > tempScore):     # score is correlated to max?
                  tempBestScore=tempScore
                  tempBestSolution=solution
             solution=tempBestSolution
             self.display(tempBestScore,"!"),
         self.display(score,"."),
         if(self.model.lives == 1):
           #print "DEATH"
           self.display2()
           self.model.evalBetter()
           revN = model.maxVal-model.minVal
           return bestSolution,bestScore,self.model
         
         if(j%50==0):
            #print "here"
            self.display2()
            self.model.evalBetter()
    revN = model.maxVal-model.minVal
    return bestSolution,bestScore,self.model      

def probFunction(old,new,t):
   return np.exp(1 *(old-new)/t)

class SA(SearchersBasic): #minimizing
  model = None
  minR=0
  maxR=0
  random.seed(1)
  def __init__(self,modelName,displayS):
    self.model=modelName
    self.displayStyle=displayS


  def neighbour(self,solution,minR,maxR):
    returnValue = []
    n=len(solution)
    for i in range(0,n):
      tempRand = random.random()
      if tempRand <(1/self.model.n):
        returnValue.append(minR[i] + (maxR[i] - minR[i])*random.random())
      else:
        returnValue.append(solution[i])
    return returnValue

  def evaluate(self):
    model=self.model
    #print "Model used: %s"%(model.info())
    minR = model.minR
    maxR = model.maxR
    model.baseline(minR,maxR)
    #print "MaxVal: %f MinVal: %f"%(model.maxVal, model.minVal)

    s = [minR[z] + (maxR[z] - minR[z])*random.random() for z in range(0,model.n)]
    #print s
    e = model.evaluate(s)
    emax = int(myoptions['SA']['emax'])
    sb = s                       #Initial Best Solution
    eb = e                       #Initial Best Energy
    k = 1
    kmax = int(myoptions['SA']['kmax'])
    count=0
    while(k <= kmax and e > emax):
      #print k,e
      sn = self.neighbour(s,minR,maxR)
      en = model.evaluate(sn)
      if(en < eb):
        sb = sn
        eb = en
        self.display(en,"."),#we get to somewhere better globally
      tempProb = probFunction(e,en,k/kmax)
      tempRand = random.random()
#      print " tempProb: %f tempRand: %f " %(tempProb,tempRand)
      if(en < e):
        s = sn
        e = en
        self.display(en,"+"), #we get to somewhere better locally
      elif(tempProb > tempRand):
        jump = True
        s = sn
        e = en
        self.display(en,"?"), #we are jumping to something sub-optimal;
        count+=1
      self.display(en,"."),
      k += 1
      if(self.model.lives == 0):
        self.display2()
        self.model.emptyWrapper()
        #print "out1" 
        revN = model.maxVal-model.minVal
        return sb,eb,self.model 
      
      if(k % 50 == 0):
         self.display2()
         self.model.evalBetter()
       #  print "%f{%d}"%(sb,count),
         count=0
    #print "out2"
    self.model.emptyWrapper()
    revN = model.maxVal-model.minVal
    return sb,eb,self.model

class GA(SearchersBasic):
  model = None
  minR=0
  maxR=0
  population={}
  random.seed(1)
  def __init__(self,modelName,displayS):
    self.model=modelName
    self.displayStyle=displayS
    self.crossoverRate = float(myoptions['GA']['crossOverRate'])
    self.mutationRate = 1/self.model.n
    self.elitismrank = int(myoptions['GA']['elitism'])
    self.generation = int(myoptions['GA']['generation'])

  def crossOver(self,listdaddy,listmommy):
    rate=self.crossoverRate
    #assert(len(listdaddy)==len(listmommy)),"Something's messed up"
    if(random.random()<rate):
      minR,maxR=0,len(listdaddy)
      tone = int(minR + random.random()*((maxR)-minR))
      ttwo = int(minR + random.random()*(maxR-(minR)))
      one,two=min(tone,ttwo),max(tone,ttwo)
      #print "CrossOver: %d %d "%(one,two)
      #if(one==two):two+=2+(minR+random.random()*(maxR-minR-two-2))
      newDaddy=listdaddy[:one]+listmommy[one:two]+listdaddy[two:]
      newMommy=listmommy[:one]+listdaddy[one:two]+listmommy[two:]
      return newDaddy,newMommy
    return listdaddy,listmommy

  def mutation(self,listdaddy,listmommy):
     rate=1#self.mutationRate
     #assert(len(listdaddy)==len(listmommy)),"Something's messed up"
     if(random.random() < rate):
       #print "MUTATION"
       mutant = listdaddy[:]
       minR,maxR=0,min(len(listdaddy),len(listmommy))
       mutationE = int(minR + (random.random()*(maxR-minR)))
       mutationH = int(minR + (random.random()*(maxR-minR)))
       #print "++ %f %f"%(len(listdaddy),len(listmommy))
       #print ">> %f %f"%(mutationE,mutationH)
       mutant[mutationE]=listmommy[mutationH]
       return mutant
     return listdaddy
  
  #Changes a list of numbers to a stream of numbers
  #eg. [0.234,0.54,0.54325] -> [2345454325]
  def singleStream(self,listpoints):
     singlelist=[]
     for i in listpoints:
       tempstr = str(i)[2:]
       for x in tempstr:
         singlelist.append(x)
     #print singlelist
     return singlelist 

  def generate(self):
    minR = self.model.minR
    maxR = self.model.maxR
    #http://stackoverflow.com/questions/4119070/
    #how-to-divide-a-list-into-n-equal-parts-python
    lol = lambda lst, sz: [lst[i:i+sz] for i in range(0, len(lst), sz)]
    model=self.model
    minR = model.minR
    maxR = model.maxR
    #model.baseline(minR,maxR)
    temps1 = self.Roulette(self.population)
    temps2 = self.Roulette(self.population)
    #workaround: Bug: was getting e in temp2 so,
    #whenever I see anything other than 0-9
    #I replace it
    try:
      import re 
      temps1 = re.sub('[^0-9]', '', temps1)
      temps2 = re.sub('[^0-9]', '', temps2)
    except:
      print temps1
      print temps2
      raise Exception("Ouch!")
    s1 = map(int, temps1)[:self.model.n]
    s2 = map(int, temps2)[:self.model.n]
    #print "S1,S2: %d %d " %(len(s1),len(s2))
    c1,c2=self.crossOver(s1,s2)
    #print "C1,C2: %d %d " %(len(c1),len(c2))
    m1 = self.mutation(c1,c2)
    m2 = self.mutation(c2,c1)
    #print "M1,M2: %d %d " %(len(m1),len(m2))
    #print "self.model.n: %d"%model.n
    normalc1 = [int(''.join(map(str,x)))/10**len(x) for x in lol(m1,1)]
    normalc2 = [int(''.join(map(str,x)))/10**len(x) for x in lol(m2,1)]
    #print "normalc1,normalc2: %d %d"%(len(normalc1),len(normalc2))
    #normalc1 = map(lambda x:minR+x*(maxR-minR),normalc1)    
    #normalc2 = map(lambda x:minR+x*(maxR-minR),normalc2)
    if(len(normalc1)>=self.model.n and len(normalc2)>=self.model.n):
      return normalc1[:self.model.n],normalc2[:self.model.n]  #workaround
    else:
      #print "eeeeeeeeeeee>>>>>>>>>>>>>>>>>>>>>>>>eeeeeeehaha"
      str1 = [random.random() for z in range(0,self.model.n)]
      normalc1,normalc2=[],[]
      for ij in xrange(len(str1)):
        normalc1.append(minR[ij]+str1[ij]*(maxR[ij]-minR[ij]))
      str2 = [random.random() for z in range(0,self.model.n)]  
      for ij in xrange(len(str2)):
        normalc2.append(minR[ij]+str1[ij]*(maxR[ij]-minR[ij]))
      #normalc2 = map(lambda x:minR+x*(maxR-minR),str2)
      return normalc1,normalc2

  #http://stackoverflow.com/questions/10324015
  #/fitness-proportionate-selection-roulette-wheel-selection-in-python
  def Roulette(self,choices):
    maxN = sum(choices.values())
    pick = random.uniform(0, maxN)
    current = 0
    for key, value in choices.items():
        current += abs(value)
        if current > abs(pick):
            return key
    print "Ouch!!"
    print pick,maxN
  
  def keyTransform(self,s):
    minR = self.model.minR
    maxR = self.model.maxR
    strs = self.singleStream(s)
    strs = (''.join(map(str,strs)))
    temp=[]
    for i in xrange(len(s)):
      temp.append(minR[i]+s[i]*(maxR[i]-minR[i]))
    fitness = self.model.evaluate(temp)
    return strs,fitness

  def initialPopulation(self):
    model=self.model
    for i in xrange(50):
      s = [random.random() for z in range(0,model.n)]
      strs,fitness = self.keyTransform(s)
      self.population[strs]=fitness

  def elitism(self):
    rank = self.elitismrank
    #print len(self.population),
    #This controls whether this GA maximizes
    #or minimizes
    l = sorted(self.population.values())
    l = l[rank:]
    # TODO: not at all efficient
    for i in l:
      self.population = {key: value \
      for key, value in self.population.items() \
             if value is not i}
    #print len(self.population)
     

  def evaluate(self):
    #print "evaluate>>>>>>>>>>>>>>>>>>>>>>>>>"
    bestSolution=[]
    bestScore = 1e6
    done=False
    model=self.model
    #print "Model used: %s"%(model.info())
    minR = model.minR
    maxR = model.maxR
    model.baseline(minR,maxR)
    #print "MaxVal: %f MinVal: %f"%(model.maxVal, model.minVal)
    #print "n: %d"%model.n
    self.initialPopulation()
    #print "initial population generated"
    for x in xrange(self.generation):
      #print "Generation: %d"%x
      #print "#",
      for i in xrange(20):
        s1,s2 = self.generate()
        #TODO: dirty
        strs,fitness = self.keyTransform(s1)
        self.population[strs]=fitness
        strs,fitness = self.keyTransform(s2)
        self.display(score=fitness)
        self.population[strs]=fitness
        if(fitness<bestScore):
          bestScore=fitness
          bestSolution=strs
        #print "child born"
      self.model.evalBetter()
      self.elitism()
      #self.display2()
      if(self.model.lives == 0):
        self.display2()
        self.model.emptyWrapper()
        lol = lambda lst, sz: [lst[i:i+sz] \
        for i in range(0, len(lst), sz)]
        tempSolution = [int(''.join(map(str,x)))/10**len(x)\
        for x in lol(bestSolution,int(len(bestSolution)/model.n))]
        solution=[]
        for ij in xrange(len(tempSolution)):
          solution.append(minR[ij]+tempSolution[ij]*(maxR[ij]-minR[ij]))
        #solution= map(lambda x:minR+x*(maxR-minR),tempSolution) 
        return solution,bestScore,self.model

      
    #print sorted(self.population.values())
    self.model.emptyWrapper()
    lol = lambda lst, sz: [lst[i:i+sz] \
    for i in range(0, len(lst), sz)]
    tempSolution = [int(''.join(map(str,x)))/10**len(x)\
     for x in lol(bestSolution,int(len(bestSolution)/model.n))]
    solution= map(lambda x:minR+x*(maxR-minR),tempSolution) 
    return solution,bestScore,self.model

class DE(SearchersBasic):
  def __init__(self,modelName,displayS):
    self.model=modelName
    self.displayStyle=displayS

  def threeOthers(self,frontier,one):
    #print "threeOthers"
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
  
  def trim(self,x,i)  : # trim to legal range
    m=self.model
    return max(m.minR[i], min(x, m.maxR[i]))      

  def extrapolate(self,frontier,one,f,cf):
    #print "Extrapolate"
    two,three,four = self.threeOthers(frontier,one)
    #print two,three,four
    solution=[]
    for d in xrange(self.model.n):
      x,y,z=two[d],three[d],four[d]
      if(random.random() < cf):
        solution.append(self.trim(x + f*(y-z),d))
      else:
        solution.append(one[d]) 
    #print "blah"
    import sys
    sys.stdout.flush()
    return solution

  def update(self,f,cf,frontier,total=0.0,n=0):
    #print "update %d"%len(frontier)
    model=self.model
    newF = []
    total,n=0,0
    for x in frontier:
      #print "update: %d"%n
      s = model.evaluate(x)
      new = self.extrapolate(frontier,x,f,cf)
      #print new
      newe=model.evaluate(new)
      if(newe<s):
        newF.append(new)
      else:
        newF.append(x)
      total+=min(newe,s)
      n+=1
    return total,n,newF
      
  def evaluate(self,repeat=100,np=100,f=0.75,cf=0.3,epsilon=0.01):
    #print "evaluate"
    model=self.model
    minR = model.minR
    maxR = model.maxR
    model.baseline(minR,maxR)
    frontier = [[model.minR[i]+random.random()*(model.maxR[i]-model.minR[i]) for i in xrange(model.n)]
               for _ in xrange(np)]
    #print frontier
    for i in xrange(repeat):
      #print i,
      total,n,frontier = self.update(f,cf,frontier)
      #if(total/n < epsilon):
      #  break;
      self.model.evalBetter()
    minR=9e10
    for x in frontier:
      energy = self.model.evaluate(x)
      if(minR>energy):
        minR = energy
        solution=x  
    return solution,minR,self.model

class PSO(SearchersBasic):
  def __init__(self,modelName,displayS):
    self.v = []
    self.p = []
    self.lb = []
    self.model=modelName
    self.gb = [self.model.minR[i] + random.random()*(self.model.maxR[i]-self.model.minR[i]) \
    for i in xrange(self.model.n)]
    self.eb=self.model.evaluate(self.gb)
    self.displayStyle=displayS 
    self.phi1=myoptions['PSO']['phi1']
    self.phi2=myoptions['PSO']['phi2']
    self.W=myoptions['PSO']['W']
    self.N=myoptions['PSO']['N']
    self.Repeat=myoptions['PSO']['repeat']    
    self.threshold=myoptions['PSO']['threshold'] 
    for x in xrange(self.N):
      self.v.append([0 for _ in xrange(self.model.n)])
      self.p.append([self.model.minR[i] + random.random()*(self.model.maxR[i]-self.model.minR[i])\
      for i in xrange(self.model.n)])
      self.lb.append(self.p[x])
      if(self.model.evaluate(self.p[x])<self.model.evaluate(self.gb)):
        self.gb = self.p[x]
        self.eb = self.model.evaluate(self.gb)
  
  def trim(self,x,i)  : # trim to legal range
    m=self.model
    return max(m.minR[i], min(x, m.maxR[i]))  
  
  def velocity(self,v,p,lb,gb):

    newv = [self.K*(self.W*v[i]+self.phi1*random.random()*(lb[i]-p[i])\
           +self.phi2*random.random()*(gb[i]-p[i])) for i in xrange(self.model.n)]
    #print "blah1"
    return newv

  def displace(self,v,p):
    newp = [v[i]+p[i] for i in xrange(self.model.n)]
    #print "NEWP: ",newp
    return [self.trim(newp[i],i) for i in xrange(len(newp))] 

  def evaluate(self,N=30,phi1=1.3,phi2=2.8,w=1):

    model=self.model 
    v= self.v
    p= self.p 
    lb= self.lb 
    gb= self.gb
    #print "GB: "%gb
    #print "evaluate"
    phi1=self.phi1
    phi2=self.phi2
    N=self.N
    W=self.W
    Repeat=self.Repeat
    threshold=float(self.threshold)
    eb=10**6
    minR = model.minR
    maxR = model.maxR
    phi = phi1+phi2
    self.K = 2/(abs(2 - (phi) -math.sqrt(phi **2) -4*phi))
    model.baseline(minR,maxR)

    for i in xrange(Repeat):
      #if(i%998):print "boom"     
      if(eb<threshold):
        return 0,eb,model
      for n in xrange(N):
        v[n]=self.velocity(v[n],p[n],lb[n],gb)
        p[n]=self.displace(v[n],p[n])
        pener= model.evaluate(p[n])
        lener= model.evaluate(lb[n])        
        if(pener<lener):
          lb[n] = p[n]          
          if(pener < model.evaluate(gb)):
            gb = p[n]
      eb = model.evaluate(gb)
    return 0,eb,model
