Report 2014-11-19
==========

- The tfidf list doesn't seem to be working. 
TODO:
1. write a regular expression which would just choose the functions rather than the words (Most of the words in the list doesn't seem to be right)


- Removed stemming and remove ". " rather than "."

- Graphs are being generated along with the weights.

Documents: http://www.math.unipd.it/~aiolli/corsi/1213/aa/user_guide-0.12-git.pdf
Examples: http://scikit-learn.org/stable/auto_examples/

Report 2014-11-26
============

- I extract only the functions in the scikit learn document. [file](https://github.com/vivekaxl/Courses/blob/master/Misc/LN/findingfuncs/library_tfidf.txt)
- I used the first 300 functions based on the tfidf scores.
- Graphs are generated along with the weights [graph](https://github.com/vivekaxl/Courses/blob/master/Misc/LN/findingfuncs/new_output.png)

Documents: http://www.math.unipd.it/~aiolli/corsi/1213/aa/user_guide-0.12-git.pdf
Examples: http://scikit-learn.org/stable/auto_examples/

TODO:
- Need to make smarter function matching.
- Make a graph for each class in the library such as Classification, regression etc. This would make the graphs manageable and easy to read. 


