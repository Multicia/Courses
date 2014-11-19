

def merge(dir_name,result):
  """"""
  import glob
  path = dir_name + "*.txt"
  read_files = glob.glob(path)
  with open(result, "wb") as outfile:
    for f in read_files:
      print f
      with open(f, "rb") as infile:
        outfile.write(infile.read())

if __name__ == '__main__':
  merge("data/library/","library.txt")

