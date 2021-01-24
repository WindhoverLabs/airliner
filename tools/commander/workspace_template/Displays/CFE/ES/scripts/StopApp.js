importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
importPackage(Packages.org.yamcs.studio.script);

var appName = display.getWidget("inAppName").getPropertyValue("text");

Yamcs.issueCommand('/cfs/cfe_es/StopApp(Name: ' + appName + ')');
