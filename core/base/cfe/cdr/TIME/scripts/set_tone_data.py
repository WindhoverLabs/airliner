from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

METSeconds =int(display.getWidget('METSeconds').getPropertyValue('pv_value').getValue())
METSubseconds =int(display.getWidget('METSubseconds').getPropertyValue('pv_value').getValue())
STCFSeconds =int(display.getWidget('STCFSeconds').getPropertyValue('pv_value').getValue())
STCFSubseconds =int(display.getWidget('STCFSubseconds').getPropertyValue('pv_value').getValue())
Leaps =int(display.getWidget('Leaps').getPropertyValue('pv_value').getValue())
State =int(display.getWidget('State').getPropertyValue('pv_value').getValue())

cpu_id = display.getMacroValue('CPUID')
cmd = '/cfs/' + cpu_id + '/cfe_time/ToneData'


Yamcs.issueCommand(cmd, {
'Payload.AtToneMET.Seconds': METSeconds,
'Payload.AtToneMET.Subseconds': METSubseconds,
'Payload.AtToneSTCF.Seconds': STCFSeconds,
'Payload.AtToneSTCF.Subseconds': STCFSubseconds,
'Payload.AtToneLeaps': Leaps,
'Payload.AtToneState': State})
