# import java packages
from org.yamcs.studio.script import Yamcs

channel = display.getWidget('Channel').getPropertyValue('pv_value').getValue()
dir_index = int(display.getWidget('Dir').getPropertyValue('pv_value').getValue())
cpu_id = display.getMacroValue("CPUID")
pChannel = -1

if channel == 'Channel 1':
    pChannel = 1
elif channel == 'Channel 2':
    pChannel = 2

Yamcs.issueCommand('/cfs/' + cpu_id + '/cf/EnableDirPolling', {
    'Chan': pChannel,
    'Dir': dir_index,
    'Spare_0_': 0,
    'Spare_1_': 0})
