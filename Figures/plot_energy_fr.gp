set title "Consommation d'énergie par nœud"
set xlabel "Temps (s)"
set ylabel "Valeur"
set grid
set key top left
set terminal png size 800,600
set output "fig_1_energy.png"
plot "plot_data_energy_fr.txt" using 1:2 title "D-SERAN" with lines lw 2, \
     "plot_data_energy_fr.txt" using 1:3 title "AODV" with lines lw 2, \
     "plot_data_energy_fr.txt" using 1:4 title "DSR" with lines lw 2, \
     "plot_data_energy_fr.txt" using 1:5 title "OLSR" with lines lw 2
