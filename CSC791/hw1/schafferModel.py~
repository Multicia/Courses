import os
file = open("./data.log","w")
x=-10000
while(x!=10000):
  y=(x**2)+(x-2)**2
  file.write("%f %f\n"%(x,y))
  x+=1


os.system('gnuplot graph.gnu')
