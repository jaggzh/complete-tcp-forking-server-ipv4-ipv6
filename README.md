# Complete basic TCP/IP Forking Server Example

Many examples exist online but they're incomplete or over-simplified.  This is intended as a decent basic forking server example.  It handles:

1. IPv4 and IPv6
1. Forking to allow multiple clients
1. Proper (I hope) re-use of port to prevent "Address already in use" errors when binding
1. SIGINT/HUP/TERM handling to close the socket (is that needed with SO_REUSEADDR?)

## Update 2021-01-11

1. Added Callback-based server in `forking-server-with-callbacks/`
1. Forking server has improvements to signal handling

## Example of callback server running:

Server:
```
$ ./server_cbs 
Server started on port 1234
Client connected
_svr_client_handler()
Client connected on 127.0.0.1
---> CB: Client connected: IP=127.0.0.1
Received from 127.0.0.1: 7 bytes: {Hello
}
---> CB: Our read callback called
Received from 127.0.0.1: 7 bytes: {World
}
---> CB: Our read callback called
---> CB: Our client disconnected with read() = 0
Client disconnected (EOF on socket)
```

Telnet localhost 1234:
```
$ telnet localhost 1234
Trying ::1...
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Hello
World
^]
telnet> quit
Connection closed.
```
