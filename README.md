# C++ WebAssembly OpenGL App template.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

<!-- cpp_wasm_gl_app_template -->
<!-- cpp_wasm_template -->
<!-- gl-wasm-template -->

This project offers a light boilerplate to build cross-platform C++17 / WebAssembly OpenGL app using CMake, GLFW and emscripten.

## Quickstart

#### Dependencies.

For this project you will need to install CMake 3.12.0+ and the [emsdk](https://github.com/emscripten-core/emsdk).
GLFW3 can be retrieved as a submodule.

#### Cloning.

Retrieve the project with its submodules by typing :
```bash
git clone --recurse-submodules -j4 https://github.com/tcoppex/gl-wasm-template
```

If you have already cloned the repo, use :
```bash
git submodule update --init
```

#### Compilation.

Create a build directory :
```bash
mkdir BUILDs && cd BUILDs
```

Compile for desktop : 
```bash
mkdir desktop && cd desktop
cmake ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ..
```

Compile for the web (*using `emcmake` before `cmake`*) :
```bash
mkdir web && cd web
emcmake cmake ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ..
```

#### Running / serving the files.

You will find the desktop application in the `bin/` directory, whereas the web 
app is in `public/`.

WebAssembly requires a proper server to run its application. For this you can either use
 `http-server` (if available on your system) or the custom script provided in `tools/simple_server` :
```bash
cd ../public
../tools/simple_server/main.py
```

<!-- 
## Known Limitations.

#### unimportant warnings
* mime-type warning is due to a server not recognizing the wasm mime-type. Use `tools/simple_server` to get ride of it.
* emscripten_set_main_loop_timing warning is due to a call of `glfwSwapInterval` before the emscripten mainloop is launched. You can disable it using the simple api provided on JS side, once the loop is launched. 
-->

### TODO

* Proper cpp framework / sample / public_api structure.
* Better web-side structure.
* Improved events handling.
* Add arguments to the server script (directory, host, port).
* NPM package utilities (compiler / minifier / automatic server)
* Advanced samples structures.
* Advanced front-end : React sample.
* Advanced back-end : server-side sample.
* native Android support ?

## References

* https://emscripten.org/

## License

This project is released under the *MIT license*.
