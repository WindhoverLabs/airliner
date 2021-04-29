from org.csstudio.opibuilder.scriptUtil import PVUtil, WidgetUtil, DataUtil, ScriptUtil
from org.eclipse.swt.graphics import RGB
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from org.csstudio.opibuilder.script import PVTuple
from org.yamcs.studio.data import IPVListener
from java.lang import Runnable
import math

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


try:
    display.getWidget('ChannelIndex').setPropertyValue('maximum', get_max_to_channels()-1)
except:
    print('No ChannelIndex on Display')