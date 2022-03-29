Pyliner
==========

.. code-block:: bash

   wget https://github.com/yamcs/yamcs/releases/download/yamcs-5.6.0/yamcs-5.6.0-linux-aarch64.tar.gz
   tar -xzf yamcs-5.6.0-linux-aarch64.tar.gz
   sudo mkdir /opt/yamcs
   sudo tar -xzf yamcs-5.6.0-linux-aarch64.tar.gz -C /opt/yamcs/
   sudo apt update
   sudo apt-get install default-jre
   sudo apt-get install librxtx-java
   git clone https://github.com/WindhoverLabs/airliner.git
   cd airliner
