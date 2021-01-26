importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
importPackage(Packages.org.yamcs.studio.script);
importPackage(Packages.org.yamcs.studio.data);

var strMode =VTypeHelper.getString(display.getWidget('inMode').getPropertyValue('pv_value'));

Yamcs.issueCommand('/cfs/cfe_es/SetSysLogMode', {'Payload.Mode': strMode});