
#	simple as can be.
#	the wash host application provides arrays of fixed lengths:
#	points_x
#	points_y
#	pressure
#	rotation
#	tilt_x
#	tilt_y

#	and some others TBD
#	probably will provide seeded randoms as well and maybe noise
#

#	probably will add some better type checking and safety later too, idk!

import random

mag = 20

for i in xrange(len(xdata)):
	xdata[i] += -.5 + random.random() * mag
	ydata[i] += -.5 + random.random() * mag
