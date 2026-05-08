set terminal png size 1000,700 enhanced font "Arial,12"
set output "plot_fase2.png"

set title "Multiplicación de Matrices: Clásico vs Strassen"
set xlabel "Tamaño de la Matriz (n)"
set ylabel "Tiempo (microsegundos)"
set key top left
set grid
set logscale y

plot "results.txt" using 1:2 with linespoints title "Clásico" lw 2 lc rgb "blue" pt 7, \
     "results.txt" using 1:4 with linespoints title "MC-CUTOFF" lw 2 lc rgb "green" pt 7, \
     "results.txt" using 1:6 with linespoints title "Power of 2" lw 2 lc rgb "red" pt 7