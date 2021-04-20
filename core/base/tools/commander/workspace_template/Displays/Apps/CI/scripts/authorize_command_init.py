#import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

msg_id_drop_down = display.getWidget('msgIdInput')
cmd_code_drop_down = display.getWidget('cmdCodeInput')

registry = YAMLRegistry()

all_messages = registry.getAllCommands()
msg_id_drop_down.setPropertyValue('items', list(all_messages))



# print('triggered')