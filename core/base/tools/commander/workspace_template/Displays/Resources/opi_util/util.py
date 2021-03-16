"""
utility API for OPI Script authors.
"""

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