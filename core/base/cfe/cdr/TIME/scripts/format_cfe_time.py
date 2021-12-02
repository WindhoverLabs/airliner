"""
   Please reference airliner/core/base/cfe/docs/cFE UsersGuide/Doxygen/cfetimeugformat.html for details.

   International Atomic Time (TAI)
   Coordinated Universal Time (UTC)

   TAI = MET + STCF
   UTC = TAI - Leap Seconds
   UTC = MET + STCF - Leap Seconds
"""

from com.windhoverlabs.studio.time import CFETime
from com.windhoverlabs.studio.registry import YAMLRegistry

import logging

def main():
    logging.basicConfig()
    logger = logging.getLogger('TIME')

    registry = YAMLRegistry()

    # Don't specify an exception type on the except clause. If you do, Jython will not catch Java exceptions.
    try:
        display.getWidget("Met_Time_Field").setPropertyValue('text', str(CFETime.getRelativeTime(pvs[1].getValue().getValue(), pvs[2].getValue().getValue())))
    except:
        logger.error("Failed to fetch pvs for Met_Time_Field")
    display.getWidget("GPS_Time_Field").setPropertyValue('text', str(CFETime.getTimeSinceEpoch(pvs[1].getValue().getValue(), pvs[2].getValue().getValue(),
                                                                                               registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_YEAR/value"),
                                                                                               registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_DAY/value"),
                                                                                               registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_HOUR/value"),
                                                                                               registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_MINUTE/value"),
                                                                                               registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_SECOND/value"))))
    try:
        pass
    except:
        logger.error("Failed to fetch pvs for GPS_Time_Field")
    try:
        display.getWidget("CFS_Time_Field").setPropertyValue('text', str(CFETime.getTimeSinceCFSEpoch(pvs[1].getValue().getValue(), pvs[2].getValue().getValue())))
    except:
        logger.error("Failed to fetch pvs for CFS_Time_Field")

main()
