"""
This script auto-generates an instance of the AppControl OPI. It essentially creates LinkingContainer[1]
widgets and links it to ApplicationControl_App.opi.

[1]:https://docs.yamcs.org/yamcs-studio/
"""
# import java packages
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil
from com.windhoverlabs.studio.registry import YAMLRegistry
from org.eclipse.swt.graphics import RGB

# import python packages
import logging
import sys

# import our own APIs. Not sure if this is the cleanest way of doing this. If we don't do it this way, we might have add
# an __init__.py to every directory in the project. Don't want to hardcode this path either; open to ideas about this.
sys.path.append(FileUtil.workspacePathToSysPath("Displays"))
from Resources.opi_util import util


def is_opi_app_specific():
    """
    Determine if the OPI is opened in application specific or generic mode.
    
    :return bool: True if this opi is application specific. False otherwise.
    """
    is_specific = True

    return is_specific


def main():
    logging.basicConfig()
    logger = logging.getLogger('AppControl')
    if is_opi_app_specific() is True:
        app_name = display.getMacroValue("APP")
        logger.warning(app_name)
        project_name = display.getMacroValue("PROJECT_NAME")
        logger.warning(project_name)

        registry_path = display.getMacroValue("REGISTRY_PATH").lower()

        registry = YAMLRegistry()

        display.getWidget("app_name").setPropertyValue("text", util.get_short_name_from_registry(registry,
                                                                                                 registry_path).upper() + " - App Control")

        print('app name:{}'.format(util.get_short_name_from_registry(registry,registry_path).upper()))


main()
