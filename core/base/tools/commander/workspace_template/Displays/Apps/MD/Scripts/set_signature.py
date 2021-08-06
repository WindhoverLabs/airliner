from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs


TableID = int(display.getWidget('TableID').getPropertyValue('pv_value').getValue())
Signature = int(display.getWidget('Signature').getPropertyValue('pv_value').getValue())

Yamcs.issueCommand('/cfs/$(CPUID)/md/SetSignature', {
    'TableId': TableID,
    'Signature': Signature,
    'Padding': 0})