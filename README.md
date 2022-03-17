# How to Run SITL Mode
1. ```make obc-sitl```  
 This will build the "obc/ppd", "obc/cpd", and "obc/simlink" builds, as well as the Gazebo plugins.
2. ```make workspace-sitl```  
 This will build the combined registry, XTCE, and ground displays for the "ppd", "cpd", and "simlink" builds.
 The next steps will require 5 different shells. Make sure to *always* create a python virtual environment by running ```make python-env``` from the root directory of the repo.  
 **Shell 1**
3. ```cd build/obc/sitl_commander_workspace```
4. ```bin/yamcsd /opt/yamcs .```  
 This should launch YAMCS  
 **Shell 2**  
5. ```cd build/obc/simlink/host/host```
6. ```./start-gazebo```  
 This should start Gazebo  
 **Shell 3**
7. ```cd build/obc/simlink/target/target/exe```
8. ```sudo ./simlink```  
 This should launch Simlink.  Simlink will immediately connect to the simulation and begin emulating the SED application with bridged simulation data.  
 **Shell 4**
9. ```cd build/obc/ppd/sitl/target/target/exe```
10. ```sudo SEDLIB_SHMEM_KEY_FILE=../../../../../simlink/target/target/exe/.sedlib_shmem ./airliner```  
 This should launch the PPD instance of Airliner with SEDLIB remapped to the registers managed by Simlink.  
 **Shell 5**
11. ```cd build/obc/cpd/sitl/target/target/exe```
12. ```sudo SEDLIB_SHMEM_KEY_FILE=../../../../../simlink/target/target/exe/.sedlib_shmem ./airliner```  
 This should launch the CPD instance of Airliner with SEDLIB remapped to the registers managed by Simlink.  

You may now use Commander to interact with the system.
