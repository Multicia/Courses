Call Graph Extraction
=====================
Code:
```
def test(): 
  t = scipy.linspace(0,120,4000)
  acc = lambda t: 10*scipy.sin(2*pi*2.0*t) + 5*scipy.sin(2*pi*8.0*t) + 2*scipy.random.random(len(t))
  signal = acc(t)
  FFT = abs(scipy.fft(signal))
  freqs = scipy.fftpack.fftfreq(signal.size, t[1]-t[0])
  pylab.subplot(211)
  pylab.plot(t, signal)
  pylab.subplot(212)
  pylab.plot(freqs,20*scipy.log10(FFT),'x')
  pylab.show()
  r =random.random()
def test2():
  pylab.subplot(211)
  pylab.plot(t, signal)
  pylab.subplot(212)
  test()

if __name__ == '__main__':
  test2()
```

1. Find the call graphs between the user defined function for eg. (borrowed from [link](http://blog.prashanthellina.com/2007/11/14/generating-call-graphs-for-understanding-and-refactoring-python-code/)) 
<pre><code>digraph G {
rankdir=LR
test2 -> test;
}
</code></pre>
2. Go into each function and find all the calls to the library.
<pre><code>
digraph G {
rankdir=LR
.
.
test2->pylab_subplot;
pylab_subplot->pylab_plot;
pylab_plot->pylab_subplot;
pylab_subplot->test;
test->test2
}
</code></pre>
3. Final graph: [link](https://raw.githubusercontent.com/vivekaxl/Courses/master/Misc/LN/callgraphmanip/input1.png)


