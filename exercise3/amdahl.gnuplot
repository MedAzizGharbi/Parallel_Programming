set terminal pngcairo size 1200,800
set output 'amdahl_speedup.png'

set title "Amdahl's Law Speedup Curves"
set xlabel "Number of Cores"
set ylabel "Speedup"

set xrange[1:128]

#Logarithmic y-axis
set logscale y 10
set logscale x 2

set grid
set key left top

S(n,f) = 1.0 / (f + (1.0-f)/n)

plot \
		x title "Ideal" lw 2, \
		S(x, 0.2) title "f = 0.2" lw 2, \
		S(x, 0.1) title "f = 0.1" lw 2, \
		S(x, 0.05) title "f = 0.05" lw 2, \
		S(x, 0.01) title "f = 0.01" lw 2, \
		S(x, 0.001) title "f = 0.001" lw 2
