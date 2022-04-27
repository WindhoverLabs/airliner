# Requirements to Run Airliner SITL
## Ubuntu 18.04 LTS
Airliner requires [Ubuntu 18.04 LTS](https://releases.ubuntu.com/18.04/ubuntu-18.04.6-desktop-amd64.iso).  During installation do not connect to wifi or network.

After completing installation, sign into the new ubuntu account (with administrator/sudo previleges), and connect to network.  Run the following command on a terminal shell to get the latest updates of packages.

    sudo apt-get update

### Timeshift
On a new linux install, it is a good idea to install timeshift to run backups on a periodic basis.  Timeshift is not a requirement for Airliner, but it is a good idea to protect development enviroments through backups.  Use the commands below to install Timeshift on your new ubuntu install.

    sudo add-apt-repository ppa:teejee2008/ppa
    sudo apt-get update
    sudo apt-get install timeshift -y

Then run timeshift from Applications and setup a backup automation schedule.

### Microsoft Visual Code
This is completely optional.  But this is how to install MS Visual Code in Ubuntu 18.04 installation.

    sudo apt update
    sudo apt install gnupg2 software-properties-common apt-transport-https wget -y
    wget -q https://packages.microsoft.com/keys/microsoft.asc -O- | sudo apt-key add -
    sudo add-apt-repository "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main"
    sudo apt update
    sudo apt install code

## Install Libraries Needed for SITL
The shell file [airliner_reqs_ubuntu18.04.sh](./airliner_reqs_ubuntu18.04.sh) is in the same folder as this `README.md` file.  Download the shell file to a folder like `~/Downloads` folder.  Run the shell command there. It should install all the libraries needed for SITL.

    source ~/Downloads/airliner_reqs_ubuntu18.04.sh

## Setup SSH for github
SSH makes it easy to use git with github.  Use the commands below to generate an ssh key for github.

    mkdir ~/.ssh
    cd ~/.ssh
    ssh-keygen

When asked for a filename, give it a filename without any extensions.  Say the filename given is `wh_github`.  Then `wh_github.pub` is where the public key is stored in the `~/.ssh/` folder.  And `~/.ssh/wh_github` is the private key file, that is to be kept secret.

Use the following command to copy the contents of the public file onto system clipboard.

    cat ~/.ssh/wh_github.pub|xclip -i -selection clipboard

Log into github and go to account settings, and click on the `SSH and GPG keys` navigation tab on the left.  On the right, click on the `New SSH Key` button.  On the form that pops, give the SSH Key a `title` that corresponds to the machine on which the ubuntu 18.04 is installed.  Inside the `key` text area, paste the public key that is stored in your clipboard.  Hit the `Add SSH Key` button to add your ssh key on github.  Now you should be able to access any github repository that you have authenticated access to from your local git repo.

## Git Clone
Create a project directory where you will store your projects.  Say that it is `~/src` folder.

    mkdir ~/src
    cd ~/src
    git clone git@github.com:WindhoverLabs/squeaky-weasel.git

This will clone the `squeaky-weasel` project inside `~/src` folder.  Change directory to this project folder, and checkout the development branch you need to work on.  In this example, the branch is called `genericize_sed_interfaces`.  Modify the following commands with your customized development branch name.

    cd squeaky-weasel
    git checkout genericize_sed_interfaces
    git pull

To update all the submodule in the project run the command below.

    git submodule update --init --recursive

This those pull in all the submodules as they are in the git branch.

## Python Virtual Environment Requirement
To set up a python virtual environment for building, use the following commands.

    cd software/airliner/public
    make python-env
    source venv/bin/activate

The last command activates python virtual environment within the shell.  Now we should be ready to build the code.

## Build Code
In the `software/airliner/public` directory run the following two build commands.

    make obc-sitl
    make workspace-sitl

## Running the Code
Go to the `build/obc` directory and to run the code as follows.

    cd build/obc
    terminator -g term-sitl.cfg

This should fire up several new virtual environment terminals simultaneously.  Some of those will ask from administrator passwords.  Give those passwords in the following sequence.

1. simlink
2. cpd
3. ppd

You should have a gazebo window open up as well.