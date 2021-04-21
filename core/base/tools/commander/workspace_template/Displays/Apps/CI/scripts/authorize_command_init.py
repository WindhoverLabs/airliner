#import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

msg_id_drop_down = display.getWidget('msgIdInput')
cmd_code_drop_down = display.getWidget('cmdCodeInput')
app_name = display.getMacroValue('APP')

registry = YAMLRegistry()
all_commands = registry.getAllCommands()

app_commands = []

for msg in all_commands:
    if all_commands[msg]['app'] == app_name:
        app_commands.append(msg)


msg_id_drop_down.setPropertyValue('items', app_commands)

msg_key = str(pvs[0].getValue().getValue())

if msg_key in all_commands:
    cmd_code_drop_down.setPropertyValue('items', list(all_commands[msg_key]['commands']))
