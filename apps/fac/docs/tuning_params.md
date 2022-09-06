| Param         | Used When                                           | Description |
|---------------|-----------------------------------------------------|-------------|
| FW_RR_FF      | ControlRatesEnabled                                 | Used by `_roll_ctrl` in FAC.   | 
| FW_RR_P       | ControlRatesEnabled                                 | Used by `_roll_ctrl` in FAC.   |
| FW_RR_I       | ControlRatesEnabled                                 | Used by `_roll_ctrl` in FAC.   |
| FW_PR_I       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC. |
| FW_P_TC       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| FW_R_TC       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| FW_PR_FF      | ControlRatesEnabled                                | Used by `_pitch_ctrl` in FAC.  |
| FW_PR_P       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| FW_RSP_OFF    | Dead code                                           | Dead code in FAC in FAC.  |
| FW_PSP_OFF    | Dead code                                           | Dead code in FAC.  |
| FW_L1_PERIOD  | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| AIRSPD_TRIM  | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO` | Used by `__tecs` in FPC.    |
| AIRSPD_MIN   | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO` | Used by `__tecs` in FPC.    |
| AIRSPD_MAX   | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO` |  Used by `__tecs` in FPC.   |
| THR_CRUISE    | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO` |  Used by `__tecs` in FPC.  |
| THR_MIN     | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO` |  Used by `__tecs` in FPC.    |
| THR_MAX     | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO` or `FW_POSCTRL_MODE_OTHER`      |  Used by `__tecs` in FPC.  |
| T_CLMB_MAX     | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO` |  Used by `__tecs` in FPC.      |
| T_SINK_MAX      | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or `FW_POSCTRL_MODE_AUTO`  |  Used by `__tecs` in FPC.    |
| T_SINK_MIN      | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or  `FW_POSCTRL_MODE_AUTO` |  Used by `__tecs` in FPC.    |
| T_TIME_CONST      | `FW_POSCTRL_MODE_POSITION` or `FW_POSCTRL_MODE_ALTITUDE` or  `FW_POSCTRL_MODE_AUTO` |  Used by `__tecs` in FPC.  |
