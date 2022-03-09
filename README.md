# How to Run SITL Mode
1. ```make obc-sitl```
 This will build the "obc/ppd", "obc/cpd", and "obc/simlink" builds, as well as the Gazebo plugins.
1. ```make workspace-sitl```
 This will build the combined registry, XTCE, and ground displays for the "ppd", "cpd", and "simlink" builds.
 The next steps will require 5 different shells.
 **Shell 1**
1. ```cd build/obc/sitl_commander_workspace```
1. ```bin/yamcsd /opt/yamcs .```
 This should launch YAMCS
 **Shell 2**
1. ```cd build/obc/simlink/host/host```
1. ```./start-gazebo```
 This should start Gazebo
 **Shell 3**
1. ```cd build/obc/simlink/target/target/exe```
1. ```sudo ./simlink```
 This should launch Simlink.  Simlink will immediately connect to the simulation and begin emulating the SED application with bridged simulation data.
 **Shell 4**
1. ```cd build/obc/ppd/sitl/target/target/exe```
1. ```sudo SEDLIB_SHMEM_KEY_FILE=../../../../../simlink/target/target/exe/.sedlib_shmem ./airliner```
 This should launch the PPD instance of Airliner with SEDLIB remapped to the registers managed by Simlink.
 **Shell 5**
1. ```cd build/cbc/ppd/sitl/target/target/exe```
1. ```sudo SEDLIB_SHMEM_KEY_FILE=../../../../../simlink/target/target/exe/.sedlib_shmem ./airliner```
 This should launch the CPD instance of Airliner with SEDLIB remapped to the registers managed by Simlink.

You may now use Commander to interact with the system.
