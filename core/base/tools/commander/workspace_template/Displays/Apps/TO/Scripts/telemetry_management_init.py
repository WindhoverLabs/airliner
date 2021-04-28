"""
General script that is used by all commands that require msg Ids/command codes such as AuthorizeCmd, DeauthorizeCmd, etc.
This generates a dropdown widgets that maps message ids to command codes.
"""

#import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

# FIXME: Perhaps these widget names should be macros
msg_id_drop_down = display.getWidget('msgIdInput')
app_name = display.getMacroValue('APP')

registry = YAMLRegistry()
all_messages = registry.getAllTelemetry()

app_messages = []


for msg in all_messages:
    if all_messages[msg]['app'] == app_name:
        app_messages.append(msg)

msg_id_drop_down.setPropertyValue('items', app_messages)
