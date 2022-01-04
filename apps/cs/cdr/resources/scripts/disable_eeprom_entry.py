from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

EntryID = int(display.getWidget('EntryID').getPropertyValue('pv_value').getValue())
cpu_id = display.getMacroValue('CPUID')

cmd = '/cfs/' + cpu_id + '/cs/DisableEntryEEPROM'

Yamcs.issueCommand(cmd, {'EntryID': EntryID })