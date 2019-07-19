# Candy-Claw-Bot
CandyClawProgram.ino: 
The arduino source code for the Candy Claw Robot, a collaboration between my dad, my brother, and I. The arduino drives the motors, runs the countdown timer and end sequence, respects the limit switches, and opens and closes, and raises and lowers the claw. 

CandyClawConvert.py:
Code to display the volts and amps used by the Candy Claw Bot over time. This was a preliminary test for our data logger, which would be used on the Legoheads competition robot. 

packRect.py:
The packer that we used: https://github.com/secnot/rectpack/tree/master/rectpack
To build the Candy Claw Bot, we needed to buy large amounts of acrylic plexiglass, which came in large sheets. This program uses the packer from rectpack to output html to tell the cutting company how to cut the three 30 x 32 and four 24 x 30 panes that we purchased. 
