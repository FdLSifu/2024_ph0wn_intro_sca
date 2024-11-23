#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/random.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <bits/sigaction.h>
#include "rsa.h"

#define MAX_CONNECT 20

ssize_t recv_until(int fd, char *buf, size_t n)
{
	size_t size = 0;
	char c;
	ssize_t rc;

	while (size < n)
	{
		rc = recv(fd, &c, 1, 0);

		if (rc == -1)
		{
			if (errno == EAGAIN || errno == EINTR)
			{
				continue;
			}
			return -1;
		}

		buf[size] = c;
		size++;
	}

	buf[size] = '\0';
	return size;
}

ssize_t sendlen(int fd, const char *buf, size_t n)
{
	ssize_t rc;
	size_t nsent = 0;

	while (nsent < n)
	{
		rc = send(fd, buf + nsent, n - nsent, 0);

		if (rc == -1)
		{
			if (errno == EAGAIN || errno == EINTR)
			{
				continue;
			}
			return -1;
		}

		nsent += rc;
	}
	return nsent;
}

void handle_client(int client)
{
	char received[MAX_SIZE + 1] = {0};
	char *trace = 0;
	int tracelen = 0;

	if (recv_until(client, received, MAX_SIZE) != MAX_SIZE)
	{
		close(client);
		exit(1);
	}

	trace = calloc(NB_LEAKAGE, sizeof(char));
	if (trace)
	{
		tracelen = rsa_withleakage(received, trace);
		sendlen(client, trace, tracelen);
		free(trace);
	}
	else
	{
		close(client);
		exit(1);
	}
}

void handle_signal(int signal)
{
	signal = signal;
	waitpid(-1, NULL, WNOHANG);
}

int server(int port)
{
	int server, client, opt;
	unsigned int clientlen;
	struct sockaddr_in structserver, structclient;
	struct sigaction sig;
	pid_t pid;

	if (port < 1024)
	{
		puts("[-] Port must be > 1024");
		exit(1);
	}

	sig.sa_handler = handle_signal;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_NOCLDSTOP;

	if (sigaction(SIGCHLD, &sig, NULL) == -1)
	{
		perror("[-] Sigaction Fail ");
		exit(1);
	}

	if ((server = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[-] Socket Fail ");
		exit(1);
	}

	puts("[+] Socket created.");

	opt = 1;

	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
	{
		perror("setsockopt");
		exit(1);
	}

	memset(&structserver, 0, sizeof(structserver));
	structserver.sin_family = AF_INET;
	structserver.sin_addr.s_addr = inet_addr("0.0.0.0");
	structserver.sin_port = htons(port);

	if (bind(server, (struct sockaddr *)&structserver, sizeof(structserver)) == -1)
	{
		perror("[-] Bind Fail ");
		exit(1);
	}

	if (listen(server, MAX_CONNECT) == -1)
	{
		perror("[-] Listen Fail ");
		exit(1);
	}

	printf("[+] Listening on %d.\n", port);

	unsigned int seed = 0;
	for (;;)
	{
		memset(&structclient, 0, sizeof(structclient));
		clientlen = sizeof(structclient);
		client = accept(server, (struct sockaddr *)&structclient, &clientlen);

		if (getrandom(&seed, sizeof(seed), 0) == -1)
		{
			exit(1);
		}
		srand(seed);

		if (client == -1)
		{
			if (errno != EINTR)
			{
				perror("[-] Accept Fail ");
			}
			continue;
		}

		printf("[+] New client : %s\n", inet_ntoa(structclient.sin_addr));

		if ((pid = fork()) == -1)
		{
			perror("[-] Fork Fail ");
			continue;
		}

		if (!pid)
		{ /* child */
			close(fileno(stdin));
			close(fileno(stdout));
			close(fileno(stderr));

			if ((pid = fork()) == -1)
			{
				perror("[-] Fork Fail ");
				continue;
			}

			if (!pid)
			{ /* child's child; prevents zombies */
				alarm(60);
				// Init random

				handle_client(client);
			}

			exit(0);
		}
		else
		{ /* parent */
			close(client);
		}
	}

	close(server);
	return 0;
}
