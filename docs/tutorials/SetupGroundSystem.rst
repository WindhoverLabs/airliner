Setup Ground System - Commander
===============================

We call airliner's ground system *Commander* which uses the YAMCS_ suite of tools. This guide assumes that the airliner build that is being used is ``bebop2/sitl``, but *any* airliner build may be used.
This guide can be used as a template for any other airliner build.

Build airliner
###############
NOTE:You *must* have **python3.6** or newer. It is also assumed that the user has Ubuntu18 or 20 installed.

Install Dependencies
::
  apt-get install libdwarf-dev
  apt-get install libelf-dev
  apt-get install libsqlite3-dev
  apt-get install cmake
  apt-get install gcc-multilib
  apt-get install g++-multilib
  apt-get install maven
  apt-get install default-jre
  apt-get install openjdk-8-jdk
  pip3 install pyyaml
  pip3 install ruamel-yaml
  pip3 install yamlpath
  pip3 install sqlite_utils
  pip3 install cerberus
  pip3 install six
  pip3 install yamlpath

Go to airliner root directory:
    ``cd airliner``

Build airliner:
    ``make bebop2/sitl``

Building the Commander Workspace
################
    |   ``cd build/bebop2/sitl/target/``
    |   ``make commander``

    The commander workspace is the workspace the ground system uses to store telemetry/tele-commanding definitions(XTCE), configuration, Displays, etc.
    More details about what users can do with the commander workspace later.

Run Airliner
#############
    |   ``cd airliner/build/bebop2/sitl/target/target/exe``
    |   ``./airliner``


Install YAMCS
#############
TODO

Run YAMCS
##########
    |   ``cd airliner/build/bebop2/sitl/target``
    |   ``make start-yamcs``

If YAMCS builds and runs successfully, it should output a web address starting with ``http``. This web interface can be
used to interact with airliner. You should be able to receive telemetry and send commands to airliner from the YAMCS webapp.

You can find more documentation about YAMCS `here <https://docs.yamcs.org/>`_.

.. _YAMCS: https://yamcs.org/