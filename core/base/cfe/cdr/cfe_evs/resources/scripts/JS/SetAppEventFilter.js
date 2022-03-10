importPackage(com.windhoverlabs.yamcs.script);
importPackage(Packages.org.csstudio.display.builder.runtime.script);

var appName = widget.getEffectiveMacros().getValue("APP").toUpperCase();
var strMask = PVUtil.getString(ScriptUtil.getPrimaryPV(ScriptUtil.findWidgetByName(widget, 'Mask')));
var eventID = widget.getEffectiveMacros().getValue("EID")

if(strMask == "None (0x0000)") {
	mask = "0x0000"
} else if(strMask == "First 1 Stop (0xFFFF)") {
	mask = "0xFFFF"
} else if(strMask == "First 2 Stop (0xFFFE)") {
	mask = "0xFFFE"
} else if(strMask == "First 4 Stop (0xFFFC)") {
	mask = "0xFFFC"
} else if(strMask == "First 8 Stop (0xFFF8)") {
	mask = "0xFFF8"
} else if(strMask == "First 16 Stop (0xFFF0)") {
	mask = "0xFFF0"
} else if(strMask == "First 32 Stop (0xFFE0)") {
	mask = "0xFFE0"
} else if(strMask == "First 64 Stop (0xFFC0)") {
	mask = "0xFFC0"
} else if(strMask == "Every Other One (0x0001)") {
	mask = "0x0001"
} else if(strMask == "Every Other One (0x0002)") {
	mask = "0x0002"
} else if(strMask == "Every Other One (0x0003)") {
	mask = "0x0003"
} else {
	mask = strMask
}

Yamcs.issueCommand(widget, '/cfs/$(CPUID)/cfe_evs/SetAppEventFilter', {
    'Payload.AppName':  appName,
    'Payload.EventID': eventID,
    'Payload.Mask':  mask
});

