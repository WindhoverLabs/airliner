from org.csstudio.opibuilder.scriptUtil import PVUtil, WidgetUtil, DataUtil, ScriptUtil
from org.eclipse.swt.graphics import RGB
from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry
from org.csstudio.opibuilder.script import PVTuple
from org.yamcs.studio.data import IPVListener
from java.lang import Runnable
import math

try:
    registry = YAMLRegistry()

    max_channels = registry.getAllConfig()['to']['TO_MAX_CHANNELS']['value']
    display.getWidget('ChannelIndex').setPropertyValue('maximum', max_channels-1)
except:
    print('No ChannelIndex on Display')