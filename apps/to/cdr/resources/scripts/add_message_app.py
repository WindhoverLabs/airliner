"""
Script to send AuthorizeCmd command.
"""

# import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

registry = YAMLRegistry()
command_cpu_id = display.getMacroValue("CPUID")
msg_id_cpu = display.getWidget('msgIdInput').getVar('current_cpuid')
all_tlm = registry.getAllTelemetry('/modules/' + msg_id_cpu.lower() + "/modules")
msgID = None
msg_limit = None

if not (display.getWidget('msgIdInput').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('MsgLimit').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('PQueueIdx').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('ChannelIndex').getPropertyValue('pv_value') is None):
    msg_key = display.getWidget('msgIdInput').getVar("msg_id")
    msg_limit = int(display.getWidget('MsgLimit').getPropertyValue('pv_value').getValue())
    pq_index = int(display.getWidget('PQueueIdx').getPropertyValue('pv_value').getValue())
    channel_index = int(display.getWidget('ChannelIndex').getPropertyValue('pv_value').getValue())

    if msg_key in all_tlm:
        msgID = all_tlm[msg_key]["msgID"]

        Yamcs.issueCommand('/cfs/' + command_cpu_id.upper() + '/to/AddMessageFlow',
                           {'ChannelIdx': channel_index,
                            'MsgID': msgID,
                            'MsgLimit': msg_limit,
                            'PQueueIdx': pq_index
                            }
                           )
