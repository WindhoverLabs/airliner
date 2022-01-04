"""
Script to send RegisterCmd command.
"""

# import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

msg_id = None
command_code = None
step = None
log = None

msgInputWidget = display.getWidget('msgIdInput')
cmdCodeInputWidget = display.getWidget('cmdCodeInput')
StepInputWidget = display.getWidget('Step')
LogInputWidget = display.getWidget('Log')

if not (msgInputWidget.getPropertyValue('pv_value') is None) and \
        not (cmdCodeInputWidget.getPropertyValue('pv_value') is None) and\
        not (StepInputWidget.getPropertyValue('pv_value') is None) and\
        not (LogInputWidget.getPropertyValue('pv_value') is None):

    msg_id = display.getWidget('msgIdInput').getPropertyValue('pv_value').getValue()
    command_code = display.getWidget('cmdCodeInput').getPropertyValue('pv_value').getValue()

    if StepInputWidget.getPropertyValue('pv_value').getValue() == "STEP_1":
        step = 'STEP_1'
    elif StepInputWidget.getPropertyValue('pv_value').getValue() == "STEP_2":
        step = 'STEP_2'
    if LogInputWidget.getPropertyValue('pv_value').getValue() == "LOG":
        log = 'LOG'
    elif LogInputWidget.getPropertyValue('pv_value').getValue() == "EXCLUDE":
        log = 'EXCLUDE_LOG'

    Yamcs.issueCommand('/cfs/' + display.getMacroValue('CPUID') + '/ci/RegisterCmd',
					   {'msgID': msg_id,
						'cmdCode': command_code,
						'step': step,
						'log':log
						}
					   )
