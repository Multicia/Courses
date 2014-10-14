from __future__ import division 
import sys
sys.dont_write_bytecode = True

myoptions = {'MaxWalkSat':{'maxTries':'50','maxChanges':'2000','threshold':'0.001','probLocalSearch':'0.25'},'SA':{'kmax':'1000','emax':'0'},'GA':{'crossOverRate':'0.6','mutationRate':'0.1','elitism':'50','generation':'20'}}

myModeloptions = {'Lives': 4,'a12':0.56}
myModelobjf = {'Viennet':3,'Schaffer':2, 'Fonseca':2, 'Kursawe':2, 'ZDT1':2,'ZDT3':2,'DTLZ7':20}
