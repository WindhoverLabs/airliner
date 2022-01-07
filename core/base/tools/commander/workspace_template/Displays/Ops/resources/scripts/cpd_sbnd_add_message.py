"""
Script to send AuthorizeCmd command.
"""

#import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

registry = YAMLRegistry()
all_messages = display.getWidget('CPDMsgID').getVar("all_msgs")
pv_value = display.getWidget('CPDMsgID').getPropertyValue('pv_value')

if pv_value is not None:
	msg_key = display.getWidget('CPDMsgID').getPropertyValue('pv_value').getValue()

	msgID = all_messages[msg_key]['msgID']
	
	Yamcs.issueCommand('/cfs/CPD/sbnd/AddCommand',{'Mid': msgID})
