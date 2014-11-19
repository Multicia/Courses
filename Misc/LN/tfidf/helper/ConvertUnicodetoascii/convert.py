def convert(f_name):
  import unicodedata
  o_file = f_name.split(".")[0] + "_conv." + f_name.split(".")[1]
  output = open(o_file,"w")
  for line in open(f_name,"r"):
    udata=line.decode("utf-8")
    asciidata=udata.encode("ascii","ignore")
    output.write(asciidata)

  output.close()
  

if __name__ == '__main__':
  convert("library.txt")

