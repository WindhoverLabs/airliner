from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

TableID = int(display.getWidget('TableID').getPropertyValue('pv_value').getValue())
Signature = str(display.getWidget('Signature').getPropertyValue('pv_value').getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/md/SetSignature'

Yamcs.issueCommand(cmd, {
    'TableId': TableID,
    'Signature': Signature,
    'Padding': 0})