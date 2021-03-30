"""
CFE Time Utilities
"""
from com.windhoverlabs.studio.registry import ConfigRegistry

# FIXME: This module might get messy if we keep adding functions to it...perhaps we should re-structure it differently.

def get_cfe_time_fs_factor(registry, registry_path):
    """
    This function assumes that registry has been loaded by the caller.
    """
    fs_factor = registry.get(ConfigRegistry.appendPath(registry_path, "config/CFE_TIME_FS_FACTOR/"))

    return fs_factor['value']


def micro_2_seconds(microseconds):
    seconds = microseconds / 1000000

    return seconds


def CFE_TIME_Sub2MicroSecs(SubSeconds):
    """
    Refer to airliner/core/base/cfe/fsw/src/time/cfe_time_api.c
    """
    MicroSeconds = None
    if SubSeconds > 0xffffdf00:
        MicroSeconds = 999999
    else:
        MicroSeconds = (((((SubSeconds >> 7) * 125) >> 7) * 125) >> 12)

        if (SubSeconds & 0x3ffffff) != 0:
            MicroSeconds += 1

        if MicroSeconds > 500000:
            MicroSeconds -= 1

    return MicroSeconds


def CFE_TIME_Micro2SubSecs(MicroSeconds):
    """
    Refer to airliner/core/base/cfe/fsw/src/time/cfe_time_api.c
    """
    SubSeconds = None

    if MicroSeconds > 999999:
        SubSeconds = 0xFFFFFFFF

    else:
        SubSeconds = ((((MicroSeconds << 11) / 5) << 3) / 3125) << 12

        if SubSeconds > 0x80001000:
            SubSeconds += 0x1000

    return SubSeconds


def CFE_TIME_CFE2FSSeconds(SecondsCFE):
    """
    Refer to airliner/core/base/cfe/fsw/src/time/cfe_time_api.c
    """

    # Using a signed integer allows the factor to be negative...
    registry = YAMLRegistry()
    # hardcoding the path for now
    ConvertFactor = get_cfe_time_fs_factor(registry, "/modules/core/modules/cfe/modules/cfe_time")

    # File system time = cFE time + conversion factor...
    SecondsFS = SecondsCFE + ConvertFactor

    # Prevent file system time from going below zero...
    if ConvertFactor < 0:
        if -ConvertFactor > SecondsCFE:
            SecondsFS = 0

    return SecondsFS
