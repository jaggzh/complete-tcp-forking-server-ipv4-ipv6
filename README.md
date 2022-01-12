# Complete basic TCP/IP Forking Server Example

Many examples exist online but they're incomplete or over-simplified.  This is intended as a decent basic forking server example.  It handles:

1. IPv4 and IPv6
1. Forking to allow multiple clients
1. Proper (I hope) re-use of port to prevent "Address already in use" errors when binding
1. SIGINT/HUP/TERM handling to close the socket (is that needed with SO_REUSEADDR?)

## Update 2021-01-11

1. Added Callback-based server in `forking-server-with-callbacks/`
1. Forking server has improvements to signal handling
