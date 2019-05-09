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
	char szBuf[] = "GET / HTTP/1.0\r\n\
Host: 127.0.0.1:4000\r\n\
Connection: keep-close\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 SE 2.X MetaSr 1.0\r\n\
Accept-Encoding: gzip, deflate\r\n\r\n\r\n";
	unsigned long nLen = strlen(szBuf);
	long nRead = 0;
	char bufRead[512] = { 0 };

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(4000);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);

	if (result == -1)
	{
		perror("oops: client1");
		return -1;
	}
#ifdef WIN32
	send(sockfd, szBuf, nLen, 0);
	nRead = recv(sockfd, bufRead, 512, 0);
#else
	write(sockfd, szBuf, nLen);
	nRead = read(sockfd, bufRead, 512);
#endif // WIN32
	printf("buff from server = %s", szBuf);
#ifdef WIN32
	closesocket(sockfd);
	WSACleanup();
#else
	close(sockfd);
#endif
	return 0;
}
