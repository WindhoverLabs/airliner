"""
Script to send DeauthorizeCmd command.
"""

# import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

registry = YAMLRegistry()
all_commands = registry.getAllCommands()

msg_id = None
command_code = None
if not (display.getWidget('msgIdInput').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('cmdCodeInput').getPropertyValue('pv_value') is None):
    msg_id = display.getWidget('msgIdInput').getPropertyValue('pv_value').getValue()
    command_code = display.getWidget('cmdCodeInput').getPropertyValue('pv_value').getValue()

    Yamcs.issueCommand('/cfs/' + display.getMacroValue('CPUID') + '/ci/DeauthorizeCmd',
					   {'msgID': msg_id,
						'cmdCode': command_code
						}
					   )
