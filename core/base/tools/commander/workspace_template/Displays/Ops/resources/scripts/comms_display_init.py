"""
General script that is used by all commands that require msg Ids/command codes such as AuthorizeCmd, DeauthorizeCmd, etc.
This generates a dropdown widgets that maps message ids to command codes.
"""

#import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

# FIXME: Perhaps these widget names should be macros
ppdCombo = display.getWidget('PPDMsgID')
cpdCombo = display.getWidget('CPDMsgID')

registry = YAMLRegistry()
ppd_msgs = []
cpd_msgs = []
ppd_items = []
cpd_items = []

# Initialize the PPD displays. Just load it up with CPD commands for now.        
ppd_msgs = registry.getAllCommands('/modules/cpd/modules')
for msg in ppd_msgs:
	msgID = ppd_msgs[msg]['msgID']
	if type(msgID) == int:
		ppd_items.append(msg)

ppdCombo.setVar("all_msgs", ppd_msgs)
ppdCombo.setPropertyValue('items', sorted(ppd_items))

# Initialize the CPD displays.  Just load it up with CPD telemetry for now.        
cpd_msgs = registry.getAllTelemetry('/modules/cpd/modules')
for msg in cpd_msgs:
	msgID = cpd_msgs[msg]['msgID']
	if type(msgID) == int:
		cpd_items.append(msg)

cpdCombo.setVar("all_msgs", cpd_msgs)
cpdCombo.setPropertyValue('items', sorted(cpd_items))
