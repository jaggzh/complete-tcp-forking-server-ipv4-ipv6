#include <netdb.h>
#include <netinet/in.h>

void setup();

void our_cb_svr_sig(int sig);
void our_cb_cl_connect(
		const char *ipstr,
		int sockfd,
		struct sockaddr_in *cli_addr
		);
void our_cb_cl_read(
		const char *ipstr,
		int sockfd,
		struct sockaddr_in *cli_addr,
		char *buf,
		size_t buflen
		);
void our_cb_cl_disconnect(
		const char *ipstr,
		int sockfd,
		struct sockaddr_in *cli_addr,
		int readval
		);
