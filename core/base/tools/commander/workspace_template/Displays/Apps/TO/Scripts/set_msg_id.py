cpu_id = display.getMacroValue('CPUID')
msg_key = display.getWidget('msgIdInput').getPropertyValue('pv_value').getValue()
all_cpus = eval(display.getMacroValue('ALL_CPUS'))

for cpu in all_cpus:
    if cpu in msg_key:
        # FIXME:This is very ugly and fragile I know. I will think about a better way of handling this.
        msg_key = msg_key.split(") ")[1]
        cpu_id = cpu
        # Assuming all cpu names are unique(and they should be!), we can move on and break out of the loop
        break

widget.setVar("msg_id", msg_key)
widget.setVar("current_cpuid", cpu_id)