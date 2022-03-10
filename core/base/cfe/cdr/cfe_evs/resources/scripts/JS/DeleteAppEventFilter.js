importPackage(com.windhoverlabs.yamcs.script);
importPackage(Packages.org.csstudio.display.builder.runtime.script);

var appName = widget.getEffectiveMacros().getValue("APP").toUpperCase();
var eventID = widget.getEffectiveMacros().getValue("EID")

Yamcs.issueCommand(widget, '/cfs/$(CPUID)/cfe_evs/DeleteAppEventFilter', {
    'Payload.AppName':  appName,
    'Payload.EventID': eventID
});

