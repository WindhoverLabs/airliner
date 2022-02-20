from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, DataUtil, FileUtil
from com.windhoverlabs.yamcs.script import Yamcs

appName = ScriptUtil.getWidgetValueByName(widget, 'inAppName').getValue()
entryPoint = ScriptUtil.getWidgetValueByName(widget, 'inEntryPoint').getValue()
fileName = ScriptUtil.getWidgetValueByName(widget, 'inFileName').getValue()
stackSize = int(ScriptUtil.getWidgetValueByName(widget, 'inStackSize').getValue())
exceptionAction = ScriptUtil.getWidgetValueByName(widget, 'inExceptionAction').getValue()
priority = int(ScriptUtil.getWidgetValueByName(widget, 'inPriority').getValue())

cpu_id = widget.getEffectiveMacros().getValue("CPUID");

Yamcs.issueCommand(widget, '/cfs/'+ cpu_id + '/cfe_es/StartApp', {
    'Payload.Application':     appName,
    'Payload.AppEntryPoint':   entryPoint,
    'Payload.AppFileName':     fileName,
    'Payload.StackSize':       stackSize,
    'Payload.ExceptionAction': exceptionAction,
    'Payload.Priority':        priority})