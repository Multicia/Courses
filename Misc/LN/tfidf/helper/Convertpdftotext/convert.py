from pdfminer.pdfinterp import PDFResourceManager, PDFPageInterpreter
from pdfminer.converter import TextConverter
from pdfminer.layout import LAParams
from pdfminer.pdfpage import PDFPage
from cStringIO import StringIO

def convert_pdf(path,text_fname):
    """Input: pdf file Output: Output.txt"""
   
    rsrcmgr = PDFResourceManager()
    retstr = StringIO()
    codec = 'utf-8'
    laparams = LAParams()
    device = TextConverter(rsrcmgr, retstr, codec=codec, laparams=laparams)
    fp = file(path, 'rb')
    interpreter = PDFPageInterpreter(rsrcmgr, device)
    password = ""
    maxpages = 0
    caching = True
    pagenos=set()
    for page in PDFPage.get_pages(fp, pagenos, maxpages=maxpages, password=password,caching=caching, check_extractable=True):
        interpreter.process_page(page)
    fp.close()
    device.close()
    string = retstr.getvalue()
    retstr.close()
    text_file = open(text_fname, "w")
    text_file.write(string)
    text_file.close()
    return text_fname

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
  text_file = convert_pdf("user_guide-0.11.pdf","user_guide-0.11.txt")
  convert(text_file)
