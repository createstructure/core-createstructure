# corecreatestructure
[![GitHub license](https://img.shields.io/badge/license-GNU-green?style=flat)](https://github.com/CastellaniDavide/corecreatestructure-corecreatestructure/blob/master/LICENSE)
![Author](https://img.shields.io/badge/author-Castellani%20Davide-green?style=flat)
![Language C++, Docker](https://img.shields.io/badge/language-C++,%20Docker-yellowgreen?style=flat)
![sys.platform supported](https://img.shields.io/badge/OS%20platform%20supported-Linux-blue?style=flat)
[![On GitHub](https://img.shields.io/badge/on%20GitHub-True-green?style=flat&logo=github)](https://github.com/createstructure/core-createstructure)

## Table of content
- [corecreatestructure](#corecreatestructure)
  - [Table of content](#table-of-content)
  - [Description](#description)
  - [Directories structure](#directories-structure)
  - [Installation](#installation)
    - [Requirements](#requirements)
    - [Installation](#installation-1)
    - [Execution examples](#execution-examples)
  - [Repo dependencies](#repo-dependencies)

## Description
Repo where there is the docker source to be runned by the managercreatestructure

## Directories structure
```
├── bin
│   ├── core.cpp
│   └── libraries
│       ├── createstructure_changes.hpp
│       ├── createstructure_chooseTemplate.hpp
│       ├── createstructure_cryptation.hpp
│       ├── createstructure_download.hpp
│       ├── createstructure_elaborate.hpp
│       ├── createstructure_emoji.hpp
│       ├── createstructure.hpp
│       ├── createstructure_inputCheck.hpp
│       ├── createstructure_memory.hpp
│       ├── createstructure_positive.hpp
│       ├── createstructure_replace.hpp
│       ├── createstructure_rest.hpp
│       ├── createstructure_split.hpp
│       ├── createstructure_upload.hpp
│       ├── createstructure_work.hpp
│       └── json.hpp
├── Dockerfile
├── docs
│   ├── LICENSE
│   └── README.md
├── .git
│   └── ...
└── .github
    └── workflows
        └── publish.yml
```

## Installation
### Requirements
 - Docker
 
### Installation
 - `docker pull docker.pkg.github.com/createstructure/core-createstructure/core`

### Execution examples
 - `docker run docker.pkg.github.com/createstructure/core-createstructure/core`

## Repo dependencies
 - [nlohmann/json](https://github.com/nlohmann/json)

---
Made w/ :love: by Castellani Davide 
If you have any problem please contact me:
- [Discussion](https://github.com/createstructure/createstructure/discussions/new) and choose "core-createstructure" category
- [Issue](https://github.com/createstructure/core-createstructure/issues/new)
- [help@castellanidavide.it](mailto:help@castellanidavide.it)
