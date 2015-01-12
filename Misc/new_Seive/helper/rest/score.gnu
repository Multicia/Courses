set terminal png
set key horizontal
set xlabel "Models"
set ylabel "Score"
set title "Scores"
set output "score.png"
set boxwidth 0.15
set xrange [-0.75:7]
set yrange [-2:2]
plot 'score.dat' using ($0-.45):4:5:xtic(1) with boxerrorbars title col, \
'' using ($0-0.30):2:3 with boxerrorbars title col,\
'' using ($0-0.15):6:7 with boxerrorbars title col,\
'' using ($0):8:9 with boxerrorbars title col,\
'' using ($0+0.15):10:11 with boxerrorbars title col,\
'' using ($0+0.30):12:13 with boxerrorbars title col
