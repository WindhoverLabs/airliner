"""
Script to send DeregisterCmd command.
"""

# import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

registry = YAMLRegistry()
all_commands = registry.getAllCommands()

msgInputWidget = display.getWidget('msgIdInput')
cmdCodeInputWidget = display.getWidget('cmdCodeInput')
StepInputWidget = display.getWidget('Step')
LogInputWidget = display.getWidget('Log')

msgID = None
command_code = None
step = None
log = None

print('triggered')
if not (msgInputWidget.getPropertyValue('pv_value') is None) and \
        not (cmdCodeInputWidget.getPropertyValue('pv_value') is None) and \
        not (StepInputWidget.getPropertyValue('pv_value') is None) and \
        not (LogInputWidget.getPropertyValue('pv_value') is None):
    msg_key = msgInputWidget.getPropertyValue('pv_value').getValue()
    command_code_key = cmdCodeInputWidget.getPropertyValue('pv_value').getValue()

    if StepInputWidget.getPropertyValue('pv_value').getValue() == "STEP_1":
        step = 'STEP_1'
    elif StepInputWidget.getPropertyValue('pv_value').getValue() == "STEP_2":
        step = 'STEP_2'
    if LogInputWidget.getPropertyValue('pv_value').getValue() == "LOG":
        log = 'LOG'
    elif LogInputWidget.getPropertyValue('pv_value').getValue() == "EXCLUDE":
        log = 'EXCLUDE_LOG'

    # NOTE: These might be redundant...
    if msg_key in all_commands:
        msgID = all_commands[msg_key]["msgID"]

        if command_code_key in all_commands[msg_key]['commands']:
            command_code = all_commands[msg_key]['commands'][command_code_key]['cc']

            Yamcs.issueCommand('/cfs/' + display.getMacroValue('CPUID') + '/ci/UpdateCmd',
                               {
                                   'msgID': msgID,
                                   'cmdCode': command_code,
                                   'step': step,
                                   'log': log
                               }
                               )
