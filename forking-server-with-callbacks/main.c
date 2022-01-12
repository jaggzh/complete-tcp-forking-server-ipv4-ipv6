#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>

#include "settings.h" // copy from setting-example.h
#include "main.h"
#include "server_cbs.h"

int main(int argc, char *argv[]) {
	setup();
	svr_start(PORT); // doesn't end until dead
}

void setup() {
	cb_svr_sig = our_cb_svr_sig;         // at termination of server
	cb_svr_cl_connect = our_cb_cl_connect; // for each connect
	cb_svr_cl_read = our_cb_cl_read;       // for each read
	cb_svr_cl_disconnect = our_cb_cl_disconnect; // for each connect
}

void our_cb_svr_sig(int sig) {
	printf("---> CB: Server terminating\n");
}

void our_cb_cl_connect(
		const char *ipstr,
		int sockfd,
		struct sockaddr_in *cli_addr
		) {
	printf("---> CB: Client connected: IP=%s\n", ipstr);
}

void our_cb_cl_read(                // called on data read
		const char *ipstr,
		int sockfd,
		struct sockaddr_in *cli_addr,
		char *buf,
		size_t buflen
		) {
	printf("---> CB: Our read callback called\n");
}

void our_cb_cl_disconnect(                // called on data read
		const char *ipstr,
		int sockfd,
		struct sockaddr_in *cli_addr,
		int readval
		) {
	printf("---> CB: Our client disconnected with read() = %d\n", readval);
}


