"""
General script that is used by all commands that require msg Ids/command codes such as AuthorizeCmd, DeauthorizeCmd, etc.
This generates a dropdown widgets that maps message ids to command codes.
"""

#import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
# FIXME: Move this to the plugin as other apps will probably use it.
def get_all_config(modules, out_config):
    for module in modules:
        if 'modules' in modules[module]:
            get_all_config(modules[module], out_config)
        if 'config' in modules[module] and not (modules[module]['config'] is None):
            out_config.update({module: modules[module]['config']})


def get_max_to_channels():
    registry = YAMLRegistry()
    config = dict()
    get_all_config(registry.get('/')['modules'], config)

    return config['to']['TO_MAX_CHANNELS']['value']


# FIXME: Perhaps these widget names should be macros
msg_id_drop_down = display.getWidget('msgIdInput')
app_name = display.getMacroValue('APP')

registry = YAMLRegistry()
all_messages = registry.getAllTelemetry()

app_messages = []

for msg in all_messages:
    if all_messages[msg]['app'] == app_name:
        app_messages.append(msg)

try:
    display.getWidget('ChannelIndex').setPropertyValue('maximum', get_max_to_channels()-1)
except:
    print('No ChannelIndex on Display')


msg_id_drop_down.setPropertyValue('items', app_messages)
