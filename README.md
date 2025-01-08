# Gravigun
Game made for jam - https://silenvs.itch.io/gravigun


# Build with CMake
`mkdir _build ; cmake -DCMAKE_BUILD_TYPE=Debug -B _build ; cmake --build _build`

# Web build with Emscriten
Make sure that emscriten is installed. Create directory for the build, say `_build_web`.
`emcmake cmake -B _build_web -DPLATFORM=Web`, `cd _build_web; emmake make -j`.
To disable wasm, pass `-DDISABLE_WASM=1` to emcmake.