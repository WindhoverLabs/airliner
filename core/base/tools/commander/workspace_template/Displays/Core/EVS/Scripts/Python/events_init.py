"""
This script auto-generates event ids rows for each event id found in the YAML file. It essentially creates LinkingContainer[1]
widgets and uses EventRecord.opi to generates a container widget for each event in airliner confuration.

[1]:https://docs.yamcs.org/yamcs-studio/
"""
# import java packages
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil
from org.csstudio.opibuilder.scriptUtil import YAMLUtil
from org.eclipse.swt.graphics import RGB

# import python packages
import logging


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
    template_opi = display.getMacroValue("TEMPLATE_OPI")

    if app_name is None or project_name is None or yaml_path is None or template_opi is None:
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


def main():
    logging.basicConfig()
    logger = logging.getLogger('events_init')
    if validate_opi() is False:
        logger.warning("OPI is not valid. Ensure that the APP, PROJECT_NAME, TEMPLATE_OPI and YAML_PATH macros have some kind of "
                       "value.")
        return -1

    app_name = display.getMacroValue("APP")
    project_name = display.getMacroValue("PROJECT_NAME")
    yaml_path = FileUtil.workspacePathToSysPath(display.getMacroValue("YAML_PATH"))

    display.getWidget("app_name").setPropertyValue("text", app_name)

    events = get_events_from_yaml(app_name, yaml_path, logger)

    for event in events:
        event_label = "{}:{}".format(event, events[event])

        new_event_record = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
        # TODO: Use the enumeration values from Studio to avoid magical strings
        new_event_record.setPropertyValue("opi_file", display.getMacroValue("TEMPLATE_OPI"))
        new_event_record.setPropertyValue("height", 55)
        new_event_record.setPropertyValue("width", 685)
        new_event_record.setPropertyValue("background_color", RGB(255, 255, 255))
        new_event_record.setPropertyValue("border_color", RGB(240, 240, 240))
        new_event_record.setPropertyValue("border_style", 1)
        new_event_record.setPropertyValue("name", event)

        display.getWidget("EventsTable").addChild(new_event_record)

        # Access children of new_event_record only AFTER they are added to the display container
        new_event_record.getChildByName("EventRecordTemplate").getChildByName("EventId").setPropertyValue("text", event_label)


main()
