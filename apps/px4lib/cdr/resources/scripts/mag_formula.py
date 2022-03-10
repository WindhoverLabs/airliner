from math import atan2

X = pvs[0].getValue().getValue()
Y = pvs[1].getValue().getValue()
M_PI = 3.14

heading = 180 * atan2(Y, X)/M_PI

if heading < 0:
    heading += 360

widget.setPropertyValue('text', heading)
# widget.setPropertyValue('pv_value', heading)


