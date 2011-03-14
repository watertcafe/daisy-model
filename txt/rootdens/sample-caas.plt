# set terminal pdf
# set output "caas.pdf"
set yrange [-25:25]
set xrange [-40:40]
set zrange [-100:50]
#caas layout
splot '-' using 2:1:3 with points notitle, 30*cos(x*2*pi/75) with lines notitle
0 0 -7.5
0 0 -20
0 0 -30
0 0 -40
0 0 -50
0 0 -60
0 0 -70

0 18.75 -20
0 18.75 -30
0 18.75 -40
0 18.75 -50
0 18.75 -60
0 18.75 -70

0 37.50 -30
0 37.50 -40
0 37.50 -50
0 37.50 -60
0 37.50 -70

15 0 -7.5
15 0 -20
15 0 -30
15 0 -40
15 0 -50
15 0 -60
15 0 -70

15 18.75 -20
15 18.75 -30
15 18.75 -40
15 18.75 -50
15 18.75 -60
15 18.75 -70

15 37.50 -30
15 37.50 -40
15 37.50 -50
15 37.50 -60
15 37.50 -70
e
#cau layout
-30 -15 0 15 30
-20 -10 0 10 20
-15 -5 5 15 25 35 45
splot '-' using 1:2:3 with points notitle, 30*cos(x*2*pi/80) with lines notitle
-30 0 5
-30 0 15

-15 0 -5
-15 0 5
-15 0 15
-15 0 25
-15 0 35
