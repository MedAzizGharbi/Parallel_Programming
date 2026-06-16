set terminal pngcairo size 1200,800 enhanced font "Arial,12"
set output "speedup.png"

set datafile separator ","
set title "Speedup of π Variants"
set xlabel "Threads"
set ylabel "Speedup"
set grid
set key left top

set logscale x 2   # log base 2 for threads (VERY useful)
set format x "2^{%L}"

# reference single-thread times (update if needed)
T1_A = 0.613210
T1_B = 5.595677
T1_C = 5.624380

plot \
"A_avg.csv" using 2:(T1_A/$3) with linespoints lw 2 title "Variant A", \
"B_avg.csv" using 2:(T1_B/$3) with linespoints lw 2 title "Variant B", \
"C_avg.csv" using 2:(T1_C/$3) with linespoints lw 2 title "omp for"
