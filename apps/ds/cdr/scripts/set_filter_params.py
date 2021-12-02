from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

msgID         = int(display.getWidget('MessageID').getPropertyValue('pv_value').getValue())
param_index   = int(display.getWidget('FilterParmsIndex').getPropertyValue('pv_value').getValue())
Algorithm_N   = int(display.getWidget('Algorithm_N').getPropertyValue('pv_value').getValue())
Algorithm_X   = int(display.getWidget('Algorithm_X').getPropertyValue('pv_value').getValue())
Algorithm_O   = int(display.getWidget('Algorithm_O').getPropertyValue('pv_value').getValue())

cpu_id = display.getMacroValue('CPUID')

cmd = '/cfs/' + cpu_id + '/ds/SetFilterParams'

Yamcs.issueCommand(cmd,
				  {'MessageID': msgID,
				    'FilterParmsIndex':  param_index,
				    'Algorithm_N': Algorithm_N,
				    'Algorithm_X': Algorithm_X,
				    'Algorithm_O': Algorithm_O,
				    'Padding':  0
				  }
				 );