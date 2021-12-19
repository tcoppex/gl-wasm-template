## C++ / WebAssembly OpenGL App template.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


This project offers a light boilerplate to build cross-platform _**C++17** / **WebAssembly**_ OpenGL app using CMake, emscripten, and GLFW.

### Building.

We will be using the command-line on Unix and [Git Bash](https://git-for-windows.github.io/) on Windows.

<details>
  <summary><strong>Quick start & run !</strong></summary>
<div class="highlight highlight-source-shell"><pre><span class="pl-c"><span class="pl-c">#</span> [Optional] Retrieve system build dependencies on Unix via synaptic.</span>
<span class="pl-c"><span class="pl-c">#</span> sudo apt install git build-essential cmake ninja-build</span>

<span class="pl-c"><span class="pl-c">#</span> Clone the project with its submodule.</span>
git clone --recurse-submodules -j4 https://github.com/tcoppex/gl-wasm-template
<span class="pl-c1">cd</span> gl-wasm-template

<span class="pl-c"><span class="pl-c">#</span> Create a build directory.</span>
mkdir BUILD <span class="pl-k">&amp;&amp;</span> <span class="pl-c1">cd</span> BUILD

<span class="pl-c"><span class="pl-c">#</span> Compile for the web (using emcmake before cmake).</span>
emcmake cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build <span class="pl-c1">.</span>

<span class="pl-c"><span class="pl-c">#</span> Serve the webapp.</span>
python3 ../tools/simple_server/main.py --serve ../bin/public
</pre>
</div>
</details>

#### Requirements.

We will need the [emscripten SDK](https://github.com/emscripten-core/emsdk) installed and setup on our system. Others dependencies
can be either retrieved via our package manager or as submodules.

The following table summarizes the project dependencies :

| Dependency            | Version | Description                | Shipped               | Required           | 
|-----------------------|---------|----------------------------|-----------------------|--------------------|
| [Emscripten SDK](https://github.com/emscripten-core/emsdk) | 3.0.0   | *WebAssembly toolchains.*  | :x:                   | :heavy_check_mark: |
| [CMake](https://github.com/Kitware/CMake)          | 2.8.12  | *Build tool.*              | :x:                   | :heavy_check_mark: |
| [GLFW](https://github.com/glfw/glfw)           | 3.3.6   | *Window manager library.*  | (:heavy_check_mark: ) | :heavy_check_mark: |

<tiny><em>(Version numbers described the development 
version and do not suggest a strict dependency).</em></tiny>

First, retrieve the project and its submodules via the command-line by typing :
```bash
git clone --recurse-submodules -j4 https://github.com/tcoppex/gl-wasm-template
cd gl-wasm-template
```

#### Compilation.

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

### Running the App.

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
* emscripten_set_main_loop_timing warning is due to a call of `glfwSwapInterval` before the emscripten mainloop is launched. You can disable it using the simple api provided on JS side, once the loop is launched. 
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
