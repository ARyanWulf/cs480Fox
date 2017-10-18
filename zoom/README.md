# PA3
Brianna Blain-Castelli

CS480

## Filenames
Vector shader is named v.txt

Fragment shader is named f.txt

Both are located in the build folder

## Keys to use
### Translation

q = forward translation

w = backwards/reversed translation

e = stop translation

### Rotation

a = pause rotation

s = rotate forward

d = rotate backward

### Arrow Keys
Left = positive translation and rotation

Middle = pause translation and rotation

Right = negative translation and rotation

### Mouse
Left = positive translation and rotation

Middle = pause translation and rotation

Right = negative translation and rotation

## CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place 
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
```

```bash
mkdir build
cd build
cmake ..
make
./Tutorial
```

