from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

msgID = int(display.getWidget('MessageID').getPropertyValue('pv_value').getValue())
param_index = int(display.getWidget('FilterParmsIndex').getPropertyValue('pv_value').getValue())
filter_type = int(display.getWidget('FilterType').getPropertyValue('pv_value').getValue())
cpu_id = display.getMacroValue('CPUID')

cmd = '/cfs/' + cpu_id + '/ds/SetFilterType'

Yamcs.issueCommand(cmd,
                   {'MessageID': msgID,
                    'FilterParmsIndex': param_index,
                    'FilterType': filter_type,
                    'Padding': 0
                    }
                   )
