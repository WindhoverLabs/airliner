"""
This script auto-generates event ids rows for each event id found in the YAML file. It also generates all of the
buttons for each command of each event.
"""
# import java packages
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil
from org.yamcs.studio.data import IPVListener
from java.lang import Runnable
from org.eclipse.swt.widgets import Display
from org.csstudio.opibuilder.scriptUtil import YAMLUtil
from  org.csstudio.opibuilder.properties import ActionsProperty, WidgetPropertyCategory
from org.csstudio.opibuilder.model import AbstractWidgetModel
# import python packages
import logging
import copy


# from  org.csstudio.opibuilder.scriptUtil import




def validate_opi():
    """
    Validate the OPI. Essentially check that we have all of the macros we need.

    :return bool: True if this opi is valid. False otherwise.
    """
    is_valid = True

    # getMacroValue returns a unicode object, NOT an str object. See "5.6.1. String Methods" of the python2.7.18 docs
    # for details.
    app_name = display.getMacroValue("APP")
    project_name = display.getMacroValue("PROJECT_NAME")
    yaml_path = display.getMacroValue("YAML_PATH")

    if app_name is None or project_name is None or yaml_path is None:
        is_valid = False

    return is_valid


def get_module(in_module, yaml_data):
    """
    Get the module data from yaml_data dict. Especially useful for when the modules are structured in
    a hierarchical fashion like our config files.

    :param in_module(str): The name of the module to look for.
    :param yaml_data(dcit): The dictionary that has the configruation data.

    :return dict: The dictionary with the data of module.
    """
    module_data = None
    for module in yaml_data['modules']:
        # FIXME: More succinct way of doing this?
        if 'modules' in yaml_data['modules'][module]:
            module_data = get_module(in_module, yaml_data['modules'][module])
        if in_module == module:
            module_data = yaml_data['modules'][in_module]

    return module_data


def get_events_from_yaml(module, yaml_path, logger):
    """
    Fetch the events from the YAML file.
    :param module(str): The name of the module(or app) from which we want to fetch events from.
    :param logger:
    :return: A dict object that has all of the events.
    """
    yamnl_data = YAMLUtil.parseYAML(yaml_path)
    module_data = get_module(module, yamnl_data)
    events = None

    if module_data:
        if 'events' in module_data:
            if not (module_data['events'] is None):
                events = module_data['events']
            else:
                logger.error("The events key is empty for module {}".format(module))
        else:
            logger.error("There is no events key present for module {}".format(module))
    else:
        logger.error("module {} does not exist".format(module))

    return events


def deep_copy_widget(widget):
    """
    Makes a deep copy of widget and returns it. Essentially it makes a new widget and copies all of the properties.
    """
    # FIXME: widget_id should not be an argument. We should be able to query this from widget object.
    new_widget = WidgetUtil.createWidgetModel(widget.getTypeID())
    for property_id in widget.getAllPropertyIDs():
        new_widget.addProperty((widget.getProperty(property_id)))

    return new_widget


def get_new_event_record_container(event_label):
    template_widget_recdord = display.getWidget("EventRecordTemplate").getWidgetModel()
    new_event_record_container = WidgetUtil.createWidgetModel(widget.getTypeID())
    new_event_record_container = deep_copy_widget(template_widget_recdord)
    for child in template_widget_recdord.getAllDescendants():
        if child.getPropertyValue("widget_type") == "Label":
            child.setPropertyValue("text", event_label)

        for

        copy.deepcopy(child)
        new_event_record_container.addChild(child, False)

    return new_event_record_container


def main():
    logging.basicConfig()
    logger = logging.getLogger('events_init')
    if validate_opi() is False:
        logger.warning("OPI is not valid. Ensure that the APP, PROJECT_NAME and YAML_PATH macros have some kind of "
                       "value.")
        return -1

    app_name = display.getMacroValue("APP")
    project_name = display.getMacroValue("PROJECT_NAME")
    yaml_path = FileUtil.workspacePathToSysPath(display.getMacroValue("YAML_PATH"))

    display.getWidget("app_name").setPropertyValue("text", app_name)

    events = get_events_from_yaml(app_name, yaml_path, logger)

    for event in events:
        event_label = "{}({})".format(event, events[event])
        new_container = get_new_event_record_container(event_label)

        display.getWidget("EventsTable").addChild(new_container)

    # Cleanup template once we are done.
    display.getWidget("EventsTable").removeChild(display.getWidget("EventsTable").getWidget("EventRecordTemplate"))


main()
