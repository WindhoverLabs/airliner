//Script to demonstrate auto-generation pattern of an OPI that is tailored to the event commands of an APP.

importPackage(Packages.org.csstudio.opibuilder.scriptUtil);
importPackage(org.csstudio.opibuilder.properties);
importPackage(org.csstudio.opibuilder.model);

var app_name  =       display.getMacroValue("APP");

//If you want to add more commands(another column), just add the name of the command to this array. Do note that
//you'll need special logic for adding another XML script to the button that fires that command. Hopefully the pattern below
//makes this obvious. 
var commands = ['Reset'];
if(app_name != null && app_name!="")
{
    display.getWidget("app_name").setPropertyValue("text",app_name);
    for(var i =0;i<8;i++)
	{
	    for(var j =0;j<commands.length;j++)
	       {
		   //The following macro is NOT used here. It's just here for documentation purposes.
		   var event_id_macro_input = DataUtil.createMacrosInput(true);
		   event_id_macro_input.put("EVENT_ID", i);
		   event_id_macro_input.put("APP", "TO");

		    var label_widget =          WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.Label");
		    var button_widget =         WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.ActionButton");
		    var actions =               FileUtil.loadXMLFile("/Displays/Core/EVS/Scripts/XML/ResetEvent.xml");
		    var action_property =       new ActionsProperty(AbstractWidgetModel.PROP_ACTIONS, "Actions", WidgetPropertyCategory.Behavior, false);
		    var xml_input_property =    action_property.readValueFromXML(actions);

		    action_property.setPropertyValue(xml_input_property);
		    button_widget.addProperty(action_property);
		    //The "pv_value" property is used as the event id by "ResetEvent" script. This is becasuse non-container widgets such as ActionButton
		    //do not have macros of their own. However they do have pv_values one may use.
		    button_widget.setPropertyValue("pv_value", i);
		    button_widget.setPropertyValue("text", commands[j]);

		   display.addChild(label_widget);
		   display.addChild(button_widget);
		}
	}

}