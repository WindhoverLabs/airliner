from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

Address =int(display.getWidget('Address').getPropertyValue('pv_value').getValue())
Size =int(display.getWidget('Size').getPropertyValue('pv_value').getValue())
MaxBytesPerCycle =int(display.getWidget('MaxBytesPerCycle').getPropertyValue('pv_value').getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/cs/OneShot'

Yamcs.issueCommand(cmd,
				  {'Address': Address,
                                   'Size':  Size,
				    'MaxBytesPerCycle':  MaxBytesPerCycle
				  }
				)