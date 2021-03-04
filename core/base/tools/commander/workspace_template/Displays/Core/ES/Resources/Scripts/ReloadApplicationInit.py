"""
This script auto-generates event ids rows for each event id found in the YAML file. It essentially creates LinkingContainer[1]
widgets and uses EventRecord.opi to generate a container widget for each event in airliner confuration.

[1]:https://docs.yamcs.org/yamcs-studio/
"""
# import java packages
from org.csstudio.opibuilder.scriptUtil import PVUtil, ScriptUtil, FileUtil, WidgetUtil, DataUtil
from org.csstudio.opibuilder.scriptUtil import YAMLUtil
from org.eclipse.swt.graphics import RGB

# import python packages
import logging


def is_opi_app_specific():
    """
    Determine if the OPI is opened in application specific or generic mode.
    
    :return bool: True if this opi is application specific. False otherwise.
    """
    is_specific = True

    # getMacroValue returns a unicode object, NOT an str object. See "5.6.1. String Methods" of the python2.7.18 docs
    # for details.
    app_name = display.getMacroValue("APP")
    project_name = display.getMacroValue("PROJECT_NAME")
    yaml_path = display.getMacroValue("CONFIG_REGISTRY")

    #if app_name is None or project_name is None or yaml_path is None:
    #    is_specific = False

    return is_specific


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


def main():
    logging.basicConfig()
    logger = logging.getLogger('ReloadApp')
    if is_opi_app_specific() is True:
        app_name = display.getMacroValue("APP")
        logger.warning(app_name)
        project_name = display.getMacroValue("PROJECT_NAME")
        logger.warning(project_name)
        yaml_path_macro = display.getMacroValue("CONFIG_REGISTRY")
        logger.warning(yaml_path_macro)
        yaml_path = FileUtil.workspacePathToSysPath(yaml_path_macro)

        yaml_data = YAMLUtil.parseYAML(yaml_path)
        module_data = get_module(app_name, yaml_data)

        display.getWidget("app_name").setPropertyValue("text", module_data["short_name"].upper() + " - Reload App")

main()
