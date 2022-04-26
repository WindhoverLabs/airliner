#!/bin/bash
sudo apt-get install -y libgazebo9-dev
sudo apt-get install -y libboost-all-dev
sudo apt-get install -y python3-opencv
sudo apt-get install -y libopencv-dev
sudo apt-get install -y libgstreamer-plugins-base1.0-dev
sudo apt-get install -y default-jre
sudo apt-get install -y openjdk-8-jdk
#sudo apt-get install -y python>=3.6
sudo apt-get install -y gcc-multilib
sudo apt-get install -y g++-multilib
sudo apt-get install -y cmake
sudo apt-get install -y libdwarf-dev
sudo apt-get install -y libelf-dev
sudo apt-get install -y libsqlite3-dev
sudo apt-get install -y libeigen3-dev
sudo apt-get install -y librxtx-java
sudo apt-get install -y gazebo9
sudo apt-get install -y git
sudo apt-get install -y python3-pip
sudo pip3 install virtualenv
sudo apt install terminator -y

cd /opt/
sudo wget https://github.com/yamcs/yamcs/releases/download/yamcs-5.6.2/yamcs-5.6.2-linux-x86_64.tar.gz
sudo tar xvfz yamcs-5.6.2-linux-x86_64.tar.gz
sudo mv yamcs-5.6.2 yamcs
sudo rm yamcs-5.6.2-linux-x86_64.tar.gz
