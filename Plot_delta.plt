set title "Displacement as a function of time for different forces"
set xlabel "Time (log) "
set ylabel "Displacement (log) "
set key top left
set logscale xy
plot "MSD_delta2_d_f0.000.txt" u 1:3 w l lw 1.5 t "f=0"
replot "MSD_delta2_d_f0.001.txt" u 1:3 w l lw 1.5 t "f=0.001"
replot "MSD_delta2_d_f0.010.txt" u 1:3 w l lw 1.5 t "f=0.01"
replot "MSD_delta2_d_f0.100.txt" u 1:3 w l lw 1.5 t "f=0.1"
replot "MSD_delta2_d_f1.000.txt" u 1:3 w l lw 1.5 t "f=1"

