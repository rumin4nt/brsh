
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

mpoints_x = points_x[:]
mpoints_y = points_y[:]

import random

mag = 20

for i in xrange(len(mpoints_x)):
	mpoints_x[i] += -.5 + random.random() * pressure[i] * mag
	mpoints_y[i] += -.5 + random.random() * pressure[i] * mag
