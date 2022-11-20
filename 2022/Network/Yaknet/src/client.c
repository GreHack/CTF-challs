// C program for the Client Side
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
// inet_addr
#include <arpa/inet.h>
#include <unistd.h>

// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>

#include "proto.h"

static int client_connect(const char *host, const char *port)
{
	struct addrinfo hints = { 0 };
	struct addrinfo *addrs, *p;
	int ret;
	int sfd;

	hints.ai_family = AF_UNSPEC; /* or AF_INET */
	hints.ai_socktype = SOCK_STREAM; /* or DGRAM for UDP ? */
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	ret = getaddrinfo(host, port, &hints, &addrs);
	if (ret != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		return -1;
	}

	/* find the first address that successfully connect */
	for (sfd = -1, p = addrs; p != NULL; p = p->ai_next) {
		sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sfd == -1)
			continue;
		ret = connect(sfd, p->ai_addr, p->ai_addrlen);
		if (ret != -1)
			break; /* success */
		close(sfd); /* try next entry */
	}
	freeaddrinfo(addrs);
	if (p == NULL) {
		/* tried all the addresses */
		fprintf(stderr, "could not connect\n");
		return -1;
	}

	return sfd;
}

// Function to send data to
// server socket.
void client_main(char *host, char *port, char *username)
{
	int sfd;
	char list_command_to_send[4][5] = { "hey", "get", "put", "toto"};
	char rcv_response[400];
	struct cmd_msg m_rcv = { 0 };
	ssize_t n;

	sfd = client_connect(host, port);
	if (sfd < 0) {
		fprintf(stderr, "Error\n");
		return;
	}
	n = recv_cmd(sfd, &m_rcv);
	printf("Connection established\n");
    printf("sending hey with %s\n", username);
	send_hey(sfd, username);

	n = recv_cmd(sfd, &m_rcv);
	if (n < 0)
		printf("err\n");
	if (n != sizeof(m_rcv)) {
		fprintf(stderr, "recv invalid len %d should be = %d\n", n, sizeof(m_rcv));
	}
	printf("1. responded %i : %s\n", m_rcv.cmd, m_rcv.rsp.msg);

	send_get(sfd, 2, 2);
	n = recv_cmd(sfd, &m_rcv);
	if (n < 0)
		printf("err\n");
	if (n != sizeof(m_rcv))
		fprintf(stderr, "recv invalid len %d should be = %d\n", n, sizeof(m_rcv));

	printf("2. responded %d : x:%i, y:%i, val:%i\n", m_rcv.cmd,  m_rcv.get.x,  m_rcv.get.y, m_rcv.get.val);
	send_get(sfd, 3, 3);
	n = recv_cmd(sfd, &m_rcv);
	printf("3. responded %d : x:%i, y:%i, val:%i\n", m_rcv.cmd,  m_rcv.get.x,  m_rcv.get.y, m_rcv.get.val);

    send_get(sfd, 93, 43);
	n = recv_cmd(sfd, &m_rcv);
	printf("4. responded %d : x:%i, y:%i, val:%i\n", m_rcv.cmd,  m_rcv.get.x,  m_rcv.get.y, m_rcv.get.val);


	send_mov(sfd, 1, 1);
    n = recv_cmd(sfd, &m_rcv);
	//printf("responded %d : %s\n", m_rcv.cmd,  m_rcv.rsp.msg);
	printf("5. responded %d : x:%f, y:%f\n", m_rcv.cmd,  m_rcv.pos.x,  m_rcv.pos.y);

    send_mov(sfd, 94, 4);
	n = recv_cmd(sfd, &m_rcv);
	//printf("responded %d : %s\n", m_rcv.cmd,  m_rcv.rsp.msg);
	printf("6. responded %d : x:%f, y:%f\n", m_rcv.cmd,  m_rcv.pos.x,  m_rcv.pos.y);

	send_mov(sfd, 94,43);
	send_mov(sfd, 94,43);

	close(sfd);
	return;
}

// Driver Code
int main(int argc, char **argv)
{
	char *host = "localhost";
	char *port = "12345";
	char *username = "GreHack_Player1";

	// Create connection
	// depending on the input
	if (argc > 1) {
		host = argv[1];
	    if (argc > 2) {
		    port = argv[2];
		    if (argc > 3) {
		        username = argv[3];
		    }
		}
    } else {
        printf("No hostname or port specify will use default ones");
	    printf("Usage: ./yaknet [hostname] [port] [username(optional)]\n");
    }
    printf("hostname : %s\n", host);
    printf("port : %s\n", port);
    printf("username : %s\n", username);

	client_main(host, port, username);
	// Create thread
	//pthread_create(&tid, NULL,
	//            clienthread,
	//            &client_request);
	// Suspend execution of
	// calling thread
	//pthread_join(tid, NULL);
}
