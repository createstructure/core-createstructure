## :octocat: Rebuild the core-createstructure locally :octocat:
Install the REST API locally permits you to do some debug and try new configurations.

> ATTENTION
>
> Some features, like subscription with GitHub marketplace, can't be done with the local configuration.
>
> To do these actions you need to run manually the functions, in the previous example `CreateUpdateRemoveClient`

### Steps:
- [:octocat: Rebuild the core-createstructure locally :octocat:](#octocat-rebuild-the-core-createstructure-locally-octocat)
  - [Steps:](#steps)
  - [1. Install VirtualBox on the PC](#1-install-virtualbox-on-the-pc)
  - [2. Install Ubuntu](#2-install-ubuntu)
  - [3. Install dependencies](#3-install-dependencies)
  - [4. Build Docker](#4-build-docker)
  - [5. Run Docker](#5-run-docker)

### 1. Install VirtualBox on the PC
For installation we suggest VirtualBox, a tool that allows you to create one or more virtual machines :computer:.
If any of these crashes, in any case, your PC will not lose data, at most you will have to restart it :smile:.

To install VirtualBox on your PC you need to:
- Get in into the UEFI
- Enable the function which name is like "Virtualization" (for each UEFI this step is different but similar)
- Save the configuration and restart the PC
- Go to the [VirtualBox website](https://www.virtualbox.org/)
- Press "Download"
- Run the downloaded file
- Follow the installation steps

### 2. Install Ubuntu
As the OS we suggest to use Ubuntu, because it is lightweight (for RAM and CPU) and it's free.

To install Ubuntu on VirtualBox you need to:
- Download the last LTS version of Ubuntu by the following link: [https://ubuntu.com/download/desktop](https://ubuntu.com/download/desktop)
> Now you can continue with the other steps during the download
- Open VirtualBox
- Press "New"
- Compile the form
    - As name put "rest-createstructure"
    - As Type: "Linux"
    - As version: "Ubuntu (64-bit)" or "Ubuntu (32-bit)"
    - Press "Next >"
- Set the RAM to use for the VirtualMachine, at most half of local RAM and press "Next >"
- Leave "Create a virtual hard disk now" and press "Create"
- Leave "VDI ..." and press "Next >"
- Leave "Dynamically allocated" and press "Next >"
- Change the hard disk memory, we suggest 16GB and press "Create"
> Make sure that Ubuntu download is finished before to continue
- On the VirtualBox console, selecting the created VM, press "Start"
- Select as start-up disk Ubuntu, already downloaded
    - Press the folder icon
    - Press "Add", in the top menu
    - Select the Ubuntu iso, the file will have a structure like "ubuntu-version-other_info.iso"
    - Press "Choose" and "Start"
- Follow the install steps (the installation needs some minutes)

### 3. Install dependencies
Now you have to install lamp (Linux Apache MySQL (DB) PHP) on the VM.
> We suggest you to open this guide on the virtual machine, so you can copy and paste easlier the following commands. 

To install Dependes on the Virtual Machine you need to:
- On the VM (Virtual Machine) open the terminal (`Ctrl + Alt + T`)
- On the terminal paste `sudo apt update; sudo apt install apt-transport-https ca-certificates curl software-properties-common -y; curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -; sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable"; sudo apt install docker-ce -y; sudo usermod -aG docker ${USER}; su - ${USER}; git clone --recurse-submodules https://github.com/createstructure/core-createstructure; cd core-createstructure` and insert password

### 4. Build Docker
To build the Docker, into terminal and inside "core-createstructure" folder type `docker build -t core .`

### 5. Run Docker
To run the builded Docker type : docker run core