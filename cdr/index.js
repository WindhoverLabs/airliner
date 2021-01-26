'use strict';

var path = require('path');

const CdrFlightAppsPlugin = require(path.join(global.CDR_INSTALL_DIR, '/commander/classes/CdrFlightAppsPlugin')).CdrFlightAppsPlugin;

module.exports = class CfeCdrFlightAppsPlugin extends CdrFlightAppsPlugin {
    constructor(config) {
        config.name = 'px4lib';
        config.webRoot = path.join( __dirname, 'web');  
        super(config);
        
        var content = {
            px4lib: {
        	    shortDescription: 'PX4',
        	    longDescription: 'PX4 Message Displays',
        	    nodes: {
        	        Dataplot: {
        	            type: CdrFlightAppsPlugin.ContentType.PANEL,
        	            shortDescription: 'Dataplot',
        	            longDescription: 'Generic Dataplot',
        	            filePath: '/data_plot.pug'
        	        },
        	        PX4_ActuatorArmed: {
        	        	shortDescription: 'Actuator Armed',
        	        	longDescription: 'Actuator Armed',
        	        	nodes: {
                	        PX4_ActuatorArmedMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Actuator Armed Message',
                	            filePath: '/PX4_ActuatorArmedMsg.lyt'
                	        },
                	        PX4_ActuatorArmedMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Actuator Armed Message',
                	            filePath: '/PX4_ActuatorArmedMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_ActuatorControls: {
        	        	shortDescription: 'Actuator Controls',
        	        	longDescription: 'Actuator Controls',
        	        	nodes: {
	            	        PX4_ActuatorControlsMsg: {
	            	            type: CdrFlightAppsPlugin.ContentType.PANEL,
	            	            shortDescription: 'Message',
	            	            longDescription: 'PX4 Actuator Controls Message',
	            	            filePath: '/PX4_ActuatorControlsMsg.pug'
	            	        },
	            	        PX4_ActuatorControlsMsgLyt: {
	            	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
	            	            shortDescription: 'Main',
	            	            longDescription: 'PX4 Actuator Controls Message',
	            	            filePath: '/PX4_ActuatorControlsMsg.lyt'
	            	        }
        	        	}
        	        },
        	        PX4_ActuatorsOutputs: {
        	        	shortDescription: 'Actuator Outputs',
        	        	longDescription: 'Actuator Outputs',
        	        	nodes: {
                	        PX4_ActuatorOutputsMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Actuator Outputs Message',
                	            filePath: '/PX4_ActuatorOutputsMsg.lyt'
                	        },
                	        PX4_ActuatorOutputsMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Actuator Outputs Message',
                	            filePath: '/PX4_ActuatorOutputsMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_Airspeed: {
        	        	shortDescription: 'Airspeed',
        	        	longDescription: 'Airspeed',
        	        	nodes: {
                	        PX4_AirspeedMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Air Speed Message',
                	            filePath: '/PX4_AirspeedMsg.lyt'
                	        },
                	        PX4_AirspeedMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Air Speed Message',
                	            filePath: '/PX4_AirspeedMsg.pug'
                	        },
                	        PX4_AirspeedMsg_AirTemperature_dp: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Air Temperature',
                	            longDescription: 'Air Temperature',
                	            filePath: '/PX4_AirspeedMsg_AirTemperature_dp.pug'
                	        },
                	        PX4_AirspeedMsg_IndicatedAirspeed_dp: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Indicated',
                	            longDescription: 'Indicated Airspeed',
                	            filePath: '/PX4_AirspeedMsg_IndicatedAirspeed_dp.pug'
                	        },
                	        PX4_AirspeedMsg_TrueAirspeed_dp: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'True',
                	            longDescription: 'True Airspeed',
                	            filePath: '/PX4_AirspeedMsg_TrueAirspeed_dp.pug'
                	        },
                	        PX4_AirspeedMsg_Unfiltered: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'True - Unfiltered',
                	            longDescription: 'True Airspeed - Unfiltered.',
                	            filePath: '/PX4_AirspeedMsg_Unfiltered_dp.pug'
                	        }
        	        	}
        	        },
        	        PX4_BatteryStatus: {
        	        	shortDescription: 'Battery Status',
        	        	longDescription: 'Battery Status',
        	        	nodes: {
                	        PX4_BatteryStatusMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Battery Status Message',
                	            filePath: '/PX4_BatteryStatusMsg.lyt'
                	        },
                	        PX4_BatteryStatusMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Battery Status Message',
                	            filePath: '/PX4_BatteryStatusMsg.pug'
                	        },
                	        PX4_BatteryStatusMsg_Current: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Current',
                	            longDescription: 'Battery Current',
                	            filePath: '/PX4_BatteryStatusMsg_Current.pug'
                	        },
                	        PX4_BatteryStatusMsg_CurrentFiltered: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Current (filtered)',
                	            longDescription: 'Battery Current (filtered)',
                	            filePath: '/PX4_BatteryStatusMsg_CurrentFiltered.pug'
                	        },
                	        PX4_BatteryStatusMsg_Discharged: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Discharged',
                	            longDescription: 'Battery Discharged',
                	            filePath: '/PX4_BatteryStatusMsg_Discharged.pug'
                	        },
                	        PX4_BatteryStatusMsg_Remaining: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Remaining',
                	            longDescription: 'Remaining battery charge.',
                	            filePath: '/PX4_BatteryStatusMsg_Remaining.pug'
                	        },
                	        PX4_BatteryStatusMsg_Voltage: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Voltage',
                	            longDescription: 'Battery voltage',
                	            filePath: '/PX4_BatteryStatusMsg_Voltage.pug'
                	        },
                	        PX4_BatteryStatusMsg_VotlageFiltered: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Voltage (filtered)',
                	            longDescription: 'Battery voltage (filtered).',
                	            filePath: '/PX4_BatteryStatusMsg_VoltageFiltered.pug'
                	        }
        	        	}
        	        },
        	        PX4_CommanderState: {
        	        	shortDescription: 'Commander State',
        	        	longDescription: 'Commander State',
        	        	nodes: {
                	        PX4_CommanderStateMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Commander State Message',
                	            filePath: '/PX4_CommanderStateMsg.lyt'
                	        },
                	        PX4_CommanderStateMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Commander State Message',
                	            filePath: '/PX4_CommanderStateMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_ControlState: {
        	        	shortDescription: 'Control State',
        	        	longDescription: 'Control State',
        	        	nodes: {
                	        PX4_ControlStateMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Control State Message',
                	            filePath: '/PX4_ControlStateMsg.lyt'
                	        },        	        PX4_ControlStateMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Control State Message',
                	            filePath: '/PX4_ControlStateMsg.pug'
                	        },
                	        PX4_ControlStateMsg_Acc: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Acc',
                	            longDescription: 'Acc',
                	            filePath: '/PX4_ControlStateMsg_Acc.pug'
                	        },
                	        PX4_ControlStateMsg_Airspeed: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Airspeed',
                	            longDescription: 'Airspeed',
                	            filePath: '/PX4_ControlStateMsg_Airspeed.pug'
                	        },
                	        PX4_ControlStateMsg_HorizAccMag: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Horiz Acc Mag',
                	            longDescription: 'Horiz Acc Mag',
                	            filePath: '/PX4_ControlStateMsg_HorizAccMag.pug'
                	        },
                	        PX4_ControlStateMsg_Pos: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Position',
                	            longDescription: 'Position',
                	            filePath: '/PX4_ControlStateMsg_Pos.pug'
                	        },
                	        PX4_ControlStateMsg_PosVariance: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Position Variance',
                	            longDescription: 'Position Variance',
                	            filePath: '/PX4_ControlStateMsg_PosVariance.pug'
                	        },
                	        PX4_ControlStateMsg_RateBias: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Rate Bias',
                	            longDescription: 'Rate Bias',
                	            filePath: '/PX4_ControlStateMsg_RateBias.pug'
                	        },
                	        PX4_ControlStateMsg_Rates: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Rates',
                	            longDescription: 'Rates',
                	            filePath: '/PX4_ControlStateMsg_Rates.pug'
                	        },
                	        PX4_ControlStateMsg_Vel: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Velocity',
                	            longDescription: 'Velocity',
                	            filePath: '/PX4_ControlStateMsg_Vel.pug'
                	        },
                	        PX4_ControlStateMsg_VelVariance: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Velocity Variance',
                	            longDescription: 'Velocity Variance',
                	            filePath: '/PX4_ControlStateMsg_VelVariance.pug'
                	        }
        	        	}
        	        },
        	        PX4_DifferentialPressure: {
        	        	shortDescription: 'Differential Pressure',
        	        	longDescription: 'Differential Pressure',
        	        	nodes: {
                	        PX4_DifferentialPressureMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Differential Pressure Message',
                	            filePath: '/PX4_DifferentialPressureMsg.lyt'
                	        },        	        PX4_DifferentialPressureMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Differential Pressure Message',
                	            filePath: '/PX4_DifferentialPressureMsg.pug'
                	        },
                	        PX4_DifferentialPressureMsg_DiffPress: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Differential Pressure',
                	            longDescription: 'Differential Pressure',
                	            filePath: '/PX4_DifferentialPressureMsg_DiffPress.pug'
                	        },
                	        PX4_DifferentialPressureMsg_Temperature: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Temperature',
                	            longDescription: 'Temperature',
                	            filePath: '/PX4_DifferentialPressureMsg_Temperature.pug'
                	        }
        	        	}
        	        },
        	        PX4_DistanceSensor: {
        	        	shortDescription: 'Distance Sensor',
        	        	longDescription: 'Distance Sensor',
        	        	nodes: {
                	        PX4_DistanceSensorMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Distance Sensor Message',
                	            filePath: '/PX4_DistanceSensorMsg.lyt'
                	        },
                	        PX4_DistanceSensorMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Distance Sensor Message',
                	            filePath: '/PX4_DistanceSensorMsg.pug'
                	        },
                	        PX4_DistanceSensorMsg_CurrentDistance: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'CurrentDistance',
                	            longDescription: 'Distance Sensor - CurrentDistance',
                	            filePath: '/PX4_DistanceSensorMsg_CurrentDistance.pug'
                	        },
                	        PX4_DistanceSensorMsg_Covariance: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Covariance',
                	            longDescription: 'Distance Sensor - Covariance',
                	            filePath: '/PX4_DistanceSensorMsg_Covariance.pug'
                	        }
        	        	}
        	        },
        	        PX4_EstimatorStatus: {
        	        	shortDescription: 'Estimator Status',
        	        	longDescription: 'Estimator Status',
        	        	nodes: {
                	        PX4_EstimatorStatusMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Estimator Status Message',
                	            filePath: '/PX4_EstimatorStatusMsg.lyt'
                	        },
                	        PX4_EstimatorStatusMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Estimator Status Message',
                	            filePath: '/PX4_EstimatorStatusMsg.pug'
                	        },
                	        PX4_EstimatorStatusMsg_Covariances: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Covariances',
                	            longDescription: 'Estimator Covariances',
                	            filePath: '/PX4_EstimatorStatusMsg_Covariances.pug'
                	        },
                	        PX4_EstimatorStatusMsg_States: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'States',
                	            longDescription: 'Estimator States',
                	            filePath: '/PX4_EstimatorStatusMsg_States.pug'
                	        }
        	        	}
        	        },
        	        PX4_GpsInjectData: {
        	        	shortDescription: 'GPS Inject Data',
        	        	longDescription: 'GPS Inject Data',
        	        	nodes: {
                	        PX4_GpsInjectDataMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 GPS Inject Data Message',
                	            filePath: '/PX4_GpsInjectDataMsg.lyt'
                	        },
                	        PX4_GpsInjectDataMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 GPS Inject Data Message',
                	            filePath: '/PX4_GpsInjectDataMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_HomePosition: {
        	        	shortDescription: 'Home Position',
        	        	longDescription: 'Home Position',
        	        	nodes: {
        	        		PX4_HomePositionMsgLyt: {
        	                    type: CdrFlightAppsPlugin.ContentType.LAYOUT,
            	                shortDescription: 'Main',
            	                longDescription: 'PX4 Home Position Message',
            	                filePath: '/PX4_HomePositionMsg.lyt'
        	        		},
                	        PX4_HomePositionMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Home Position Message',
                	            filePath: '/PX4_HomePositionMsg.pug'
                	        }
            	        }
        	        },
        	        PX4_InputRc: {
        	        	shortDescription: 'Input RC',
        	        	longDescription: 'Input RC',
        	        	nodes: {
                	        PX4_InputRcMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Input RC Message',
                	            filePath: '/PX4_InputRcMsg.lyt'
                	        },
                	        PX4_InputRcMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Input RC Message',
                	            filePath: '/PX4_InputRcMsg.pug'
                	        },
                	        PX4_InputRcMsg_RSSI: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'RSSI',
                	            longDescription: 'Input RC - RSSI',
                	            filePath: '/PX4_InputRcMsg_RSSI.pug'
                	        },
                	        PX4_InputRcMsg_Values: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Values',
                	            longDescription: 'Input RC - Values',
                	            filePath: '/PX4_InputRcMsg_Values.pug'
                	        }
        	        	}
        	        },
        	        PX4_LedControl: {
        	        	shortDescription: 'LED Control',
        	        	longDescription: 'LED Control',
        	        	nodes: {
                	        PX4_LedControlMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Led Control Message',
                	            filePath: '/PX4_LedControlMsg.lyt'
                	        },
                	        PX4_LedControlMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Led Control Message',
                	            filePath: '/PX4_LedControlMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_ManualControlSetpoint: {
        	        	shortDescription: 'Manual Control Setpoint',
        	        	longDescription: 'Manual Control Setpoint',
        	        	nodes: {
                	        PX4_ManualControlSetpointMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Manual Control Setpoint Message',
                	            filePath: '/PX4_ManualControlSetpointMsg.lyt'
                	        },
                	        PX4_ManualControlSetpointMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Manual Control Setpoint Message',
                	            filePath: '/PX4_ManualControlSetpointMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_McAttCtrlStatus: {
        	        	shortDescription: 'Multicopter Attitude Control Status',
        	        	longDescription: 'Multicopter Attitude Control Status',
        	        	nodes: {
                	        PX4_McAttCtrlStatusMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Multicoptor Attitude Control Status Message',
                	            filePath: '/PX4_McAttCtrlStatusMsg.lyt'
                	        },
                	        PX4_McAttCtrlStatusMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Multicoptor Attitude Control Status Message',
                	            filePath: '/PX4_McAttCtrlStatusMsg.pug'
                	        },
                	        PX4_McAttCtrlStatusMsg_RateInteg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Rates',
                	            longDescription: 'Multicoptor Attitude Control, integrated rates.',
                	            filePath: '/PX4_McAttCtrlStatusMsg_RateInteg.pug'
                	        }
        	        	}
        	        },
        	        PX4_MissionMessage: {
        	        	shortDescription: 'Mission Message',
        	        	longDescription: 'Mission Message',
        	        	nodes: {
                	        PX4_MissionMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Mission Message',
                	            filePath: '/PX4_MissionMsg.lyt'
                	        },
                	        PX4_MissionMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Mission Message',
                	            filePath: '/PX4_MissionMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_MissionResult: {
        	        	shortDescription: 'Mission Result',
        	        	longDescription: 'Mission Result',
        	        	nodes: {
                	        PX4_MissionResultMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Mission Result Message',
                	            filePath: '/PX4_MissionResultMsg.lyt'
                	        },
                	        PX4_MissionResultMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Mission Result Message',
                	            filePath: '/PX4_MissionResultMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_MultirotorMotorLimits: {
        	        	shortDescription: 'Multirotor Motor Limits',
        	        	longDescription: 'Multirotor Motor Limits',
        	        	nodes: {
                	        PX4_MultirotorMotorLimitsMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Multirotor Motor Limits Message',
                	            filePath: '/PX4_MultirotorMotorLimitsMsg.lyt'
                	        },
                	        PX4_MultirotorMotorLimitsMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Multirotor Motor Limits Message',
                	            filePath: '/PX4_MultirotorMotorLimitsMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_OpticalFlow: {
        	        	shortDescription: 'Optical Flow',
        	        	longDescription: 'Optical Flow',
        	        	nodes: {
                	        PX4_OpticalFlowMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Optical Flow Message',
                	            filePath: '/PX4_OpticalFlowMsg.lyt'
                	        },
                	        PX4_OpticalFlowMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Optical Flow Message',
                	            filePath: '/PX4_OpticalFlowMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_PositionSetpointTriplet: {
        	        	shortDescription: 'Position Setpoint Triplet',
        	        	longDescription: 'Position Setpoint Triplet',
        	        	nodes: {
                	        PX4_PositionSetpointTripletMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Position Setpoint Triplet Message',
                	            filePath: '/PX4_PositionSetpointTripletMsg.lyt'
                	        },
                	        PX4_PositionSetpointTripletCurrentMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Current',
                	            longDescription: 'PX4 Position Setpoint Triplet Current Message',
                	            filePath: '/PX4_PositionSetpointTripletCurrentMsg.pug'
                	        },
                	        PX4_PositionSetpointTripletNextMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Next',
                	            longDescription: 'PX4 Position Setpoint Triplet Next Message',
                	            filePath: '/PX4_PositionSetpointTripletNextMsg.pug'
                	        },
                	        PX4_PositionSetpointTripletPreviousMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Previous',
                	            longDescription: 'PX4 Position Setpoint Triplet Previous Message',
                	            filePath: '/PX4_PositionSetpointTripletPreviousMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_RcChannels: {
        	        	shortDescription: 'RC Channels',
        	        	longDescription: 'RC Channels',
        	        	nodes: {
                	        PX4_RcChannelsMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 RC Channels Message',
                	            filePath: '/PX4_RcChannelsMsg.lyt'
                	        },
                	        PX4_RcChannelsMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 RC Channels Message',
                	            filePath: '/PX4_RcChannelsMsg.pug'
                	        },
                	        PX4_RcChannelsMsg_Channels: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Channels',
                	            longDescription: 'Channels',
                	            filePath: '/PX4_RcChannelsMsg_Channels.pug'
                	        },
                	        PX4_RcChannelsMsg_Function: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Function',
                	            longDescription: 'Function',
                	            filePath: '/PX4_RcChannelsMsg_Function.pug'
                	        },
                	        PX4_RcChannelsMsg_RSSI: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'RSSI',
                	            longDescription: 'RSSI',
                	            filePath: '/PX4_RcChannelsMsg_RSSI.pug'
                	        },
                	        PX4_RcChannelsMsg_SignalLost: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'SignalLost',
                	            longDescription: 'SignalLost',
                	            filePath: '/PX4_RcChannelsMsg_SignalLost.pug'
                	        }
        	        	}
        	        },
        	        PX4_Safety: {
        	        	shortDescription: 'Safety',
        	        	longDescription: 'Safety',
        	        	nodes: {
                	        PX4_SafetyMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Safety Message',
                	            filePath: '/PX4_SafetyMsg.lyt'
                	        },
                	        PX4_SafetyMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Safety Message',
                	            filePath: '/PX4_SafetyMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_SatelliteInfo: {
        	        	shortDescription: 'Safellite Info',
        	        	longDescription: 'Safellite Info',
        	        	nodes: {
                	        PX4_SatelliteInfoMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Satellite Info Message',
                	            filePath: '/PX4_SatelliteInfoMsg.lyt'
                	        },
                	        PX4_SatelliteInfoMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Satellite Info Message',
                	            filePath: '/PX4_SatelliteInfoMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_SensorAccel: {
        	        	shortDescription: 'Sensor Accel',
        	        	longDescription: 'Sensor Accel',
        	        	nodes: {
                	        PX4_SensorAccelMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Sensor Accelerometer Message',
                	            filePath: '/PX4_SensorAccelMsg.lyt'
                	        },
                	        PX4_SensorAccelMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Sensor Accelerometer Message',
                	            filePath: '/PX4_SensorAccelMsg.pug'
                	        },
                	        PX4_SensorAccelMsg_Accel: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Accel',
                	            longDescription: 'Sensor Accel',
                	            filePath: '/PX4_SensorAccelMsg_Accel.pug'
                	        },
                	        PX4_SensorAccelMsg_Integral: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Integral',
                	            longDescription: 'Sensor Accel Integral',
                	            filePath: '/PX4_SensorAccelMsg_Integral.pug'
                	        },
                	        PX4_SensorAccelMsg_Raw: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Raw',
                	            longDescription: 'Sensor Accel Raw',
                	            filePath: '/PX4_SensorAccelMsg_Raw.pug'
                	        },
                	        PX4_SensorAccelMsg_Temperature: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Temperature',
                	            longDescription: 'Sensor Accel Temperature',
                	            filePath: '/PX4_SensorAccelMsg_Temperature.pug'
                	        },
                	        PX4_SensorAccelMsg_TemperatureRaw: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Temperature Raw',
                	            longDescription: 'Sensor Accel Temperature Raw',
                	            filePath: '/PX4_SensorAccelMsg_TemperatureRaw.pug'
                	        }
        	        	}
        	        },
        	        PX4_SensorBaro: {
        	        	shortDescription: 'Sensor Baro',
        	        	longDescription: 'Sensor Baro',
        	        	nodes: {
                	        PX4_SensorBaroMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Sensor Barometer Message',
                	            filePath: '/PX4_SensorBaroMsg.lyt'
                	        },
                	        PX4_SensorBaroMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Sensor Barometer Message',
                	            filePath: '/PX4_SensorBaroMsg.pug'
                	        },
                	        PX4_SensorBaroMsg_Altitude: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Altitude',
                	            longDescription: 'Sensor Baro Altitude',
                	            filePath: '/PX4_SensorBaroMsg_Altitude.pug'
                	        },
                	        PX4_SensorBaroMsg_Pressure: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Pressure',
                	            longDescription: 'Sensor Baro Pressure',
                	            filePath: '/PX4_SensorBaroMsg_Pressure.pug'
                	        },
                	        PX4_SensorBaroMsg_Temperature: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Temperature',
                	            longDescription: 'Sensor Baro Temperature',
                	            filePath: '/PX4_SensorBaroMsg_Temperature.pug'
                	        }
        	        	}
        	        },
        	        PX4_SensorCombined: {
        	        	shortDescription: 'Sensor Combined',
        	        	longDescription: 'Sensor Combined',
        	        	nodes: {
                	        PX4_SensorCombinedMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg.lyt'
                	        },
                	        PX4_SensorCombinedMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg.pug'
                	        },
                	        PX4_SensorCombinedMsg_Acc: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Accelerometer',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg_Acc.pug'
                	        },
                	        PX4_SensorCombinedMsg_BaroAlt: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Baro Alt',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg_BaroAlt.pug'
                	        },
                	        PX4_SensorCombinedMsg_BaroTemp: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Baro Temp',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg_BaroTemp.pug'
                	        },
                	        PX4_SensorCombinedMsg_GyroRad: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Gyro',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg_GyroRad.pug'
                	        },
                	        PX4_SensorCombinedMsg_Mag: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Mag',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg_Mag.pug'
                	        },
                	        PX4_SensorCombinedMsg_RelTimeInvalid: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Rel Time Invalid',
                	            longDescription: 'PX4 Sensor Combined Message',
                	            filePath: '/PX4_SensorCombinedMsg_RelTimeInvalid.pug'
                	        }
        	        	}
        	        },
        	        PX4_SensorCorrection: {
        	        	shortDescription: 'Sensor Correction',
        	        	longDescription: 'Sensor Correction',
        	        	nodes: {
                	        PX4_SensorCorrectionMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Sensor Correction Message',
                	            filePath: '/PX4_SensorCorrectionMsg.lyt'
                	        },
                	        PX4_SensorCorrectionMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Sensor Correction Message',
                	            filePath: '/PX4_SensorCorrectionMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_SensorGyro: {
        	        	shortDescription: 'Sensor Gyro',
        	        	longDescription: 'Sensor Gyro',
        	        	nodes: {
                	        PX4_SensorGyroMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Sensor Gyroscope Message',
                	            filePath: '/PX4_SensorGyroMsg.lyt'
                	        },
                	        PX4_SensorGyroMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Sensor Gyroscope Message',
                	            filePath: '/PX4_SensorGyroMsg.pug'
                	        },
                	        PX4_SensorGyroMsg_Gyro: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Gyro',
                	            longDescription: 'Sensor Gyro',
                	            filePath: '/PX4_SensorGyroMsg_Gyro.pug'
                	        },
                	        PX4_SensorGyroMsg_Integral: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Integral',
                	            longDescription: 'Sensor Gyro Integral',
                	            filePath: '/PX4_SensorGyroMsg_Integral.pug'
                	        },
                	        PX4_SensorGyroMsg_Raw: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Raw',
                	            longDescription: 'Sensor Gyro Raw',
                	            filePath: '/PX4_SensorGyroMsg_Raw.pug'
                	        },
                	        PX4_SensorGyroMsg_Temperature: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Temperature',
                	            longDescription: 'Sensor Gyro Temperature',
                	            filePath: '/PX4_SensorGyroMsg_Temperature.pug'
                	        },
                	        PX4_SensorGyroMsg_TemperatureRaw: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Temperature Raw',
                	            longDescription: 'Sensor Gyro Temperature Raw',
                	            filePath: '/PX4_SensorGyroMsg_TemperatureRaw.pug'
                	        }
        	        	}
        	        },
        	        PX4_SensorMag: {
        	        	shortDescription: 'Sensor Mag',
        	        	longDescription: 'Sensor Mag',
        	        	nodes: {
                	        PX4_SensorMagMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Sensor Magnetometer Message',
                	            filePath: '/PX4_SensorMagMsg.lyt'
                	        },
                	        PX4_SensorMagMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Sensor Magnetometer Message',
                	            filePath: '/PX4_SensorMagMsg.pug'
                	        },
                	        PX4_SensorMagMsg_Mag: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Mag',
                	            longDescription: 'Sensor Mag',
                	            filePath: '/PX4_SensorMagMsg_Mag.pug'
                	        },
                	        PX4_SensorMagMsg_Raw: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Raw',
                	            longDescription: 'Sensor Mag Raw',
                	            filePath: '/PX4_SensorMagMsg_Raw.pug'
                	        },
                	        PX4_SensorMagMsg_Temperature: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Temperature',
                	            longDescription: 'Sensor Mag Temperature',
                	            filePath: '/PX4_SensorMagMsg_Temperature.pug'
                	        }
        	        	}
        	        },
        	        PX4_SubsystemInfo: {
        	        	shortDescription: 'Subsystem Info',
        	        	longDescription: 'Subsystem Info',
        	        	nodes: {
                	        PX4_SubsystemInfoMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Subsystem Info Message',
                	            filePath: '/PX4_SubsystemInfoMsg.lyt'
                	        },
                	        PX4_SubsystemInfoMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Subsystem Info Message',
                	            filePath: '/PX4_SubsystemInfoMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_TelemetryStatus: {
        	        	shortDescription: 'Telemetry Status',
        	        	longDescription: 'Telemetry Status',
        	        	nodes: {
                	        PX4_TelemetryStatusMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Telemetry Status Message',
                	            filePath: '/PX4_TelemetryStatusMsg.lyt'
                	        },
                	        PX4_TelemetryStatusMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Telemetry Status Message',
                	            filePath: '/PX4_TelemetryStatusMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleAttitude: {
        	        	shortDescription: 'Vehicle Attitude',
        	        	longDescription: 'Vehicle Attitude',
        	        	nodes: {
                	        PX4_VehicleAttitudeMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Attitude Message',
                	            filePath: '/PX4_VehicleAttitudeMsg.lyt'
                	        },
                	        PX4_VehicleAttitudeMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Attitude Message',
                	            filePath: '/PX4_VehicleAttitudeMsg.pug'
                	        },
                	        PX4_VehicleAttitudeMsg_Speed: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Speed',
                	            longDescription: 'Vehicle Attitude Speed',
                	            filePath: '/PX4_VehicleAttitudeMsg_Speed.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleAttitudeSetpoint: {
        	        	shortDescription: 'Vehicle Attitude Setpoint',
        	        	longDescription: 'Vehicle Attitude Setpoint',
        	        	nodes: {
                	        PX4_VehicleAttitudeSetpointMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Attitude Setpoint Message',
                	            filePath: '/PX4_VehicleAttitudeSetpointMsg.lyt'
                	        },
                	        PX4_VehicleAttitudeSetpointMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Attitude Setpoint Message',
                	            filePath: '/PX4_VehicleAttitudeSetpointMsg.pug'
                	        },
                	        PX4_VehicleAttitudeSetpointMsg_Body: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Body',
                	            longDescription: 'Vehicle Attitude Setpoint Body',
                	            filePath: '/PX4_VehicleAttitudeSetpointMsg_Body.pug'
                	        },
                	        PX4_VehicleAttitudeSetpointMsg_Thrust: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Thrust',
                	            longDescription: 'Vehicle Attitude Setpoint Thrust',
                	            filePath: '/PX4_VehicleAttitudeSetpointMsg_Thrust.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleCommand: {
        	        	shortDescription: 'Vehicle Command',
        	        	longDescription: 'Vehicle Command',
        	        	nodes: {
                	        PX4_VehicleCommandMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Command Message',
                	            filePath: '/PX4_VehicleCommandMsg.lyt'
                	        },
                	        PX4_VehicleCommandMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Command Message',
                	            filePath: '/PX4_VehicleCommandMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleControlMode: {
        	        	shortDescription: 'Vehicle Control Mode',
        	        	longDescription: 'Vehicle Control Mode',
        	        	nodes: {
                	        PX4_VehicleControlModeMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Control Mode Message',
                	            filePath: '/PX4_VehicleControlModeMsg.lyt'
                	        },
                	        PX4_VehicleControlModeMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Control Mode Message',
                	            filePath: '/PX4_VehicleControlModeMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleGlobalPosition: {
        	        	shortDescription: 'Vehicle Global Position',
        	        	longDescription: 'Vehicle Global Position',
        	        	nodes: {
                	        PX4_VehicleGlobalPositionMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Global Position Message',
                	            filePath: '/PX4_VehicleGlobalPositionMsg.lyt'
                	        },
                	        PX4_VehicleGlobalPositionMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Global Position Message',
                	            filePath: '/PX4_VehicleGlobalPositionMsg.pug'
                	        },
                	        PX4_VehicleGlobalPositionMsg_Ep: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Ep',
                	            longDescription: 'Vehicle Global Position Ep',
                	            filePath: '/PX4_VehicleGlobalPositionMsg_Ep.pug'
                	        },
                	        PX4_VehicleGlobalPositionMsg_Ev: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Ev',
                	            longDescription: 'Vehicle Global Position Ev',
                	            filePath: '/PX4_VehicleGlobalPositionMsg_Ev.pug'
                	        },
                	        PX4_VehicleGlobalPositionMsg_Vel: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Vel',
                	            longDescription: 'Vehicle Global Position Vel',
                	            filePath: '/PX4_VehicleGlobalPositionMsg_Vel.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleGlobalVelocitySetpoint: {
        	        	shortDescription: 'Vehicle Global Velocity Setpoint',
        	        	longDescription: 'Vehicle Global Velocity Setpoint',
        	        	nodes: {
                	        PX4_VehicleGlobalVelocitySetpointMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Global Velocity Setpoint Message',
                	            filePath: '/PX4_VehicleGlobalVelocitySetpointMsg.lyt'
                	        },
                	        PX4_VehicleGlobalVelocitySetpointMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Global Velocity Setpoint Message',
                	            filePath: '/PX4_VehicleGlobalVelocitySetpointMsg.pug'
                	        },
                	        PX4_VehicleGlobalVelocitySetpointMsg_V: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Velocity',
                	            longDescription: 'Vehicle Global Velocity Setpoint Velocity',
                	            filePath: '/PX4_VehicleGlobalVelocitySetpointMsg_V.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleGpsPosition: {
        	        	shortDescription: 'Vehicle GPS Position',
        	        	longDescription: 'Vehicle GPS Position',
        	        	nodes: {
                	        PX4_VehicleGpsPositionMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle GPS Position Message',
                	            filePath: '/PX4_VehicleGpsPositionMsg.lyt'
                	        },
                	        PX4_VehicleGpsPositionMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle GPS Position Message',
                	            filePath: '/PX4_VehicleGpsPositionMsg.pug'
                	        },
                	        PX4_VehicleGpsPositionMsg_Ep: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Ep',
                	            longDescription: 'PX4 Vehicle GPS Position Message Ep',
                	            filePath: '/PX4_VehicleGpsPositionMsg_Ep.pug'
                	        },
                	        PX4_VehicleGpsPositionMsg_Ev: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Ev',
                	            longDescription: 'PX4 Vehicle GPS Position Message Ev',
                	            filePath: '/PX4_VehicleGpsPositionMsg_Ev.pug'
                	        },
                	        PX4_VehicleGpsPositionMsg_SatellitesUsed: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Satellites Used',
                	            longDescription: 'PX4 Vehicle GPS Position Message Satellites Used',
                	            filePath: '/PX4_VehicleGpsPositionMsg_SatellitesUsed.pug'
                	        },
                	        PX4_VehicleGpsPositionMsg_Vel: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Velocity',
                	            longDescription: 'PX4 Vehicle GPS Position Message Velocity',
                	            filePath: '/PX4_VehicleGpsPositionMsg_Vel.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleLandDetected: {
        	        	shortDescription: 'Vehicle Land Detected',
        	        	longDescription: 'Vehicle Land Detected',
        	        	nodes: {
                	        PX4_VehicleLandDetectedMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Land Detected Message',
                	            filePath: '/PX4_VehicleLandDetectedMsg.lyt'
                	        },
                	        PX4_VehicleLandDetectedMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Land Detected Message',
                	            filePath: '/PX4_VehicleLandDetectedMsg.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleLocalPosition: {
        	        	shortDescription: 'Vehicle Local Position',
        	        	longDescription: 'Vehicle Local Position',
        	        	nodes: {
                	        PX4_VehicleLocalPositionMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Local Position Message',
                	            filePath: '/PX4_VehicleLocalPositionMsg.lyt'
                	        },
                	        PX4_VehicleLocalPositionMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Local Position Message',
                	            filePath: '/PX4_VehicleLocalPositionMsg.pug'
                	        },
                	        PX4_VehicleLocalPositionMsg_Acc: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Acceleration',
                	            longDescription: 'PX4 Vehicle Local Position Message Acceleration',
                	            filePath: '/PX4_VehicleLocalPositionMsg_Acc.pug'
                	        },
                	        PX4_VehicleLocalPositionMsg_Attitude: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Attitude',
                	            longDescription: 'PX4 Vehicle Local Position Message Attitude',
                	            filePath: '/PX4_VehicleLocalPositionMsg_Attitude.pug'
                	        },
                	        PX4_VehicleLocalPositionMsg_Ep: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Ep',
                	            longDescription: 'PX4 Vehicle Local Position Message Ep',
                	            filePath: '/PX4_VehicleLocalPositionMsg_Ep.pug'
                	        },
                	        PX4_VehicleLocalPositionMsg_Ev: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Ev',
                	            longDescription: 'PX4 Vehicle Local Position Message Ev',
                	            filePath: '/PX4_VehicleLocalPositionMsg_Ev.pug'
                	        },
                	        PX4_VehicleLocalPositionMsg_Pos: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Position',
                	            longDescription: 'PX4 Vehicle Local Position Message Position',
                	            filePath: '/PX4_VehicleLocalPositionMsg_Pos.pug'
                	        },
                	        PX4_VehicleLocalPositionMsg_Vel: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Velocity',
                	            longDescription: 'PX4 Vehicle Local Position Message Velocity',
                	            filePath: '/PX4_VehicleLocalPositionMsg_Vel.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleLocalPositionSetpoint: {
        	        	shortDescription: 'Vehicle Local Position Setpoint',
        	        	longDescription: 'Vehicle Local Position Setpoint',
        	        	nodes: {
                	        PX4_VehicleLocalPositionSetpointMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Local Position Setpoint Message',
                	            filePath: '/PX4_VehicleLocalPositionSetpointMsg.lyt'
                	        },
                	        PX4_VehicleLocalPositionSetpointMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Local Position Setpoint Message',
                	            filePath: '/PX4_VehicleLocalPositionSetpointMsg.pug'
                	        },
                	        PX4_VehicleLocalPositionSetpointMsg_Pos: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Position',
                	            longDescription: 'Vehicle Local Position Setpoint Position',
                	            filePath: '/PX4_VehicleLocalPositionSetpointMsg_Pos.pug'
                	        },
                	        PX4_VehicleLocalPositionSetpointMsg_Vel: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Velocity',
                	            longDescription: 'Vehicle Local Position Setpoint Velocity',
                	            filePath: '/PX4_VehicleLocalPositionSetpointMsg_Vel.pug'
                	        },
                	        PX4_VehicleLocalPositionSetpointMsg_Acc: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Acceleration',
                	            longDescription: 'Vehicle Local Position Setpoint Acceleration',
                	            filePath: '/PX4_VehicleLocalPositionSetpointMsg_Acc.pug'
                	        },
        	        	}
        	        },
        	        PX4_VehicleRatesSetpoint: {
        	        	shortDescription: 'Vehicle Rates Setpoint',
        	        	longDescription: 'Vehicle Rates Setpoint',
        	        	nodes: {
                	        PX4_VehicleRatesSetpointMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Rates Setpoint Message',
                	            filePath: '/PX4_VehicleRatesSetpointMsg.lyt'
                	        },
                	        PX4_VehicleRatesSetpointMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Rates Setpoint Message',
                	            filePath: '/PX4_VehicleRatesSetpointMsg.pug'
                	        },
                	        PX4_VehicleRatesSetpointMsg_Attitude: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Attitude',
                	            longDescription: 'Vehicle Rates Setpoint Attitude',
                	            filePath: '/PX4_VehicleRatesSetpointMsg_Attitude.pug'
                	        },
                	        PX4_VehicleRatesSetpointMsg_Thrust: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Thrust',
                	            longDescription: 'Vehicle Rates Setpoint Thrust',
                	            filePath: '/PX4_VehicleRatesSetpointMsg_Thrust.pug'
                	        }
        	        	}
        	        },
        	        PX4_VehicleStatus: {
        	        	shortDescription: 'Vehicle Status',
        	        	longDescription: 'Vehicle Status',
        	        	nodes: {
                	        PX4_VehicleStatusMsgLyt: {
                	            type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                	            shortDescription: 'Main',
                	            longDescription: 'PX4 Vehicle Status Message',
                	            filePath: '/PX4_VehicleStatusMsg.lyt'
                	        },
                	        PX4_VehicleStatusMsg: {
                	            type: CdrFlightAppsPlugin.ContentType.PANEL,
                	            shortDescription: 'Message',
                	            longDescription: 'PX4 Vehicle Status Message',
                	            filePath: '/PX4_VehicleStatusMsg.pug'
                	        }
        	        	}
        	        }
        	    }
            }
        };
        
      	this.addContent(content);
    };
};
