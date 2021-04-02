"""
   Please reference airliner/core/base/cfe/docs/cFE UsersGuide/Doxygen/cfetimeugformat.html for details.

   International Atomic Time (TAI)
   Coordinated Universal Time (UTC)

   TAI = MET + STCF
   UTC = TAI - Leap Seconds
   UTC = MET + STCF - Leap Seconds
"""

from com.windhoverlabs.studio.time import CFETime


def main():
    # FIXME: Add error checking. The pvs might not always be "there"(the user is not connected to the instance, etc)
    widget.setPropertyValue('text', str(CFETime.getRelativeTime(pvs[1].getValue().getValue(), pvs[2].getValue().getValue())))

main()
