from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

table_id = int(display.getWidget('TableID').getPropertyValue('pv_value').getValue())
entry_id = int(display.getWidget('EntryID').getPropertyValue('pv_value').getValue())
field_length = int(display.getWidget('FieldLength').getPropertyValue('pv_value').getValue())
dwell_delay = int(display.getWidget('DwellDelay').getPropertyValue('pv_value').getValue())
sym_name = str(display.getWidget('SymbolName').getPropertyValue('pv_value').getValue())
offset = int(display.getWidget('Offset').getPropertyValue('pv_value').getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/md/Jam'

Yamcs.issueCommand(cmd, {
    'TableId': table_id,
    'EntryId': entry_id,
    'FieldLength': field_length,
    'DwellDelay': dwell_delay,
    'DwellAddress.Offset': offset,
    'DwellAddress.SymName': sym_name})