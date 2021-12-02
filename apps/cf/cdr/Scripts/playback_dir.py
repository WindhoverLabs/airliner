from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB
from org.yamcs.studio.script import Yamcs

TransferClass =str(display.getWidget('DirTransferClass').getPropertyValue('pv_value').getValue())
Channel =str(display.getWidget('DirChannel').getPropertyValue('pv_value').getValue())
Priority =int(display.getWidget('DirPriority').getPropertyValue('pv_value').getValue())
Preserve =str(display.getWidget('DirPreserve').getPropertyValue('pv_value').getValue())
PeerEntityID =str(display.getWidget('DirPeerEntityID').getPropertyValue('pv_value').getValue())
Source =str(display.getWidget('DirSource').getPropertyValue('pv_value').getValue())
Destination =str(display.getWidget('DirDestination').getPropertyValue('pv_value').getValue())
cpu_id = display.getMacroValue('CPUID')

cmd = '/cfs/' + cpu_id + '/cf/PlaybackDirectory'

pTransferClass = ''
pChannel = -1
pPreserve = ''

if TransferClass == 'Class 1 (Unreliable)':
    pTransferClass = 'CLASS_1'
elif TransferClass == 'Class 2 (Reliable)':
    pTransferClass = 'CLASS_2'

if Channel == 'Channel 1':
    pChannel = 0
elif Channel == 'Channel 2':
    pChannel = 1

if Preserve == 'Delete' :
    pPreserve = 'DELETE'
elif Preserve == 'Keep':
    pPreserve = 'KEEP'

Yamcs.issueCommand(cmd, {
    'Class': pTransferClass,
    'Chan': pChannel,
    'Priority': Priority,
    'Preserve': pPreserve,
    'PeerEntityId': PeerEntityID,
    'SrcPath': Source,
    'DstPath': Destination})