from org.yamcs.studio.script import Yamcs

TransferClass = display.getWidget('TransferClass').getPropertyValue('pv_value').getValue()
Channel = display.getWidget('Channel').getPropertyValue('pv_value').getValue()
Priority = int(display.getWidget('Priority').getPropertyValue('pv_value').getValue())
Preserve = display.getWidget('Preserve').getPropertyValue('pv_value').getValue()

Dir = int(display.getWidget('Dir').getPropertyValue('pv_value').getValue())
PeerEntityID = display.getWidget('PeerEntityID').getPropertyValue('pv_value').getValue()
Source = display.getWidget('Source').getPropertyValue('pv_value').getValue()
Destination = display.getWidget('Destination').getPropertyValue('pv_value').getValue()
cpu_id = display.getMacroValue("CPUID")

pTransferClass = -1
pChannel = -1
pPreserve = -1

if TransferClass == 'Class 1 (Unreliable)':
    pTransferClass = 'CLASS_1'
elif TransferClass == 'Class 2 (Reliable)':
    pTransferClass = 'CLASS_2'

if Channel == 'Channel 1':
    pChannel = '1'
elif Channel == 'Channel 2':
    pChannel = '2'

if Preserve == 'Delete':
    pPreserve = 'DELETE'
elif Preserve == 'Keep':
    pPreserve = 'KEEP'

Yamcs.issueCommand('/cfs/' + cpu_id + '/cf/SetPollParameter', {
    'Chan': pChannel,
    'Dir': Dir,
    'Class': pTransferClass,
    'Priority': Priority,
    'Preserve': pPreserve,
    'Spare_0_': 0,
    'Spare_1_': 0,
    'Spare_2_': 0,
    'PeerEntityId': PeerEntityID,
    'SrcPath':  Source,
    'DstPath':  Destination})
