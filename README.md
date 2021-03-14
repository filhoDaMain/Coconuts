# Coconuts
A didactic project to introduce myself to Game Engines architecture


# Include all submodules
Coconuts depends on third party libraries that are cloned into ./Coconuts/vendor/ directory.
To clone everything at once do

```
git clone --recurse-submodules git@github.com:filhoDaMain/Coconuts.git
```

# How to build
## Coconuts libraries and Standalone Editor application
```
cd ./Coconuts/build/
cmake ..
make
```

# Folder organization
## Coconuts
This is the Coconuts project itself. It cointains both the source code for static libraries and standalone editor application.

### bin/
An empty directory.\
This is the default location for the generated Standalone Editor application binary.

### build/
An empty directory.\
You can use it to perform a CMake out-of-source [build](#coconuts-libraries-and-standalone-editor-application)

### include/
Contains all necessary header files, including third party, that every application requires when linking with Coconuts libraries.

- **coconuts/**\
Coconuts header files and its License. Applications should only directly **#include** these headers.

- **vendor/**\
Thirdy party header files and their Licenses.

### lib/
An empty directory.\
This is the default location for the generated static libraries. Applications linking with Coconuts should look for its libraries here by default.

### src/
Contains all source code for Coconuts **ccncore** and **ccneditor** libraries and **Standalone Editor application**.

- **core/**\
Source code for Coconuts Game Engine Library (ccncore static library).

- **editor/**\
Source code for both the editor library (**library/**) and Standalone Editor application (**standalone/**).\
Any application that requires a custom Editor Layer for its GUI implementation can link with the ccneditor library (found in ./lib/).
Standalone Editor application also links with ccneditor in order to create its GUI.

### vendor/
Contains all third party source code and their Licenses.


## Demo
A demonstration project, mostly assets.
