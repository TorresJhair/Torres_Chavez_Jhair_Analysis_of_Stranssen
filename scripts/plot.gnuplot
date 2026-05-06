set terminal png size 1000,700 enhanced font "Arial,12"
set output "exp_plot.png"

set title "Classic vs Strassen vs Strassen_Strided Matrix Multiplication"
set xlabel "Matrix size (n)"
set ylabel "Time (microseconds)"
set key top left
set grid
set logscale y

plot "results.txt" using 1:2 with linespoints title "mult_classic"      lw 2 lc rgb "blue"  pt 7, \
     "results.txt" using 1:3 with linespoints title "strassen_strided"  lw 2 lc rgb "green" pt 7, \
     "results.txt" using 1:4 with linespoints title "strassen_cutoff"  lw 2 lc rgb "red" pt 7
