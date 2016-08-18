import math
import numpy as np
import matplotlib.pyplot as plt

# 0 == rise, 1 == set
# sunrise and set times in minutes from midnight
late_sun = [7*60 + 35, 21*60 + 7]
early_sun= [5*60 + 41, 16*60 + 46]
sunrise = np.zeros(365)
sunset = np.zeros(365)

# y range for sin approximation
sun_range = [math.floor((late_sun[0] - early_sun[0]) / 2.0), math.floor((late_sun[1] - early_sun[1]) / 2.0)]

# sin constants
A = [sun_range[0], sun_range[1]]    # amplitude
B =  2.0*math.pi / 365.0            # period
#x = 355                             # CURRENT DAY
x = np.linspace(1,365,365)
C = [264, 81]                       # day offset
D = [early_sun[0], early_sun[1]]    # time offset

for ii in range(0,365):
    sunrise[ii] = A[0]*math.sin(B*(x[ii] - C[0]))+D[0]
    sunset[ii] = A[1]*math.sin(B*(x[ii] - C[1]))+D[1]
    #print ii

#print math.floor(sunrise/60)
#print ':'
#print sunrise % 60
#print sunrise

"""
plt.plot(x, sunrise, lw=2, label = 'Sunrise', color = 'green') # lw sets line width
plt.plot(x, sunset, lw=2, label = 'Sunset', color = 'blue')
plt.legend(bbox_to_anchor=(1, 1), loc=1)
plt.xlabel('Day')
plt.ylabel('Time')
plt.show()
"""