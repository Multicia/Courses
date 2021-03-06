from sklearn.cluster import KMeans;

#From GM
def readFile(fileName):
  f = open(fileName)
  fileLine = f.readline()
  X = []
  while fileLine:
    X.append([float(n) for n in fileLine.split(",")])
    fileLine = f.readline()
  f.close()
  return X

def explore(data):
  errList=[];
  for i in range(2,int(len(data)**0.5)):
    km = KMeans(n_clusters=i)
    km.fit(data)
    err = abs(km.score(data))
    errList.append(err)
    #print i, err
#with brestcancer data I get the elbow at 7

def decisionTree(data,labels,depth=10):
  clf = tree.DecisionTreeClassifier(max_depth = depth)
  clf = clf.fit(data,labels)
  return clf

if __name__ == "__main__":
  data = readFile("breast-cancer-wisconsin.data")
  #explore(data)
  def kmeans(data,i):
    km = KMeans(n_clusters=i)
    print km
    km.fit(data)
    return km
  km=kmeans(data,7)
  labels = km.predict(data)
  from sklearn import tree
  model=decisionTree(data,labels)
  from StringIO import StringIO
  out = StringIO()
  tree.export_graphviz(model, out_file=out)
  from pydot import graph_from_dot_data
  graph = graph_from_dot_data(out.getvalue())
  graph.write_pdf("somefile.pdf")
