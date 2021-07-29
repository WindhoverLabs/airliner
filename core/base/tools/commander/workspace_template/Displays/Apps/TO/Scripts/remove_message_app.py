"""
Script to send RemoveMessageFlow command.
"""

# import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

registry = YAMLRegistry()
command_cpu_id = display.getMacroValue("CPUID")
msg_id_cpu = display.getWidget('msgIdInput').getVar('current_cpuid')
all_messages = registry.getAllTelemetry('/modules/' + msg_id_cpu.lower() + "/modules")

msgID = None
channel_index = None
if not (display.getWidget('msgIdInput').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('ChannelIndex').getPropertyValue('pv_value') is None):
	msg_key = display.getWidget('msgIdInput').getVar("msg_id")
	channel_index = int(display.getWidget('ChannelIndex').getPropertyValue('pv_value').getValue())

	# NOTE: These might be redundant...
	if msg_key in all_messages:
		msgID = all_messages[msg_key]["msgID"]

		Yamcs.issueCommand('/cfs/' + command_cpu_id.upper() + '/to/RemoveMessageFlow',
                           {	'ChannelIdx': channel_index,
							   	'MsgID': msgID
                            }
                           )
