"""
Script to send AuthorizeCmd command.
"""

# import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

registry = YAMLRegistry()
all_tlm = registry.getAllTelemetry()

msgID = None
msg_limit = None
if not (display.getWidget('msgIdInput').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('MsgLimit').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('PQueueIdx').getPropertyValue('pv_value') is None) and \
        not (display.getWidget('ChannelIndex').getPropertyValue('pv_value') is None):
    msg_key = display.getWidget('msgIdInput').getPropertyValue('pv_value').getValue()
    msg_limit = int(display.getWidget('MsgLimit').getPropertyValue('pv_value').getValue())
    pq_index = int(display.getWidget('PQueueIdx').getPropertyValue('pv_value').getValue())
    channel_index = int(display.getWidget('ChannelIndex').getPropertyValue('pv_value').getValue())

    # NOTE: These might be redundant...
    if msg_key in all_tlm:
        msgID = all_tlm[msg_key]["msgID"]

        Yamcs.issueCommand('/cfs/' + display.getMacroValue('CPUID') + '/to/AddMessageFlow',
                           {'ChannelIdx': channel_index,
                            'MsgID': msgID,
                            'MsgLimit': msg_limit,
                            'PQueueIdx': pq_index
                            }
                           )
