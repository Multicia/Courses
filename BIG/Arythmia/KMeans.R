mydata <- read.csv("/home/vivek/DataSet/Wisonsin/breast-cancer-wisconsin.data", header=TRUE)
dim(mydata)
#To print the dimensions of the table
str(mydata)
summary(mydata)
library("party")
iris_ctree <- ctree(Class ~ ClumpThickness+UniformityofCellSize+UniformityofCellShape, data=mydata)
print(iris_ctree)
plot(iris_ctree)

#Density plot of the first line
#set.seed(1)
#d<-data.frame(a=rnorm(100))
#library(lattice)
#densityplot(~a,data=d)

#Kmean clustering
#data preps
#mydata <- scale(mydata) # standardize variables
# Determine number of clusters
#wss <- (nrow(mydata)-1)*sum(apply(mydata,2,var))
#for (i in 2:15) wss[i] <- sum(kmeans(mydata, 
#                                     centers=i)$withinss)
#plot(1:15, wss, type="b", xlab="Number of Clusters",
#     ylab="Within groups sum of squares")