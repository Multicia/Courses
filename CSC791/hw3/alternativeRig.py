from __future__ import division
import sys
import random
import math
import numpy as np
from models import *
from searchers import *
from options import *
from utilities import *
sys.dont_write_bytecode = True
#Dr.M
rand=  random.random # generate nums 0..1
any=   random.choice # pull any from list
sqrt= math.sqrt  #square root function


def step2():
   random.seed(14)
   test = MaxWalkSat("ZDT1")
   solution,score = test.evaluate()
   print "Solution: ",
   print solution
   print "Score: ",
   print score

   print myoptions

def display(modelName,searcher,runTimes,scores):
  assert(len(runTimes) == len(scores)),'Ouch! it hurts'
  print "==============================================================="
  print "Model Name: %s"%modelName
  print "Searcher Name: %s"%searcher.__name__,
  print "Options Used: ",
  print myoptions[searcher.__name__]
  import time
  print ("Data: %s"%time.strftime("%d/%m/%Y"))
  for i in range(0,len(runTimes)):
    print "RunNo: %s RunTime: %s Score: %s"%(i+1,runTimes[i],scores[i])
  print scores
  print xtile(scores,width=25,show=" %1.6f")
  print "==============================================================="

  

def multipleRun():
 r = 20
 for klass in [Schaffer, Fonseca, Kursawe, ZDT1]:
   #print "Model Name: %s"%klass.__name__
   for searcher in [ MaxWalkSat]:
     n = 0.0
     listTimeTaken = []
     listScores = []
     random.seed(1)
     for _ in range(r):
       test = searcher(klass())
       import time
       t1 = time.time()
       solution,score = test.evaluate()
       timeTaken = (time.time() - t1) * 1000
       listTimeTaken.append(timeTaken)
       listScores.append(score)
     display(klass.__name__,searcher,listTimeTaken,listScores)
     
     

if __name__ == '__main__': 
 # random.seed(1)
 # nums = [random.random()**2 for _ in range(100)]
 # print xtile(nums,lo=0,hi=1.0,width=25,show=" %3.2f")
 # model = ZDT1()
 # model.testgx()
 # for klass in [ZDT1]:
 #   print klass.__name__
 multipleRun()
 
"""
===============================================================
Model Name: Schaffer
Searcher Name: SA Options Used:  {'emax': '0', 'kmax': '1000'}
Data: 16/09/2014
RunNo: 1 RunTime: 1376.94907188 Score: 1.90308005981e-09
RunNo: 2 RunTime: 1372.86686897 Score: 1.71671530289e-06
RunNo: 3 RunTime: 1385.39409637 Score: 9.77940617671e-06
RunNo: 4 RunTime: 1363.25502396 Score: 1.98583740967e-09
RunNo: 5 RunTime: 1351.65500641 Score: 3.15536015768e-07
RunNo: 6 RunTime: 1387.30287552 Score: 3.73828565035e-07
RunNo: 7 RunTime: 1341.34411812 Score: 2.98130703422e-07
RunNo: 8 RunTime: 1358.45994949 Score: 1.36120317132e-08
RunNo: 9 RunTime: 1345.45111656 Score: 4.81338255948e-07
RunNo: 10 RunTime: 1359.86590385 Score: 5.52972977663e-07
RunNo: 11 RunTime: 1373.17013741 Score: 2.39362028364e-06
RunNo: 12 RunTime: 1365.17000198 Score: 2.77656185581e-08
RunNo: 13 RunTime: 1376.04689598 Score: 1.21159512388e-08
RunNo: 14 RunTime: 1376.1048317 Score: 5.37424147692e-08
RunNo: 15 RunTime: 1419.28482056 Score: 3.73851103763e-06
RunNo: 16 RunTime: 1357.62906075 Score: 1.51385072162e-06
RunNo: 17 RunTime: 1407.82499313 Score: 5.35941675677e-06
RunNo: 18 RunTime: 1371.33002281 Score: 2.25474698296e-07
RunNo: 19 RunTime: 1367.82598495 Score: 2.6306905176e-06
RunNo: 20 RunTime: 1388.06605339 Score: 2.62047658859e-08
*           |            , 0.000000,  0.000000,  0.000000,  0.000002,  0.000005
===============================================================
===============================================================
Model Name: Schaffer
Searcher Name: MaxWalkSat Options Used:  {'threshold': '0.05', 'probLocalSearch': '0.25', 'maxChanges': '2000', 'maxTries': '50'}
Data: 16/09/2014
RunNo: 1 RunTime: 1320.88279724 Score: 0.00586245782918
RunNo: 2 RunTime: 1365.64302444 Score: 0.000670472037957
RunNo: 3 RunTime: 1324.83005524 Score: 0.00699513792987
RunNo: 4 RunTime: 1325.21891594 Score: 0.00433001814486
RunNo: 5 RunTime: 1328.1121254 Score: 9.11638421369e-05
RunNo: 6 RunTime: 1330.13510704 Score: 0.0328778610325
RunNo: 7 RunTime: 1396.52490616 Score: 0.00121409938656
RunNo: 8 RunTime: 1366.84989929 Score: 0.049310574757
RunNo: 9 RunTime: 1388.12398911 Score: 0.0308837400977
RunNo: 10 RunTime: 1348.51789474 Score: 0.00283894362161
RunNo: 11 RunTime: 1363.13605309 Score: 0.0304667047395
RunNo: 12 RunTime: 1368.06297302 Score: 0.0170385909002
RunNo: 13 RunTime: 1358.84094238 Score: 0.0415911675312
RunNo: 14 RunTime: 1369.6398735 Score: 0.00103188782028
RunNo: 15 RunTime: 1390.76209068 Score: 0.00245225736494
RunNo: 16 RunTime: 1358.91294479 Score: 0.0156310030173
RunNo: 17 RunTime: 1366.45102501 Score: 0.040054839822
RunNo: 18 RunTime: 1361.87696457 Score: 0.0026196832493
RunNo: 19 RunTime: 1340.52801132 Score: 0.0179183554043
RunNo: 20 RunTime: 1340.64292908 Score: 0.00815060229066
-   *       |  ------    , 0.001032,  0.002839,  0.008151,  0.030467,  0.041591
===============================================================
===============================================================
Model Name: Fonseca
Searcher Name: SA Options Used:  {'emax': '0', 'kmax': '1000'}
Data: 16/09/2014
RunNo: 1 RunTime: 651.077985764 Score: 0.0419534888819
RunNo: 2 RunTime: 648.839950562 Score: 0.0651074750783
RunNo: 3 RunTime: 647.619009018 Score: 0.163799157361
RunNo: 4 RunTime: 649.878978729 Score: 0.114464379835
RunNo: 5 RunTime: 645.64204216 Score: 0.204266163912
RunNo: 6 RunTime: 639.408826828 Score: -0.0140174822582
RunNo: 7 RunTime: 648.295164108 Score: 0.013417261435
RunNo: 8 RunTime: 648.04983139 Score: 0.127039295842
RunNo: 9 RunTime: 656.972885132 Score: 0.0525325242795
RunNo: 10 RunTime: 647.458076477 Score: 0.0566438879141
RunNo: 11 RunTime: 657.103061676 Score: 0.0402667573575
RunNo: 12 RunTime: 647.341012955 Score: 0.188910926348
RunNo: 13 RunTime: 644.497156143 Score: 0.151783773725
RunNo: 14 RunTime: 644.43898201 Score: 0.122726125002
RunNo: 15 RunTime: 654.121160507 Score: 0.0379367862929
RunNo: 16 RunTime: 665.339946747 Score: 0.124932732336
RunNo: 17 RunTime: 654.94799614 Score: 0.103151965521
RunNo: 18 RunTime: 695.8091259 Score: 0.0310111795994
RunNo: 19 RunTime: 657.23991394 Score: 0.158042979697
RunNo: 20 RunTime: 652.688026428 Score: 0.132403100141
     --     | * -------  , 0.031011,  0.052533,  0.114464,  0.132403,  0.188911
===============================================================
===============================================================
Model Name: Fonseca
Searcher Name: MaxWalkSat Options Used:  {'threshold': '0.05', 'probLocalSearch': '0.25', 'maxChanges': '2000', 'maxTries': '50'}
Data: 16/09/2014
RunNo: 1 RunTime: 1082.47494698 Score: 0.0373453526213
RunNo: 2 RunTime: 922.782182693 Score: -0.0125309782223
RunNo: 3 RunTime: 659.284114838 Score: -0.00856406042771
RunNo: 4 RunTime: 814.288854599 Score: -0.00394921042086
RunNo: 5 RunTime: 679.716110229 Score: 0.0230594227192
RunNo: 6 RunTime: 662.416934967 Score: 0.0214269550768
RunNo: 7 RunTime: 686.342000961 Score: 0.0382242002491
RunNo: 8 RunTime: 698.376893997 Score: 0.00543440182751
RunNo: 9 RunTime: 672.694921494 Score: 0.0463375351383
RunNo: 10 RunTime: 719.115018845 Score: 0.0269364756895
RunNo: 11 RunTime: 853.468894958 Score: 0.0485447124469
RunNo: 12 RunTime: 686.365127563 Score: 0.0402351677697
RunNo: 13 RunTime: 629.322052002 Score: 0.0401438719005
RunNo: 14 RunTime: 701.623916626 Score: 0.0222890938211
RunNo: 15 RunTime: 680.032014847 Score: -0.00519934069733
RunNo: 16 RunTime: 731.384992599 Score: 0.0387283484501
RunNo: 17 RunTime: 661.950111389 Score: 0.0471690998975
RunNo: 18 RunTime: 752.955913544 Score: 0.0301684697522
RunNo: 19 RunTime: 778.366804123 Score: 0.0402660695382
RunNo: 20 RunTime: 899.95098114 Score: -0.00476181652541
   ---------|    *   --- , -0.005199,  0.021427,  0.030168,  0.040144,  0.047169
===============================================================
===============================================================
Model Name: Kursawe
Searcher Name: SA Options Used:  {'emax': '0', 'kmax': '1000'}
Data: 16/09/2014
RunNo: 1 RunTime: 16206.6791058 Score: -0.0400351857037
RunNo: 2 RunTime: 16817.8071976 Score: 0.00470390137354
RunNo: 3 RunTime: 15851.7158031 Score: -0.0347784663656
RunNo: 4 RunTime: 15429.022789 Score: 0.00852886995295
RunNo: 5 RunTime: 15877.3710728 Score: -0.00466871597172
RunNo: 6 RunTime: 15409.1801643 Score: -0.0339601519916
RunNo: 7 RunTime: 15372.5910187 Score: -0.0313775327841
RunNo: 8 RunTime: 15351.2551785 Score: -0.0115262878815
RunNo: 9 RunTime: 15510.4031563 Score: -0.013970910364
RunNo: 10 RunTime: 15346.2069035 Score: -0.0405656078434
RunNo: 11 RunTime: 15480.5459976 Score: -0.00376235374809
RunNo: 12 RunTime: 15397.0649242 Score: -0.00703036121401
RunNo: 13 RunTime: 15179.8739433 Score: -0.00137694842792
RunNo: 14 RunTime: 15328.8249969 Score: -0.0238634589188
RunNo: 15 RunTime: 15314.540863 Score: -0.0380028154922
RunNo: 16 RunTime: 15095.5688953 Score: -0.00436220530017
RunNo: 17 RunTime: 15203.6659718 Score: -0.00560074225093
RunNo: 18 RunTime: 15747.0631599 Score: -0.0412257528418
RunNo: 19 RunTime: 16611.0260487 Score: -0.0387627647954
RunNo: 20 RunTime: 15525.2649784 Score: -0.00260432940614
---         | *   -----  , -0.040035,  -0.033960,  -0.011526,  -0.004362,  0.004704
===============================================================
===============================================================
Model Name: Kursawe
Searcher Name: MaxWalkSat Options Used:  {'threshold': '0.05', 'probLocalSearch': '0.25', 'maxChanges': '2000', 'maxTries': '50'}
Data: 16/09/2014
RunNo: 1 RunTime: 15576.8110752 Score: 0.0353789937321
RunNo: 2 RunTime: 15621.0858822 Score: -0.0464603511621
RunNo: 3 RunTime: 15269.6568966 Score: 0.00384087051454
RunNo: 4 RunTime: 15233.4308624 Score: 0.0491661010567
RunNo: 5 RunTime: 15210.4051113 Score: 0.0417397961658
RunNo: 6 RunTime: 15552.5839329 Score: 0.0419413492599
RunNo: 7 RunTime: 15703.4869194 Score: 0.02211483713
RunNo: 8 RunTime: 15679.4269085 Score: -0.00314869827434
RunNo: 9 RunTime: 15159.9271297 Score: 0.0283815642467
RunNo: 10 RunTime: 15565.048933 Score: 0.0105129956968
RunNo: 11 RunTime: 15542.206049 Score: 0.0148351755961
RunNo: 12 RunTime: 15552.5591373 Score: 0.0416362045322
RunNo: 13 RunTime: 15602.7190685 Score: 0.0447526875326
RunNo: 14 RunTime: 15605.2191257 Score: 0.0438802681277
RunNo: 15 RunTime: 15728.9819717 Score: 0.0490883133213
RunNo: 16 RunTime: 15755.3408146 Score: 0.0497445416382
RunNo: 17 RunTime: 15766.1249638 Score: 0.0316673605511
RunNo: 18 RunTime: 15843.1708813 Score: -0.0123047265239
RunNo: 19 RunTime: 15767.8160667 Score: 0.0378896698599
RunNo: 20 RunTime: 15697.2138882 Score: 0.0262392433297
           -|----    *-- , -0.003149,  0.022115,  0.035379,  0.041941,  0.049166
===============================================================
===============================================================
Model Name: ZDT1
Searcher Name: SA Options Used:  {'emax': '0', 'kmax': '1000'}
Data: 16/09/2014
RunNo: 1 RunTime: 9966.63689613 Score: 0.607349263331
RunNo: 2 RunTime: 9933.59899521 Score: 0.690283169167
RunNo: 3 RunTime: 9942.14200974 Score: 0.641601953363
RunNo: 4 RunTime: 9981.29606247 Score: 0.922825805241
RunNo: 5 RunTime: 9957.89313316 Score: 0.750096244953
RunNo: 6 RunTime: 9963.04512024 Score: 0.789730458693
RunNo: 7 RunTime: 9958.5340023 Score: 0.784473698789
RunNo: 8 RunTime: 9978.54208946 Score: 0.6304449494
RunNo: 9 RunTime: 9940.41800499 Score: 0.716262424352
RunNo: 10 RunTime: 9963.46998215 Score: 0.601679450232
RunNo: 11 RunTime: 10086.6520405 Score: 0.870368925988
RunNo: 12 RunTime: 9970.98112106 Score: 0.725674756766
RunNo: 13 RunTime: 9943.09306145 Score: 0.613517831442
RunNo: 14 RunTime: 10149.8432159 Score: 0.741182835881
RunNo: 15 RunTime: 10081.1738968 Score: 0.832217709177
RunNo: 16 RunTime: 10054.1470051 Score: 0.828557912536
RunNo: 17 RunTime: 10131.9639683 Score: 0.774364831697
RunNo: 18 RunTime: 12141.2899494 Score: 0.648476348908
RunNo: 19 RunTime: 11066.9779778 Score: 0.750806113764
RunNo: 20 RunTime: 10097.2750187 Score: 0.812234101483
            |   --  *--  , 0.613518,  0.690283,  0.750096,  0.789730,  0.870369
===============================================================
===============================================================
Model Name: ZDT1
Searcher Name: MaxWalkSat Options Used:  {'threshold': '0.05', 'probLocalSearch': '0.25', 'maxChanges': '2000', 'maxTries': '50'}
Data: 16/09/2014
RunNo: 1 RunTime: 22921.0500717 Score: 0.930585616737
RunNo: 2 RunTime: 23222.4369049 Score: 0.964847940098
RunNo: 3 RunTime: 23205.575943 Score: 0.937002356648
RunNo: 4 RunTime: 23109.3831062 Score: 0.928550078793
RunNo: 5 RunTime: 22652.739048 Score: 0.849983567477
RunNo: 6 RunTime: 23080.9340477 Score: 1.10044916546
RunNo: 7 RunTime: 23071.1991787 Score: 0.962791557655
RunNo: 8 RunTime: 23113.5718822 Score: 0.935928287197
RunNo: 9 RunTime: 23001.1708736 Score: 1.01864395945
RunNo: 10 RunTime: 22865.9648895 Score: 0.992306513527
RunNo: 11 RunTime: 22906.1307907 Score: 0.92393461202
RunNo: 12 RunTime: 22826.3959885 Score: 0.96421720291
RunNo: 13 RunTime: 22823.127985 Score: 0.936388553651
RunNo: 14 RunTime: 22804.6159744 Score: 1.00471185381
RunNo: 15 RunTime: 22861.2191677 Score: 0.997554176763
RunNo: 16 RunTime: 22875.3318787 Score: 1.03570759838
RunNo: 17 RunTime: 22950.1819611 Score: 0.902242342999
RunNo: 18 RunTime: 23528.9850235 Score: 1.12452522357
RunNo: 19 RunTime: 22931.5419197 Score: 1.01377412646
RunNo: 20 RunTime: 23385.7808113 Score: 0.929131305772
            |        *-- , 0.923935,  0.935928,  0.964217,  1.004712,  1.100449
===============================================================


"""

