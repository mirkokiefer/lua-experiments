#Lua, C, Mongoose and CMake Playground...
This repo only serves to teach me stuff.

##How to build

```
# fetch dependencies:
git submodule init
git submodule update

# build using cmake:
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../_install
cmake --build . --target install

# execute script:
../_install/bin/mytest
```

###XCode notes
- need to add lua code to the build settings (copy files)
- set copy files directory to "resources" and disable "copy only when installing"

