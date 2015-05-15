#include "TransferData.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <dirent.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

void error(const char *msg)
{
	perror(msg);
	exit(0);
}
int getdir(string dir, vector<string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		//cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}

void getdirname(char cwdd[]) // 1024
{
	char cwd[1024];
	printf("%lu\n", sizeof(cwd));
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		fprintf(stdout, "Current working dir: %s\n", cwd);
		for (int i = 0; i < 1024; i++)
		{
			cwdd[i] = cwd[i];
		}
	}
	else
		perror("getcwd() error");
}

int connect()
{
	int sockfd, portno, n;
	portno = 12345;

	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname("54.149.42.37");
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");
	//printf("Please enter the message: ");
	bzero(buffer, 256);
	char a[1024];
	getdirname(a);
	for (int i = 0; i< 255; i++)
	{
		buffer[i] = a[i];

	}
	printf("%c", buffer[0]);

	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
		error("ERROR writing to socket");
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0)
		error("ERROR reading from socket");
	bzero(buffer, 256);

	string dir = string("/");
	vector<string> files = vector<string>();

	getdir(dir, files);
	int bufferLength = 0;
	for (unsigned int i = 0; i < files.size(); i++) {
		cout << files[i] << endl;
		for (int j = 0; j < files[i].length(); j++)
		{
			if (bufferLength < 250)
			{
				buffer[bufferLength] = files[i][j];
				bufferLength++;
			}
		}
	}
	buffer[bufferLength] = '\0';
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
		error("ERROR writing to socket");
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0)
		error("ERROR reading from socket");
	printf("%s\n", buffer);
	close(sockfd);
	return 0;
}
