#include <netdb.h>
#include <netinet/in.h>

void sighandler(int sig);
void doprocessing(int sock, struct sockaddr_in *cli_addr);
void *get_in_addr(struct sockaddr *sa);
