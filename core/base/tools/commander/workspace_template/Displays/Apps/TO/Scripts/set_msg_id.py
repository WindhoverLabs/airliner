cpu_id = display.getMacroValue('CPUID')
msg_key = display.getWidget('msgIdInput').getPropertyValue('pv_value').getValue()

if cpu_id in msg_key:
    # FIXME:This is very ugly and fragile I know. I will think about a better way of handling this.
    msg_key = msg_key.split(")")[1]

widget.setVar("msg_id", msg_key)
