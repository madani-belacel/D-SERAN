set title "Nombre moyen de sauts"
set xlabel "Temps (s)"
set ylabel "Valeur"
set grid
set key top left
set terminal png size 800,600
set output "fig_6_hop.png"
plot "plot_data_hop_fr.txt" using 1:2 title "D-SERAN" with lines lw 2, \
     "plot_data_hop_fr.txt" using 1:3 title "AODV" with lines lw 2, \
     "plot_data_hop_fr.txt" using 1:4 title "DSR" with lines lw 2, \
     "plot_data_hop_fr.txt" using 1:5 title "OLSR" with lines lw 2
