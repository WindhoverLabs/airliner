from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

appName = display.getWidget('inAppName').getPropertyValue('pv_value').getValue()
entryPoint = display.getWidget("inEntryPoint").getPropertyValue("pv_value").getValue()
fileName = display.getWidget("inFileName").getPropertyValue("pv_value").getValue()
stackSize = int(display.getWidget("inStackSize").getPropertyValue("pv_value").getValue())
exceptionAction = display.getWidget('inExceptionAction').getPropertyValue('pv_value').getValue()
priority = int(display.getWidget("inPriority").getPropertyValue("pv_value").getValue())

cpu_id = display.getMacroValue("CPUID")

Yamcs.issueCommand('/cfs/'+ cpu_id + '/cfe_es/StartApp', {
    'Payload.Application':         appName,
    'Payload.AppEntryPoint':   entryPoint,
    'Payload.AppFileName':     fileName,
    'Payload.StackSize':             stackSize,
    'Payload.ExceptionAction': exceptionAction,
    'Payload.Priority':                  priority})