import os
file = open("./data.log","w")
x=-10
while(x!=10):
  y=(x**2)+(x-2)**2
  file.write("%f %f\n"%(x,y))
  x+=1


os.system('gnuplot graph.gnu')
