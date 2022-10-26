from org.csstudio.display.builder.runtime.script import PVUtil, ScriptUtil
import math
widget.setPropertyValue('pv_value', math.degrees(PVUtil.getDouble(pvs[0])))