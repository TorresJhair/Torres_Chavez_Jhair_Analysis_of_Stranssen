set terminal png size 1000,700 enhanced font "Arial,12"
set output "plot_fase1.png"

set title "Clásico vs Strassen MC-CUTOFF=32"
set xlabel "Tamaño de la Matriz (n)"
set ylabel "Tiempo (microsegundos)"
set key top left
set grid
set logscale y

set style line 1 linecolor rgb "red" linewidth 2 dashtype 3
set arrow from 128,graph 0 to 128,graph 1 nohead ls 1

plot "results2.txt" using 1:2 with linespoints title "Clásico" lw 2 lc rgb "blue" pt 7, \
     "results2.txt" using 1:4 with linespoints title "MC-CUTOFF=32" lw 2 lc rgb "green" pt 7, \
     1/0 ls 1 title "n_0 = 128"