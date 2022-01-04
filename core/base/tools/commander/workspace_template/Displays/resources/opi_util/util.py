"""
utility API for OPI Script authors.
"""
from com.windhoverlabs.studio.registry import ConfigRegistry

def validate_opi(display):
    """
    Validate the OPI. Essentially check that we have all of the macros we need.

    :return bool: True if this opi is valid. False otherwise.
    """
    is_valid = True

    # getMacroValue returns a unicode object, NOT an str object. See "5.6.1. String Methods" of the python2.7.18 docs
    # for details.
    app_name = display.getMacroValue("APP")
    project_name = display.getMacroValue("PROJECT_NAME")

    if app_name is None or project_name is None:
        is_valid = False

    return is_valid


def get_events_from_registry(registry, module_path):
    """
    Fetch the events from the registry. This function assumes that the registry has been loaded.
    It is also assumed that the module path follows our configuration schema of "/modules/app"
    :param registry(com.windhoverlabs.studio.registry.ConfigRegistry): The registry object.
    :param module_path(str):
    :return: A dict object that has all of the events.
    """
    return registry.get(ConfigRegistry.appendPath(module_path, "events"))


def get_long_name_from_registry(registry, module_path):
    """
    This function assumes that the registry has been loaded.
    It is also assumed that the module path follows our configuration schema of "/modules/app"
    """
    return registry.get(ConfigRegistry.appendPath(module_path, "long_name"))


def get_short_name_from_registry(registry, module_path):
    """
    This function assumes that the registry has been loaded.
    It is also assumed that the module path follows our configuration schema of "/modules/app"
    """
    return registry.get(ConfigRegistry.appendPath(module_path, "short_name"))
