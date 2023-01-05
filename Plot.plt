set title "Diffusion constant and mobility as functions of time (f=1)"
set xlabel "Tau"
plot "mu_D_f=10.txt" u 1:2 w l lc rgb "#3161E7" lw 2 t "D"
replot "mu_D_f=10.txt" u 1:3 w l lc rgb "#E92F60" lw 2 t "\mu"