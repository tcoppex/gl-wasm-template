#!/usr/bin/python3

#   (https://unlicense.org)
#
#   Run a simple HTTP server to handle WebAssembly mime type.
#
#   For more info, see :
#       * https://docs.python.org/3/library/socketserver.html 
#

DEFAULT_HOST = '127.0.0.1'
DEFAULT_PORT = 8080

def run_unique_server(addr, port, rh):
  """ Find first available port for the socket server, launch it and open a
      new browser tab to it. """

  try:
    import socketserver
    import webbrowser
    with socketserver.TCPServer((addr, port), rh) as server:
      url = 'http://%s:%d/' % (addr, port)
      print("Serving HTTP on %s port %d (%s), exit with Ctrl-C." % (addr, port, url))
      webbrowser.open(url)
      server.serve_forever()
  except OSError:
    run_unique_server(addr, port+1, rh)
  except KeyboardInterrupt:
    print("\nBye-bye server.")

def run_server(addr = DEFAULT_HOST, port = DEFAULT_PORT):
  """ Configure a server for http request and launch it """

  import http.server

  rh = http.server.SimpleHTTPRequestHandler
  rh.extensions_map = {
    '.manifest': 'text/cache-manifest',
    '.html': 'text/html',
    '.png': 'image/png',
    '.jpg': 'image/jpg',
    '.svg': 'image/svg+xml',
    '.css': 'text/css',
    '.js':  'application/x-javascript',
    '.wasm': 'application/wasm',
    '': 'application/octet-stream',
  }
  run_unique_server(addr, port, rh)

if __name__ == '__main__':
  run_server()
