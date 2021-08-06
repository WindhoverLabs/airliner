from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

table_mask = int(display.getWidget('TableMask').getPropertyValue('pv_value').getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/md/Start'

Yamcs.issueCommand(cmd, {
    'TableMask': table_mask})