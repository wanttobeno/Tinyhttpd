#include <stdio.h>
#include <sys/types.h>

#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib,"Ws2_32")
#else
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#endif

int main(int argc, char *argv[])
{
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return 0;
	}
#endif // WIN32
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(4000);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);

	if (result == -1)
	{
		perror("oops: client1");
		exit(1);
	}
#ifdef WIN32
	send(sockfd, &ch,1,0);
	recv(sockfd, &ch, 1, 0);
#else
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
#endif // WIN32
	printf("char from server = %c\n", ch);
#ifdef WIN32
	closesocket(sockfd);
	WSACleanup();
#else
	close(sockfd);
#endif
	exit(0);
}
