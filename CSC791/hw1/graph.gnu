set terminal png size 1124,768 enhanced font "Times-Roman,15"
set output 'Cycles.png
set xlabel 'x'
set ylabel 'y'
plot 'data.log' using 1:2 with linespoints
