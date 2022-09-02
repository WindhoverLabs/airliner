| Param         | Used When                                           | Description |
|---------------|-----------------------------------------------------|-------------|
| FW_RR_FF      | ControlRatesEnabled                                 | Used by `_roll_ctrl` in FAC.   | 
| FW_RR_P       | ControlRatesEnabled                                 | Used by `_roll_ctrl` in FAC.   |
| FW_RR_I       | ControlRatesEnabled                                 | Used by `_roll_ctrl` in FAC.   |
| FW_PR_I       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC. |
| FW_P_TC       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| FW_R_TC       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| FW_PR_FF       | ControlRatesEnabled                                | Used by `_pitch_ctrl` in FAC.  |
| FW_PR_P       | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| FW_RSP_OFF    | Dead code                                           | Dead code in FAC in FAC.  |
| FW_PSP_OFF    | Dead code                                           | Dead code in FAC.  |
| FW_L1_PERIOD  | ControlRatesEnabled                                 | Used by `_pitch_ctrl` in FAC.  |
| AIRSPD_TRIM  | m_VehicleControlModeMsg.ControlAutoEnabled && pos_sp_curr.Valid | Used by `__tecs` in FPC.  |
| AIRSPD_MIN   | m_VehicleControlModeMsg.ControlAutoEnabled && pos_sp_curr.Valid | Used by `__tecs` in FPC.  |
