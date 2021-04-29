"""
Script to send AuthorizeCmd command.
"""

#import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

registry = YAMLRegistry()
all_commands = registry.getAllCommands()

msgID = None
command_code = None
if not(display.getWidget('msgIdInput').getPropertyValue('pv_value') is None) and\
	not(display.getWidget('cmdCodeInput').getPropertyValue('pv_value') is None):
	msg_key = display.getWidget('msgIdInput').getPropertyValue('pv_value').getValue()
	command_code_key = display.getWidget('cmdCodeInput').getPropertyValue('pv_value').getValue()

	# NOTE: These might be redundant...
	if msg_key in all_commands:
		msgID = all_commands[msg_key]["msgID"]

		if command_code_key in all_commands[msg_key]['commands']:
			command_code = all_commands[msg_key]['commands'][command_code_key]['cc']

			Yamcs.issueCommand('/cfs/'+display.getMacroValue('CPUID')+'/ci/AuthorizeCmd',
							   {'msgID': msgID,
								 'cmdCode': command_code
								}
							  )
