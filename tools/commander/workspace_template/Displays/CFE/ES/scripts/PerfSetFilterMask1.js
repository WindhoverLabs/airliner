importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
importPackage(Packages.org.yamcs.studio.script);
importPackage(Packages.org.csstudio.simplepv);

var cmdString = '/cfs/cfe_es/PerfSetFilterMask(';

for(var i = 1; i <= 32; ++i) {
	var PerfID = 'PerfFilter' + i;
	var RequestFilterName = 'FilterRequest' + i;
	cmdString += PerfID + ': ' + VTypeHelper.getDouble(display.getWidget(RequestFilterName).getPropertyValue('pv_value'));	
}
cmdString =+ ')';

Yamcs.issueCommand(cmdString);