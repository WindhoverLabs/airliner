Pyliner
==========

.. code-block:: bash

   # On target
   wget https://github.com/yamcs/yamcs/releases/download/yamcs-5.6.0/yamcs-5.6.0-linux-aarch64.tar.gz
   tar -xzf yamcs-5.6.0-linux-aarch64.tar.gz
   sudo mkdir /opt/yamcs
   sudo tar -xzf yamcs-5.6.0-linux-aarch64.tar.gz -C /opt/yamcs/
   sudo cp -r /opt/yamcs/yamcs-5.6.0/* /opt/yamcs/
   sudo apt update
   sudo apt-get install default-jre
   sudo apt-get install python3-pip
   pip3 install yamcs-client
   # On host(assumes Xilinx tools are installed)
   git clone https://github.com/WindhoverLabs/squeaky-weasel.git
   source /tools/Xilinx/Vitis/2020.2/settings64.sh
   git submodule update --recursive --init
   make platform
   cd software/airliner/public/
   make python-env
   source venv/bin/activate
   make obc
   make workspace-pyliner
   #The pyliner workspace will be located at squeaky-weasel/software/airliner/public/build/obc/pyliner/commander_workspace
