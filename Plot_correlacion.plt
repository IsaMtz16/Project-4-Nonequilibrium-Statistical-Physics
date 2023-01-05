set title "Correlation for tau=1"
set xrange [:4]
set size square
set xlabel "Time"
set ylabel "Correlation"
D=4
t=1
f(x)=(D*exp(-x/t))/t
plot "Correlacion_tau1.txt" u 1:2 w l lc rgb "web-blue" t "tau=1"
replot f(x) lc rgb "navy" lw 2 dt 2 t "theoretical"
#plot "Correlacion_tau10.txt" u 1:2 t "tau=10"
#replot "Correlacion_tau100.txt" u 1:2 t "tau=100"