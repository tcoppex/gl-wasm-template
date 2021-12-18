"use strict";

// ----------------------------------------------------------------------------

/* Application name, used to retrieve the compiled WASM / JS files. */
const kAppName = "wasm_app";

/* Canvas ID to receive the OpenGL context. */
const kOpenGLCanvasID = "canvas";

/* Pre-declare the module object, modified after <body> loaded. */
var Module = {};

/* Pre-declare the api instance, used to interface with the C++ code. */
var api = null;

// ----------------------------------------------------------------------------

/* Simple interface to the C++ code. */
class API {
  constructor (module) {
    this.module = module;
    this.argc = module.arguments.length;
    this.argv = module.arguments.slice();

    this.setBackgroundColor = module._setBackgroundColor;
    this.setVerticalSync = module._setVerticalSync;
  }
};

/* Initialize our API. */
function initialize_custom_cppjs_api() {
  api = new API(Module);
  api.setBackgroundColor(0.0, 0.5, 0.8);
}

// ----------------------------------------------------------------------------

/* Custom event for the HTML file. */
function onRequestFullscreen() {
  let $pointerLock = document.getElementById('pointerLock');
  let $resize      = document.getElementById('resize');
  Module.requestFullscreen($pointerLock.checked, $resize.checked);
}

/* Helper to retrieve the OpenGL canvas. */
function getOpenGLCanvas() { 
  return document.getElementById(kOpenGLCanvasID);
};

// ----------------------------------------------------------------------------

window.addEventListener("load", () => {
  console.log(" >> Loaded.");

  // [Optional] setup default values for the module instance.
  Module = {
    // Redirect STDIN.
    print: (msg) => { console.log(msg); },

    // Redirect STDOUT.
    printErr: (msg) => { console.warn(msg); },

    // [Requiered] Canvas used for the OpenGL context.
    canvas: (() => {
      const canvas = getOpenGLCanvas();
      // As a default initial behavior, pop up an alert when webgl context is lost. To make your
      // application robust, you may want to override this behavior before shipping!
      // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
      canvas.addEventListener("webglcontextlost", (e) => { 
          alert("WebGL context lost. You will need to reload the page."); 
          e.preventDefault(); 
        }, 
        false
      );
      return canvas;
    })()
  };

  /* Fetch manually the WASM / App js script. */
  /// When using this method we cannot add "prefix-js" module code inside the
  /// compiled JS, or it will be replaced.
  fetch(`${kAppName}.wasm`)
    .then(response => response.arrayBuffer())
    .then(bytes => {
      // Setup the wasm binary to prevent the script to reload it.
      Module.wasmBinary = bytes;

      // Using OpenGL we need at least a canvas entry.
      if (Module.canvas === undefined) {
        Module.canvas = getOpenGLCanvas();
      }
      
      // Include the webapp's script.
      const script = document.createElement("script");
      script.src = `${kAppName}.js`;
      document.body.appendChild(script);
    })
  ;
});

// ----------------------------------------------------------------------------
