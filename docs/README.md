## :octocat: Description :octocat:
Repo where there's the source to create the repo(s)
![createstructure/core-createstructure](https://opengraph.githubassets.com/e99289000e1828d3876752cd2ab7c13d1a5a87d45d8e788629ab13b0c6e1616e/createstructure/core-createstructure)
## :octocat: Directory structure :octocat:

```
../repo
├── Dockerfile
├── bin # PHP source code
│   ├── core.cpp
│   ├── global-libraries
│   │   ├── bin # PHP source code
│   │   │   ├── createstructure.hpp
│   │   │   ├── createstructure_debian.hpp
│   │   │   ├── createstructure_emoji.hpp
│   │   │   ├── createstructure_help.hpp
│   │   │   ├── createstructure_inputs.hpp
│   │   │   ├── createstructure_login.hpp
│   │   │   ├── createstructure_memory.hpp
│   │   │   ├── createstructure_positive.hpp
│   │   │   ├── createstructure_setting.hpp
│   │   │   ├── createstructure_split.hpp
│   │   │   ├── cryptation.hpp
│   │   │   ├── json.hpp
│   │   │   ├── rest.hpp
│   │   │   └── sleep.hpp
│   │   ├── docs
│   │   │   ├── CHANGELOG.md
│   │   │   ├── LICENSE
│   │   │   ├── README.md
│   │   │   └── dependeces.svg
│   │   └── workflows
│   │       ├── push.yml
│   │       └── release.yml
│   └── local-libraries
│       ├── getTemplate.hpp
│       ├── getUploadURL.hpp
│       ├── inputCheck.hpp
│       ├── priority.hpp
│       ├── repo.hpp
│       ├── repoInfoCheck.hpp
│       └── workload.hpp
└── docs
    ├── CHANGELOG.md
    ├── LICENSE
    └── README.md

7 directories, 32 files
```
## :octocat: Changelog :octocat:
Repo where there is the docker source to be run by the manager-createstructure

- [:octocat: Changelog :octocat:](#changelog)
  - [[09.01.11] - 2021-09-10](#090111---2021-09-10)
    - [Changed](#changed)
  - [[09.01.10] - 2021-07-18](#090110---2021-07-18)
    - [Changed](#changed-1)
  - [[09.01.09] - 2021-07-07](#090109---2021-07-07)
    - [Changed](#changed-2)
  - [[09.01.08] - 2021-07-04](#090108---2021-07-04)
    - [Added](#added)
    - [Changed](#changed-3)
  - [[09.01.07] - 2021-06-25](#090107---2021-06-25)
    - [Changed](#changed-4)
  - [[09.01.06] - 2021-06-23](#090106---2021-06-23)
    - [Changed](#changed-5)
  - [[09.01.05] - 2021-06-22](#090105---2021-06-22)
    - [Added](#added-1)
  - [[09.01.04] - 2021-06-22](#090104---2021-06-22)
    - [Added](#added-2)
    - [Changed](#changed-6)
  - [[09.01.03] - 2021-06-19](#090103---2021-06-19)
    - [Added](#added-3)
  - [[09.01.02] - 2021-06-19](#090102---2021-06-19)
    - [Changed](#changed-7)
  - [[09.01.01] - 2021-06-19](#090101---2021-06-19)
    - [Added](#added-4)

### [09.01.11] - 2021-09-10
#### Changed
- Fixed an [issue](https://github.com/createstructure/libraries-createstructure/issues/2)

### [09.01.10] - 2021-07-18
#### Changed
- Updated librerias
- Optimized imports
- Formatted source

### [09.01.09] - 2021-07-07
#### Changed
- Fixed upload

### [09.01.08] - 2021-07-04
#### Added
- Private option
- Team option
#### Changed
- Optimized name management for repo (now uses prefix)

### [09.01.07] - 2021-06-25
#### Changed
- Changed the librerias management

### [09.01.06] - 2021-06-23
#### Changed
- Fixed a bug on git push

### [09.01.05] - 2021-06-22
#### Added
- Now the Docker container is also on [docker-hub](https://hub.docker.com/repository/docker/createstructure/core-createstructure)
 
### [09.01.04] - 2021-06-22
#### Added
- Now the Docker container is supported in multiple platforms
#### Changed
- Docker package now in [https://github.com/createstructure/core-createstructure/pkgs/container/core-createstructure](https://github.com/createstructure/core-createstructure/pkgs/container/core-createstructure)

### [09.01.03] - 2021-06-19
#### Added
- Enviroments in the GitHub actions

### [09.01.02] - 2021-06-19
#### Changed
- Docker container publishing mechanisms

### [09.01.01] - 2021-06-19
#### Added
- Initial version for this repo
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
- On the terminal paste `sudo apt update; sudo apt install apt-transport-https ca-certificates curl software-properties-common -y; curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -; sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable"; sudo apt install docker-ce -y; sudo usermod -aG docker runner; su - runner; git clone --recurse-submodules https://github.com/createstructure/core-createstructure; cd core-createstructure` and insert password

### 4. Build Docker
To build the Docker, into terminal and inside "core-createstructure" folder type `docker build -t core .`

### 5. Run Docker
To run the builded Docker type : docker run core

---
Made w/ :heart: by Castellani Davide

If you want to contribute you can start with:
- [Discussion](https://github.com/createstructure/core-createstructure/discussions)
- [Issue](https://github.com/createstructure/core-createstructure/issues/new)
