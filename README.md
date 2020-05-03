# Coconuts
A didactic project to introduce myself to Game Engines architecture

# Include all submodules
Cocnuts depends on some submodules that should also be cloned into the ./Coconuts/vendor directory.
To perform this step at once while cloning this repo you can do

```
git clone --recurse-submodules -j8 git@github.com:filhoDaMain/Coconuts.git
```

# How to build
## Coconuts lib
```
cd ./Coconuts/build/
cmake ..
make -j
```

## Demo project (links with Coconuts lib generated before)
```
cd ./Demo/build/
cmake ..
make
```
