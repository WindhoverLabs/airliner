Glossary
====================================

.. glossary::
  
  AshaiKASEI AK8963
    The AshaiKASEI AK8963 flight software application publishes samples from the AK8963 3-axis electronic compass.

  AMC
    TODO: Add definition of the Actuator Motor Control (AMC) Application

  BAT
    TODO: Add definition of the Battery Monitor (BAT) Application
  
  CF
    TODO: Add definition of the File Delivery (CF) Application

  CFSLIB
    TODO: Add definition of the CFS Library (CFSLIB) Application

  CI
    TODO: Add defintion of the Command Ingest (CI) Application

  CS
    TODO: Add defintion of the Checksum Services (CS) Application

  CVT
    TODO: Add defintion of the Current Value Table (CVT) Application

  DS
    TODO: Add defintion of the Data Storage (DS) Application

  EA
    TODO: Add defintion of the External Application (EA)

  FLOW
    TODO: Add defintion of the Optical Flow (FLOW) Application

  FM
    TODO: Add defintion of the File Manager (FM) Application

  GPS
    TODO: Add defintion of the Global Positioning System (GPS) Application

  HK
    TODO: Add defintion of the Housekeeping (HK) Application

  HMC5883
    TODO: Add defintion of the Honeywell HMC5883 (HMC5883) Application

  HS
    TODO: Add defintion of the Health Services (HS) Application

  LC
    TODO: Add defintion of the Limit Checker (LC) Application

  LD
    TODO: Add defintion of the Landing Detector (LD) Application

  LGC
    TODO: Add defintion of the Landing Gear Control (LGC) Application

  MAC
    TODO: Add defintion of the Multicopter Attitude Control (MAC) Application

  MAVLINK
    TODO: Add defintion of the Mavlink Application

  MD
    TODO: Add defintion of the Memory Dwell (MD) Application

  MM
    TODO: Add defintion of the Memory Manager (MM) Application

  MPC
    TODO: Add defintion of the Multicopter Position Control (MPC) Application

  MPU6050
    TODO: Add defintion of the Invensense MPU6050 (MPU6050) Application 
    
  MPU9250
    TODO: Add definition of the Invensense MPU9250 (MPU9250) Application
  
  MS5607
    TODO: Add defintion of the TE Connectivity MEAS MS5607 (MS5607) Application

  MS5611 
    TODO: Add defintion of the TE Connectivity MEAS MS5611 (MS5611) Application 
  
  NAV
    The Navigator Application (NAV) is responsible for safely navigating or moving the vehicle from one location to another.  
  
  PBLIB
    TODO: Add defintion of the Proto Buffer Library (PBLIB) Application
  
  PE
    The Position Estimator (PE) flight software application sends position data from the remote vehicle to the ground.
    
  PRMLIB
    TODO: Add definition of the Parameter Library (PRMLIB) Application
    
  PX4BR
    TODO: Add definition of the PX4 Bridge (PX4BR) Application
    
  PX4LIB
    TODO: Add definition of the PX4 Library (PX4LIB) Application
    
  QAE
    TODO: Add definition of Q Attitude Estimator (QAE) Application
    
  RCIN
    TODO: Add definition of Radio Control Input (RCIN) Application
    
  RGBLED
    TODO: Add definition of RGB LED Control (RGBLED) Application

  SBN
    TODO: Add definition of Software Bus Network (SBN) Application
    
  SC
    TODO: Add definition of Stored Command (SC) Application

  SCH
    TODO: Add definition of Scheduler (SCH) Application
    
  SENS
    TODO: Add definition of Sensor (SENS) Application
    
  SG33BL
    TODO: Add definition of Hitec SG33BL Actuator (SG33BL) Application
    
  SIM
    TODO: Add definition of Simulation (SIM) Application
    
  SONAR
    TODO: Add definition of Sonar (SONAR) Application

  TO
    The Telemetry Output (TO) flight software application sends telemetry from the remote vehicle to the ground.
    
  ULR
    TODO: Add definition of uLanding Radar (ULR) Application
    
  VC
    TODO: Add definition of Video Control (VC) Application

  VM
    TODO: Add definition of Video Manager (VM) Application
    
  CCSDS
    Consultative Committee for Space Data Systems (CCSDS) develops and documents communications and data systems standards for spaceflight. `<https://public.ccsds.org>`_ 
     
  dump table
    CFE Dump Tables are created at run time, not build time.  They do not contain configuration.  They 
    cannot be loaded or reloaded.  All values contained within a CFE dump table are cleared at load time.  
    Dump tables are updated by flight software at run time, but may be written to the file system by 
    ground command for transfer down to the ground.  Typically, dump tables contain run time metrics that 
    are too big to transmit to the ground in real time, or they contain diagnostic data that is not usually
    interesting enough to warrant consumption of the downlink telemetry bandwidth. 
