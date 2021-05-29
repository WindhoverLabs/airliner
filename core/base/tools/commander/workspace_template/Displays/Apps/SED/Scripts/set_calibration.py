"""
Script to send SetCalibration command to SED.
"""

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

# Accel
acc_x_scale = display.getWidget('AccXScale_arg').getPropertyValue('pv_value').getValue()
acc_y_scale = display.getWidget('AccYScale_arg').getPropertyValue('pv_value').getValue()
acc_z_scale = display.getWidget('AccZScale_arg').getPropertyValue('pv_value').getValue()
acc_x_offset = display.getWidget('AccXOffset_arg').getPropertyValue('pv_value').getValue()
acc_y_offset = display.getWidget('AccYOffset_arg').getPropertyValue('pv_value').getValue()
acc_z_offset = display.getWidget('AccZOffset_arg').getPropertyValue('pv_value').getValue()

# Gyro
gyro_x_scale = display.getWidget('GyroXScale_arg').getPropertyValue('pv_value').getValue()
gyro_y_scale = display.getWidget('GyroYScale_arg').getPropertyValue('pv_value').getValue()
gyro_z_scale = display.getWidget('GyroZScale_arg').getPropertyValue('pv_value').getValue()
gyro_x_offset = display.getWidget('GyroXOffset_arg').getPropertyValue('pv_value').getValue()
gyro_y_offset = display.getWidget('GyroYOffset_arg').getPropertyValue('pv_value').getValue()
gyro_z_offset = display.getWidget('GyroZOffset_arg').getPropertyValue('pv_value').getValue()

# Mag
mag_x_scale = display.getWidget('MagXScale_arg').getPropertyValue('pv_value').getValue()
mag_y_scale = display.getWidget('MagYScale_arg').getPropertyValue('pv_value').getValue()
mag_z_scale = display.getWidget('MagZScale_arg').getPropertyValue('pv_value').getValue()
mag_x_offset = display.getWidget('MagXOffset_arg').getPropertyValue('pv_value').getValue()
mag_y_offset = display.getWidget('MagYOffset_arg').getPropertyValue('pv_value').getValue()
mag_z_offset = display.getWidget('MagZOffset_arg').getPropertyValue('pv_value').getValue()

Yamcs.issueCommand('/cfs/'+display.getMacroValue('CPUID')+'/sed/SetCalibration',
				   {'Calibration.AccXScale': acc_x_scale,
					'Calibration.AccYScale': acc_y_scale,
					'Calibration.AccZScale': acc_z_scale,
					'Calibration.AccXOffset': acc_x_offset,
					'Calibration.AccYOffset': acc_y_offset,
					'Calibration.AccZOffset': acc_z_offset,
					'Calibration.GyroXScale': gyro_x_scale,
					'Calibration.GyroYScale': gyro_y_scale,
					'Calibration.GyroZScale': gyro_z_scale,
					'Calibration.GyroXOffset': gyro_x_offset,
					'Calibration.GyroYOffset': gyro_y_offset,
					'Calibration.GyroZOffset': gyro_z_offset,
					'Calibration.MagXScale': mag_x_scale,
					'Calibration.MagYScale': mag_y_scale,
					'Calibration.MagZScale': mag_z_scale,
					'Calibration.MagXOffset': mag_x_offset,
					'Calibration.MagYOffset': mag_x_offset,
					'Calibration.MagZOffset': mag_x_offset,
					}
				  )
