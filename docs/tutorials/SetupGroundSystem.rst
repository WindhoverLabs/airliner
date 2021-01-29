Setup Ground System
======================

``airliner`` uses YAMCS as its ground system. This guide assumes that the airliner build that is being used is ``tutorial/cfs``, but *any* airliner build may be used.
This guide can be used as a template for any other airliner build.

Build airliner
###############
NOTE:You *must* have **python3.6** or newer. It is also assumed that the user has Ubuntu16,18 or 20 installed.

Install Dependencies:
    |   ``apt-get install libdwarf-dev``
    |   ``apt-get install libelf-dev``
    |   ``apt-get install libsqlite3-dev``
    |   ``apt-get install cmake``
    |   ``apt-get install gcc-multilib``
    |   ``apt-get install g++-multilib``
    |   ``apt-get install maven``
    |   ``apt-get install default-jre``
    |   ``apt-get install openjdk-8-jdk``
    |   ``pip3 install pyyaml``
    |   ``pip3 install ruamel-yaml``
    |   ``pip3 install yamlpath``
    |   ``pip3 install sqlite_utils``
    |   ``pip3 install cerberus``
    |   ``pip3 install six``
    |   ``pip3 install yamlpath``

Go to airliner root directory:
    ``cd airliner``

Build airliner:
    ``make tutorial/cfs``

Generate XTCE File
################
    |   ``cd build/tutorial/cfs/target/``
    |   ``make xtce``


Run Airliner
#############
    |   ``cd airliner/build/tutorial/cfs/target/target/exe``
    |   ``./airliner``


Run YAMCS
##########
    |   ``cd airliner/tools/yamcs-cfs``
    |   ``mvn yamcs:run``

If YAMCS builds and runs successfully, it should output a web address starting with ``http``. This web interface can be
used to interact with airliner. You should be able to receive telemetry and send commands to airliner from the YAMCS webapp.

You can find more documentation about YAMCS `here <https://docs.yamcs.org/>`_.
