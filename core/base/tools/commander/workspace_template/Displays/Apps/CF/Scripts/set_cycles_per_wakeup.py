from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

cycles = int(display.getWidget('Spinner').getPropertyValue('pv_value').getValue())
cpu_id = display.getMacroValue('CPUID')

Yamcs.issueCommand('/cfs/' + cpu_id + '/cf/SetCyclesPerWakeup', {'NumCyclesPerWakeup': cycles})
