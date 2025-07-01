This adds [clay](https://github.com/nicbarker/clay) support to [pntr](https://github.com/RobLoach/pntr).

Just include [pntr_clay.h)](src/pntr_clay.h) and [clay.h](https://raw.githubusercontent.com/nicbarker/clay/refs/heads/main/clay.h) and define `CLAY_IMPLEMENTATION` and `PNTR_CLAY_IMPLEMENTATION` (see [example.c](src/example.c).)

### build example

```sh
# configure for native
cmake -B build -DCMAKE_BUILD_TYPE=Release

# build for native
cmake --build build


# configure for web
emcmake cmake -B wbuild -DCMAKE_BUILD_TYPE=Release

# build for web
cmake --build wbuild

```
