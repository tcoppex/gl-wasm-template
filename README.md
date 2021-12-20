## C++17 / WebAssembly OpenGL App template.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


This project offers a light boilerplate to build cross-platform C++17 / WebAssembly OpenGL app using CMake, emscripten, and GLFW.

### Building.

We will be using the command-line on Unix and [Git Bash](https://git-for-windows.github.io/) on Windows.

<details>
  <summary><strong>Quick start & run !</strong></summary>

```bash
# [Optional] Retrieve system build dependencies on Unix via synaptic.
# sudo apt install git build-essential cmake ninja-build python3

# Clone the project with its submodule.
git clone --recurse-submodules -j4 https://github.com/tcoppex/gl-wasm-template
cd gl-wasm-template

# Create a build directory.
mkdir BUILD && cd BUILD

# Compile for the web.
emcmake cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Serve the WebApp.
python3 ../tools/simple_server/main.py --serve ../bin/public
```

</details>

#### Requirements.

The [Emscripten SDK](https://github.com/emscripten-core/emsdk) should be installed and setup on your system. Others dependencies
can be either retrieved via the package manager or as submodules.

The following table summarizes the project dependencies :

| Dependency            | Version | Description                | Shipped               | Required           | 
|-----------------------|---------|----------------------------|-----------------------|--------------------|
| [Emscripten SDK](https://github.com/emscripten-core/emsdk) | 3.0.0   | *WebAssembly toolchains.*  | :x:                   | :heavy_check_mark: |
| [CMake](https://github.com/Kitware/CMake)          | 3.10.2  | *Build tool.*              | :x:                   | :heavy_check_mark: |
| [GLFW](https://github.com/glfw/glfw)           | 3.3.6   | *Window manager library.*  | (:heavy_check_mark: ) | :heavy_check_mark: |

<tiny><em>(Version numbers described the development version and do not suggest a strict dependency).</em></tiny>

#### Compilation.

First, retrieve the project and its submodules via the command-line by typing :
```bash
git clone --recurse-submodules -j4 https://github.com/tcoppex/gl-wasm-template
cd gl-wasm-template
```

Create a build directory :
```bash
mkdir BUILDs && cd BUILDs
```

Compile for desktop : 
```bash
mkdir desktop && cd desktop
cmake ../.. -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ..
```

Compile for the web (*using `emcmake` before `cmake`*) :
```bash
mkdir web && cd web
emcmake cmake ../.. -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ..
```

### Running the application.

You will find the desktop application in the `bin/` directory and the web 
application in `bin/public/`.

*WebAssembly* apps require a proper server to run. For this you can either use
 `http-server` (if available on your system) or the custom script provided in `tools/simple_server` :
```bash
python3 ../tools/simple_server/main.py --serve ../bin/public
```

<!-- 
## Known Limitations.

##### unimportant warnings
* mime-type warning is due to a server not recognizing the wasm mime-type. Use `tools/simple_server` to get ride of it.
* emscripten_set_main_loop_timing warning is due to a call of `glfwSwapInterval` before the emscripten mainloop is launched. 
You can disable it using the simple api provided on JS side, once the loop is launched. 
-->

### TODO

* Better web / js pipeline.
* NPM package utilities (compiler / minifier / automatic server),
* Improved events handling,
* Advanced samples structures,
* Advanced front-end : React sample,
* Advanced back-end : server-side sample,
* native Android support ?

## References

* https://emscripten.org/

## License

This project is released under the *MIT license*.
