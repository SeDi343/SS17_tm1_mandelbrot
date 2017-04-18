# tm1_mandelbrot
MANDELBROT GENERATOR @ v1.0  
Created by Sebastian Dichler, 2017  
  
The pixelgen program is generating a mandelbrot set and writes them into a P3 PPM picture.  
  
The whole process is running in a while loop and zooms out of the standard zoom.  
Debug messages appear read and time calculation messages appear in yellow background  
Format input:  
<pre>
./pixelgen -w 3840 -h 2160 -i 30000 -c 1  
</pre>
  
Useable parameters:  
<pre>
-w    to change Image width  
-h    to change Image height  
-i    to change iterations  
-t    to change zoom  
  0   the default template  
  1   for 1st template  
  2   for 2nd template  
  3   for 3rd template  
  4   for 4th template  
  5   for 5th template  
  6   for 6th template  
  7   for 7th template  
  8   for 8th template  
  9   for 9th template  
  10  for 10th template  
-c    change color type  
  1   for sinus cyan  
  2   for sinus red  
  3   for sinus green  
  4   for sinus blue  
  5   for PT1 cyan  
  6   for PT1 red  
  7   for PT1 green  
  8   for PT1 blue  
  0   for log log all channels  
-?    to print help message  
</pre>
