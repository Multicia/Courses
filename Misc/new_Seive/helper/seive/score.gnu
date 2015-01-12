set terminal png
set key horizontal
set xlabel "Models"
set ylabel "Score"
set title "Scores"
set output "score.png"
set boxwidth 0.25
set xrange [-0.5:7]
set yrange [-6:2]
plot 'score.dat' using ($0-.05):4:5:xtic(1) with boxerrorbars title col, \
'' using ($0+0.25):2:3 with boxerrorbars title col
