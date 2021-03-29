"""
Time Utilities
"""


def CFE_TIME_Sub2MicroSecs(SubSeconds):
    """
    Refer to airliner airliner/core/base/cfe/fsw/src/time/cfe_time_api.c
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
    Refere to airliner/core/base/cfe/fsw/src/time/cfe_time_api.c
    """
    SubSeconds = None

    if MicroSeconds > 999999:
        SubSeconds = 0xFFFFFFFF

    else:
        SubSeconds = ((((MicroSeconds << 11) / 5) << 3) / 3125) << 12

        if SubSeconds > 0x80001000:
            SubSeconds += 0x1000

    return SubSeconds
