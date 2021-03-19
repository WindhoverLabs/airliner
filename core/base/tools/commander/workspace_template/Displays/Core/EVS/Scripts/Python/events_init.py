"""
This script auto-generates event ids rows for each event id found in the YAML file. It essentially creates LinkingContainer[1]
widgets and uses EventRecord.opi to generates a container widget for each event in airliner configuration.

[1]:https://docs.yamcs.org/yamcs-studio/
"""
# import java packages

from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil, FileUtil
from org.eclipse.swt.graphics import RGB

from com.windhoverlabs.studio.registry import YAMLRegistry, ConfigRegistry

# import python packages
import logging
import sys

# import our own APIs. Not sure if this is the cleanest way of doing this. If we don't do it this way, we might have add
# an __init__.py to every directory in the project. Don't want to hardcode this path either; open to ideas about this.
sys.path.append(FileUtil.workspacePathToSysPath("Displays"))
from Resources.opi_util import util


def main():
    logging.basicConfig()
    logger = logging.getLogger('events_init')
    if util.validate_opi(display) is False:
        logger.warning("OPI is not valid. Ensure that the APP and PROJECT_NAME macros have some kind of "
                       "value.")
        return -1

    registry_path = display.getMacroValue("REGISTRY_PATH")

    registry = YAMLRegistry()

    app_name = display.getMacroValue("APP")
    project_name = display.getMacroValue("PROJECT_NAME")

    events = util.get_events_from_registry(registry, registry_path)

    for event in events:
        event_label = "{}  ({})  ".format(event, events[event]['id'])

        new_event_record = WidgetUtil.createWidgetModel("org.csstudio.opibuilder.widgets.linkingContainer")
        # TODO: Use the enumeration values from Studio to avoid magical strings
        new_event_record.setPropertyValue("opi_file", display.getMacroValue("TEMPLATE_OPI"))
        new_event_record.setPropertyValue("height", 20)
        new_event_record.setPropertyValue("width", 680)
        new_event_record.setPropertyValue("background_color", RGB(255, 255, 255))
        new_event_record.setPropertyValue("border_color", RGB(240, 240, 240))
        new_event_record.setPropertyValue("border_style", 0)
        new_event_record.setPropertyValue("name", event)

        display.getWidget("EventsTable").addChild(new_event_record)

        # Access children of new_event_record only AFTER they are added to the display container
        new_event_record.getChildByName("EventRecordTemplate").getChildByName("EventId").setPropertyValue("text",
                                                                                                          event_label)

        # Create the variables we need to be able to access data from button scripts.
        new_event_record.getChildByName("EventRecordTemplate").getChildByName("DeleteFilter").setPropertyValue(
            "pv_value", str(events[event]['id']))
        new_event_record.getChildByName("EventRecordTemplate").getChildByName("ResetFilter").setPropertyValue(
            "pv_value", str(events[event]['id']))

        event_macros = DataUtil.createMacrosInput(False)
        event_macros.put("EVENTID", str(events[event]['id']))
        new_event_record.getChildByName("EventRecordTemplate").setPropertyValue("macros", event_macros)

    long_name = util.get_long_name_from_registry(registry, registry_path)
    display.getWidget("app_name").setPropertyValue("text", long_name)


main()
