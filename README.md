# Coconuts

Coconuts is a 2D Game Engine library and a Game Editor standalone application.

<p align="center">
  <img src="https://user-images.githubusercontent.com/44137701/115147789-a8864180-a054-11eb-8d61-84248982e6b0.png"/>
</p>

This is mostly a personal project to introduce myself to Game Engines architecture and design.

It includes a Game Engine API generated as a static library (**ccncore**), a GUI Layer static library (**ccneditor**) and a standalone Game Editor Application (**ccneditor_standalone**).

**The "MVP" development is still ongoing**.\
**This means the overall project is still not expected to be fully functional as a Game Engine nor as as Game Level Editor.**


Please include every License files in derivative work. You are also welcome to contribute :)



### Clone
Coconuts depends on third-party libraries that are cloned into ./Coconuts/vendor/ directory.\
To clone everything at once do

```
git clone --recurse-submodules git@github.com:filhoDaMain/Coconuts.git
```

#### Pull latest supported changes from all third-party submodules

```
git submodule update
```

## How to build

#### Dependencies

| Platform      | Dependencies |
| ------------- | ---------- |
| GNU/Linux      | *xorg-dev* package       |
| OSX macOS   | (no external deps)        |


### Build libraries and Standalone Editor application

CMake out-of-source build:
```
cd ./Coconuts/build/
cmake ..
make
```

#### Compile with debug symbols for GDB and LLDB interactive debugging support
```
cd ./Coconuts/build/
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make
```

The toolchain generates both static libraries and a Standalone Editor application by default.\
Coconuts-based applications are required to link with the [generated libraries](#lib).\
Standalone Editor application is a Game Editor [executable](#bin).

## Folder organization
### Coconuts
This is the Coconuts project itself. It cointains both the source code for static libraries and standalone editor application.

#### bin/
An empty directory.\
This is the default location for the generated Standalone Editor application binary.

#### build/
An empty directory.\
You can use it to perform a CMake out-of-source [build](#coconuts-libraries-and-standalone-editor-application).

#### include/
Contains all necessary header files, including third party, that every application requires when linking with Coconuts libraries.

- **coconuts/**\
Coconuts header files and its License. Applications should only directly **#include** these headers.

- **vendor/**\
Thirdy party header files and their Licenses.

#### lib/
An empty directory.\
This is the default location for the generated static libraries. Applications linking with Coconuts should look for its libraries here by default.

#### src/
Contains all source code for Coconuts **ccncore** and **ccneditor** libraries and **Standalone Editor application**.

- **core/**\
Source code for Coconuts Game Engine Library (ccncore static library).

- **editor/**\
Source code for both the editor library (**library/**) and Standalone Editor application (**standalone/**).\
Any application that requires a custom Editor Layer for its GUI implementation can link with the ccneditor library found in ./lib/ .\
Standalone Editor application also links with ccneditor in order to create its GUI.

#### vendor/
Contains all third party source code and their Licenses.


### Demo
A demonstration project, mostly assets.
