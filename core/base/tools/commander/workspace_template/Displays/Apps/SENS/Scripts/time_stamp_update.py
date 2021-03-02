from org.csstudio.opibuilder.scriptUtil import PVUtil

time = PVUtil.getTimeString(display.getWidget("Gyro_X").getPV(), "yyyy-MM-dd HH:mm:ss")

widget.setPropertyValue("pv_value", time)

