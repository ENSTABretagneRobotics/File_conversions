// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Utils.h"

/*
Connect to an IPv4 UDP server.

SOCKET* pSock : (INOUT) Valid pointer to a socket that will be used to communicate with the server.
char* address : (IN) IPv4 address of the server.
char* port : (IN) TCP port of the server.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int initudpcli(SOCKET* pSock, char* address, char* port)
{
	struct sockaddr_in sa;

#ifdef _WIN32
	WSADATA wsaData;
#endif // _WIN32

#ifdef _WIN32
	// Initiate use of the Winsock DLL by a process.
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	// Create a UDP IPv4 socket.
	*pSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (*pSock == INVALID_SOCKET)
	{
		printf("socket() failed.\n");
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	// Configure timeouts for send and recv.
	if (setsockettimeouts(*pSock, DEFAULT_SOCK_TIMEOUT) != EXIT_SUCCESS)
	{
		printf("setsockettimeouts() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}
	/*
	memset(&sa, 0, sizeof(sa));

	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the client.
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(address);
	sa.sin_port = htons((unsigned short)atoi(port));

	// Associate the client to the desired address and port.
	if (bind(*pSock, (struct sockaddr*)&sa, sizeof(sa)) != EXIT_SUCCESS)
	{
	printf("bind() failed.\n");
	closesocket(*pSock);
	#ifdef _WIN32
	WSACleanup();
	#endif // _WIN32
	return EXIT_FAILURE;
	}
	*/
	memset(&sa, 0, sizeof(sa));

	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(address);
	sa.sin_port = htons((unsigned short)atoi(port));

	// Connect to server.
	if (connect(*pSock, (struct sockaddr*)&sa, sizeof(sa)) != EXIT_SUCCESS)
	{
		printf("connect() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Connect to an IPv4 TCP server.

SOCKET* pSock : (INOUT) Valid pointer to a socket that will be used to communicate with the server.
char* address : (IN) IPv4 address of the server.
char* port : (IN) TCP port of the server.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int inittcpcli(SOCKET* pSock, char* address, char* port)
{
	struct sockaddr_in sa;

#ifdef _WIN32
	WSADATA wsaData;
#endif // _WIN32

#ifdef _WIN32
	// Initiate use of the Winsock DLL by a process.
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	// Create a TCP IPv4 socket.
	*pSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*pSock == INVALID_SOCKET)
	{
		printf("socket() failed.\n");
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	// Configure timeouts for send and recv.
	if (setsockettimeouts(*pSock, DEFAULT_SOCK_TIMEOUT) != EXIT_SUCCESS)
	{
		printf("setsockettimeouts() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	memset(&sa, 0, sizeof(sa));

	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(address);
	sa.sin_port = htons((unsigned short)atoi(port));

	// Connect to server.
	if (connect(*pSock, (struct sockaddr*)&sa, sizeof(sa)) != EXIT_SUCCESS)
	{
		printf("connect() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Set timeout options for a given socket.

SOCKET sock : (IN) Socket.
int timeout : (IN) Timeout in ms for send and recv (0 to disable timeouts).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int setsockettimeouts(SOCKET sock, int timeout)
{
#ifdef _WIN32
	int iOptVal = 0;
#else
	struct timeval tv;
#endif // _WIN32
	int iOptLen = 0;

#ifdef _WIN32
	iOptVal = timeout; // In ms.
	iOptLen = sizeof(int);
	if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&iOptVal, iOptLen) == SOCKET_ERROR) 
	{
		printf("setsockopt() failed.\n");
		return EXIT_FAILURE;
	}
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iOptVal, iOptLen) == SOCKET_ERROR) 
	{
		printf("setsockopt() failed.\n");
		return EXIT_FAILURE;
	}
#else
	tv.tv_sec = (long)(timeout/1000);
	tv.tv_usec = (long)((timeout%1000)*1000);
	iOptLen = sizeof(struct timeval);
	if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, iOptLen) == SOCKET_ERROR)
	{
		printf("setsockopt() failed.\n");
		return EXIT_FAILURE;
	}
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, iOptLen) == SOCKET_ERROR)
	{
		printf("setsockopt() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	return EXIT_SUCCESS;
}

#ifdef _MSC_VER
// Disable some Visual Studio warnings.
#pragma warning(disable : 4127) 
#endif // _MSC_VER
/*
Wait for data to receive on a given socket.

SOCKET sock : (IN) Socket.
int timeout : (IN) Timeout in ms.

Return : EXIT_SUCCESS, EXIT_TIMEOUT or EXIT_FAILURE if there is an error.
*/
int waitforsocket(SOCKET sock, int timeout)
{
	fd_set sock_set;
	int iResult = SOCKET_ERROR;
	struct timeval tv;

	tv.tv_sec = (long)(timeout/1000);
	tv.tv_usec = (long)((timeout%1000)*1000);

	// Initialize a fd_set and add the socket to it.
	FD_ZERO(&sock_set); 
	FD_SET(sock, &sock_set);

	iResult = select((int)sock+1, &sock_set, NULL, NULL, &tv);

	// Remove the socket from the set.
	// No need to use FD_ISSET() here, as we only have one socket the return value of select() is 
	// sufficient to know what happened.
	FD_CLR(sock, &sock_set); 

	switch (iResult)
	{
	case SOCKET_ERROR:
		printf("select() failed.\n");
		return EXIT_FAILURE;
	case 0:
		// The timeout on select() occured.
		printf("select() timed out.\n");
		return EXIT_TIMEOUT;
	default:
		return EXIT_SUCCESS;
	}
}

/*
Eliminate all data coming from a given socket (if any).

SOCKET sock : (IN) Socket.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int purgesocket(SOCKET sock)
{
	char recvbuf[MAX_BUF_LEN];
	int Bytes = 0;

	for (;;)
	{
		fd_set sock_set;
		int iResult = SOCKET_ERROR;
		struct timeval tv;

		// Timeout of 0 s for select().
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		// Initialize a fd_set and add the socket to it.
		FD_ZERO(&sock_set); 
		FD_SET(sock, &sock_set);

		// Check the readability of the socket in the fd_set.
		iResult = select((int)sock+1, &sock_set, NULL, NULL, &tv);

		// Remove the socket from the set.
		// No need to use FD_ISSET() here, as we only have one socket the return value of select() is 
		// sufficient to know what happened.
		FD_CLR(sock, &sock_set); 

		switch (iResult)
		{
		case SOCKET_ERROR:
			printf("select() failed.\n");
			return EXIT_FAILURE;
		case 0:
			// The timeout on select() occured.
			return EXIT_SUCCESS;
		default:
			// There is some data on the socket.
			Bytes = recv(sock, recvbuf, MAX_BUF_LEN, 0);
			if (Bytes <= 0)
			{
				printf("recv() failed or timed out.\n");
				return EXIT_FAILURE;
			}
			break;
		}
	}
}
#ifdef _MSC_VER
// Restore the Visual Studio warnings previously disabled.
#pragma warning(default : 4127) 
#endif // _MSC_VER

int sendtoall(SOCKET sock, char* sendbuf, int sendbuflen, struct sockaddr* sa, int salen)
{
	int BytesSent = 0;
	int Bytes = 0;

	while (BytesSent < sendbuflen)
	{
		Bytes = sendto(sock, sendbuf + BytesSent, sendbuflen - BytesSent, 0, sa, (socklen_t)salen);
		if (Bytes >= 0)
		{
			if (Bytes == 0)
			{
				printf("sendto() timed out.\n");
				//printf("Total bytes sent : %u\n", BytesSent);
				return EXIT_TIMEOUT;
			}
			else
			{
				//printf("Bytes sent : %d\n", Bytes);
			}
		}
		else
		{
			printf("sendto() failed.\n");
			//printf("Total bytes sent : %u\n", BytesSent);
			return EXIT_FAILURE;
		}

		BytesSent += Bytes;
	}

	return EXIT_SUCCESS;
}

int recvfromall(SOCKET sock, char* recvbuf, int recvbuflen, struct sockaddr* sa, int* pSalen)
{
	int BytesReceived = 0;
	int Bytes = 0;

	while (BytesReceived < recvbuflen)
	{
		Bytes = recvfrom(sock, recvbuf + BytesReceived, recvbuflen - BytesReceived, 0, sa, (socklen_t*)pSalen);
		if (Bytes >= 0)
		{
			if (Bytes == 0)
			{
				printf("recvfrom() timed out.\n");
				//printf("Total bytes received : %u\n", BytesReceived);
				return EXIT_TIMEOUT;
			}
			else
			{
				//printf("Bytes received : %d\n", Bytes);
			}
		}
		else
		{
			printf("recvfrom() failed.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			return EXIT_FAILURE;
		}

		BytesReceived += Bytes;
	}

	return EXIT_SUCCESS;
}

/*
Send data to a given socket. Retry automatically if all the bytes were not sent.
Fail when a timeout occurs if it is enabled on the socket.

SOCKET sock : (IN) Socket.
char* sendbuf : (IN) Data to send.
int sendbuflen : (IN) Number of bytes to send.

Return : EXIT_SUCCESS, EXIT_TIMEOUT or EXIT_FAILURE if there is an error.
*/
int sendall(SOCKET sock, char* sendbuf, int sendbuflen)
{
	int BytesSent = 0;
	int Bytes = 0;

	while (BytesSent < sendbuflen)
	{
		Bytes = send(sock, sendbuf + BytesSent, sendbuflen - BytesSent, 0);
		if (Bytes >= 0)
		{
			if (Bytes == 0)
			{
				printf("send() timed out.\n");
				//printf("Total bytes sent : %u\n", BytesSent);
				return EXIT_TIMEOUT;
			}
			else
			{
				//printf("Bytes sent : %d\n", Bytes);
			}
		}
		else
		{
			printf("send() failed.\n");
			//printf("Total bytes sent : %u\n", BytesSent);
			return EXIT_FAILURE;
		}

		BytesSent += Bytes;
	}

	return EXIT_SUCCESS;
}

/*
Receive data at a given socket. Retry automatically if all the bytes were not received.
Fail when a timeout occurs if it is enabled on the socket.

SOCKET sock : (IN) Socket.
char* recvbuf : (INOUT) Buffer which will contain the data received.
int recvbuflen : (IN) Number of bytes to receive.

Return : EXIT_SUCCESS, EXIT_TIMEOUT or EXIT_FAILURE if there is an error.
*/
int recvall(SOCKET sock, char* recvbuf, int recvbuflen)
{
	int BytesReceived = 0;
	int Bytes = 0;

	while (BytesReceived < recvbuflen)
	{
		Bytes = recv(sock, recvbuf + BytesReceived, recvbuflen - BytesReceived, 0);
		if (Bytes >= 0)
		{
			if (Bytes == 0)
			{
				printf("recv() timed out.\n");
				//printf("Total bytes received : %u\n", BytesReceived);
				return EXIT_TIMEOUT;
			}
			else
			{
				//printf("Bytes received : %d\n", Bytes);
			}
		}
		else
		{
			printf("recv() failed.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			return EXIT_FAILURE;
		}

		BytesReceived += Bytes;
	}

	return EXIT_SUCCESS;
}

int recvlatest(SOCKET sock, char* recvbuf, int recvbuflen)
{
	int BytesReceived = 0;
	int Bytes = 0;
	char* savebuf = NULL;

	savebuf = (char*)calloc(recvbuflen, sizeof(char));

	if (savebuf == NULL)
	{
		printf("calloc() failed.\n");
		return EXIT_OUT_OF_MEMORY;
	}

	Bytes = recv(sock, recvbuf, recvbuflen, 0);
	if (Bytes >= 0)
	{
		if (Bytes == 0)
		{
			printf("recv() timed out.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			free(savebuf);
			return EXIT_TIMEOUT;
		}
		else
		{
			//printf("Bytes received : %d\n", Bytes);
		}
	}
	else
	{
		printf("recv() failed.\n");
		//printf("Total bytes received : %u\n", BytesReceived);
		free(savebuf);
		return EXIT_FAILURE;
	}

	BytesReceived += Bytes;

	while (Bytes == recvbuflen)
	{
		memcpy(savebuf, recvbuf, Bytes);
		Bytes = recv(sock, recvbuf, recvbuflen, 0);
		if (Bytes >= 0)
		{
			if (Bytes == 0)
			{
				printf("recv() timed out.\n");
				//printf("Total bytes received : %u\n", BytesReceived);
				free(savebuf);
				return EXIT_TIMEOUT;
			}
			else
			{
				//printf("Bytes received : %d\n", Bytes);
			}
		}
		else
		{
			printf("recv() failed.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			free(savebuf);
			return EXIT_FAILURE;
		}

		BytesReceived += Bytes;
	}

	if (BytesReceived < recvbuflen)
	{
		int iResult = recvall(sock, recvbuf+BytesReceived, recvbuflen-BytesReceived);
		if (iResult != EXIT_SUCCESS)
		{
			free(savebuf);
			return iResult;
		}
	}
	else
	{
		memmove(recvbuf+recvbuflen-Bytes, recvbuf, Bytes);
		memcpy(recvbuf, savebuf+Bytes, recvbuflen-Bytes);
	}

	free(savebuf);

	return EXIT_SUCCESS;
}

/*
Receive data at a given socket until a specific end character is received.
If this character is found (and the maximum number of bytes to receive has not 
been reached), it is not necessarily the last received byte in the buffer 
(other bytes might have been received after).
Fail when a timeout occurs if it is enabled on the socket.

SOCKET sock : (IN) Socket.
char* recvbuf : (INOUT) Buffer which will contain the data received.
char endchar : (IN) End character to wait for.
int maxrecvbuflen : (IN) Maximum number of bytes to receive.

Return : EXIT_SUCCESS, EXIT_TIMEOUT or EXIT_FAILURE if there is an error.
*/
int recvatleastuntil(SOCKET sock, char* recvbuf, char endchar, int maxrecvbuflen)
{
	int BytesReceived = 0;
	int Bytes = 0;
	int bStop = 0;

	for (;;)
	{
		if (BytesReceived >= maxrecvbuflen)
		{
			printf("recvbuf full.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			return EXIT_FAILURE;
		}

		Bytes = recv(sock, recvbuf + BytesReceived, maxrecvbuflen - BytesReceived, 0);
		if (Bytes >= 0)
		{
			if (Bytes == 0)
			{
				printf("recv() timed out.\n");
				//printf("Total bytes received : %u\n", BytesReceived);
				return EXIT_TIMEOUT;
			}
			else
			{
				int i = 0;

				//printf("Bytes received : %d\n", Bytes);

				// Look for endchar in the bytes just received.
				for (i = BytesReceived; i < BytesReceived+Bytes; i++)
				{
					if (recvbuf[i] == endchar)
					{
						bStop = 1;
						break;
					}
				}

				if (bStop)
				{
					break;
				}
			}
		}
		else
		{
			printf("recv() failed.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			return EXIT_FAILURE;
		}

		BytesReceived += Bytes;
	}

	return EXIT_SUCCESS;
}

/*
Receive data at a given socket until a specific end character is received.
If this character is found (and the maximum number of bytes to receive has not 
been reached), it is the last received byte in the buffer.
This function might take more network load than recvatleastuntil() but guarantees
that no bytes are received after the end character (the bytes that might be between 
the end character and the maximum number of bytes to receive are left unchanged).
Fail when a timeout occurs if it is enabled on the socket.

SOCKET sock : (IN) Socket.
char* recvbuf : (INOUT) Buffer which will contain the data received.
char endchar : (IN) End character to wait for.
int maxrecvbuflen : (IN) Maximum number of bytes to receive.

Return : EXIT_SUCCESS, EXIT_TIMEOUT or EXIT_FAILURE if there is an error.
*/
int recvuntil(SOCKET sock, char* recvbuf, char endchar, int maxrecvbuflen)
{
	int BytesReceived = 0;
	int Bytes = 0;

	// Receive byte per byte.
	while ((BytesReceived <= 0)||(recvbuf[BytesReceived-1] != endchar))
	{
		if (BytesReceived >= maxrecvbuflen)
		{
			printf("recvbuf full.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			return EXIT_FAILURE;
		}

		// Receive 1 byte.
		Bytes = recv(sock, recvbuf + BytesReceived, 1, 0);
		if (Bytes >= 0)
		{
			if (Bytes == 0)
			{
				printf("recv() timed out.\n");
				//printf("Total bytes received : %u\n", BytesReceived);
				return EXIT_TIMEOUT;
			}
			else
			{
				//printf("Bytes received : %d\n", Bytes);
			}
		}
		else
		{
			printf("recv() failed.\n");
			//printf("Total bytes received : %u\n", BytesReceived);
			return EXIT_FAILURE;
		}

		BytesReceived += Bytes;
	}

	return EXIT_SUCCESS;
}

/*
Disconnect from an IPv4 UDP server.

SOCKET sock : (IN) Socket used to communicate with the server.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int releaseudpcli(SOCKET sock)
{
	// Shutdown the connection.
	if (shutdown(sock, SD_BOTH) != EXIT_SUCCESS)
	{
		printf("shutdown() failed.\n");
		//return EXIT_FAILURE;
	}

	// Destroy the socket created by socket().
	if (closesocket(sock) != EXIT_SUCCESS)
	{
		printf("closesocket() failed.\n");
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	// Terminate use of the Winsock 2 DLL (Ws2_32.dll).
	if (WSACleanup() != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	return EXIT_SUCCESS;
}

/*
Disconnect from an IPv4 TCP server.

SOCKET sock : (IN) Socket used to communicate with the server.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int releasetcpcli(SOCKET sock)
{
	// Shutdown the connection.
	if (shutdown(sock, SD_BOTH) != EXIT_SUCCESS)
	{
		printf("shutdown() failed.\n");
		//return EXIT_FAILURE;
	}

	// Destroy the socket created by socket().
	if (closesocket(sock) != EXIT_SUCCESS)
	{
		printf("closesocket() failed.\n");
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	// Terminate use of the Winsock 2 DLL (Ws2_32.dll).
	if (WSACleanup() != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	return EXIT_SUCCESS;
}

/*
Create an IPv4 UDP server.

SOCKET* pSock : (INOUT) Valid pointer to a server socket.
char* address : (IN) IPv4 address of the server. Set to "0.0.0.0" to use all local 
network interfaces.
char* port : (IN) UDP port of the server.
int timeout : (IN) Timeout in ms for send and recv (0 to disable timeouts).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int initudpsrv(SOCKET* pSock, char* address, char* port, int timeout)
{
	struct sockaddr_in sa;

#ifdef _WIN32
	WSADATA wsaData;
#endif // _WIN32

#ifdef _WIN32
	// Initiate use of the Winsock DLL by a process.
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	// Create a UDP IPv4 socket.
	*pSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (*pSock == INVALID_SOCKET)
	{
		printf("socket() failed.\n");
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	// Configure timeouts for send and recv.
	if (setsockettimeouts(*pSock, timeout) != EXIT_SUCCESS)
	{
		printf("setsockettimeouts() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	memset(&sa, 0, sizeof(sa));

	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server.
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(address);
	sa.sin_port = htons((unsigned short)atoi(port));

	// Associate the server to the desired address and port.
	if (bind(*pSock, (struct sockaddr*)&sa, sizeof(sa)) != EXIT_SUCCESS)
	{
		printf("bind() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Create an IPv4 TCP server.

SOCKET* pSock : (INOUT) Valid pointer to a server socket.
char* address : (IN) IPv4 address of the server. Set to "0.0.0.0" to use all local 
network interfaces.
char* port : (IN) TCP port of the server.
int maxnbcli : (IN) Maximum number of simultaneous client connections. Set to 1 if there 
should be only 1 simultaneous client or to SOMAXCONN for a default maximum reasonable value.
int timeout : (IN) Timeout in ms for send and recv (0 to disable timeouts).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int inittcpsrv(SOCKET* pSock, char* address, char* port, int maxnbcli, int timeout)
{
	struct sockaddr_in sa;

#ifdef _WIN32
	WSADATA wsaData;
#endif // _WIN32

#ifdef _WIN32
	// Initiate use of the Winsock DLL by a process.
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	// Create a TCP IPv4 socket.
	*pSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*pSock == INVALID_SOCKET)
	{
		printf("socket() failed.\n");
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	// Configure timeouts for send and recv.
	if (setsockettimeouts(*pSock, timeout) != EXIT_SUCCESS)
	{
		printf("setsockettimeouts() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	memset(&sa, 0, sizeof(sa));

	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server.
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(address);
	sa.sin_port = htons((unsigned short)atoi(port));

	// Associate the server to the desired address and port.
	if (bind(*pSock, (struct sockaddr*)&sa, sizeof(sa)) != EXIT_SUCCESS)
	{
		printf("bind() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	// Set the number of simultaneous client connections that can be accepted by the server.
	if (listen(*pSock, maxnbcli) != EXIT_SUCCESS)
	{
		printf("listen() failed.\n");
		closesocket(*pSock);
#ifdef _WIN32
		WSACleanup();
#endif // _WIN32
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#ifdef _MSC_VER
// Disable some Visual Studio warnings.
#pragma warning(disable : 4127) 
#endif // _MSC_VER
/*
Wait for a client for an IPv4 UDP server.

SOCKET socksrv : (IN) Server socket.
SOCKET* pSockCli : (INOUT) Valid pointer that will receive the client socket.
int timeout : (IN) Max time to wait in ms (0 to disable timeout).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int waitforcliforudpsrv(SOCKET socksrv, SOCKET* pSockCli, int timeout)
{
	fd_set sock_set;
	int iResult = SOCKET_ERROR;
	struct timeval tv;
	//struct sockaddr_storage addr;
	//socklen_t addrlen = sizeof(struct sockaddr_storage);
	//char hostname[NI_MAXHOST];
	//char service[NI_MAXSERV];
	//char buf[1];

	tv.tv_sec = (long)(timeout/1000);
	tv.tv_usec = (long)((timeout%1000)*1000);

	*pSockCli = INVALID_SOCKET;

	// Initialize a fd_set and add the server socket to it.
	FD_ZERO(&sock_set); 
	FD_SET(socksrv, &sock_set);

	// Wait for the readability of the socket in the fd_set, with a timeout.
	if (timeout != 0)
	{
		iResult = select((int)socksrv+1, &sock_set, NULL, NULL, &tv);
	}
	else
	{
		iResult = select((int)socksrv+1, &sock_set, NULL, NULL, NULL);
	}

	// Remove the server socket from the set.
	// No need to use FD_ISSET() here, as we only have one socket the return value of select() is 
	// sufficient to know what happened.
	FD_CLR(socksrv, &sock_set); 

	if (iResult == SOCKET_ERROR)
	{
		printf("select() failed.\n");
		return EXIT_FAILURE;
	}

	if (iResult == 0)
	{ 
		printf("select() timed out.\n");
		return EXIT_TIMEOUT;
	}
	/*
	recvfrom(socksrv, buf, 0, 0, (struct sockaddr*)&addr, &addrlen);

	// Connect to client.
	if (connect(socksrv, (struct sockaddr*)&addr, addrlen) != EXIT_SUCCESS)
	{
	printf("connect() failed.\n");
	return EXIT_FAILURE;
	}

	// Display hostname and service.
	if (getnameinfo((struct sockaddr*)&addr, addrlen, hostname, NI_MAXHOST, service,
	NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV) != EXIT_SUCCESS)
	{
	printf("getnameinfo() failed.\n");
	return EXIT_FAILURE;
	}

	printf("Accepted connection from host %s and port %s.\n", hostname, service);
	*/
	printf("Accepted connection.\n");

	*pSockCli = socksrv;

	return EXIT_SUCCESS;
}

/*
Wait for a client for an IPv4 TCP server.

SOCKET socksrv : (IN) Server socket.
SOCKET* pSockCli : (INOUT) Valid pointer that will receive the client socket.
int timeout : (IN) Max time to wait in ms (0 to disable timeout).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int waitforclifortcpsrv(SOCKET socksrv, SOCKET* pSockCli, int timeout)
{
	fd_set sock_set;
	int iResult = SOCKET_ERROR;
	struct timeval tv;
	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(struct sockaddr_storage);
	char hostname[NI_MAXHOST];
	char service[NI_MAXSERV];

	tv.tv_sec = (long)(timeout/1000);
	tv.tv_usec = (long)((timeout%1000)*1000);

	*pSockCli = INVALID_SOCKET;

	// Initialize a fd_set and add the server socket to it.
	FD_ZERO(&sock_set); 
	FD_SET(socksrv, &sock_set);

	// Wait for the readability of the socket in the fd_set, with a timeout.
	if (timeout != 0)
	{
		iResult = select((int)socksrv+1, &sock_set, NULL, NULL, &tv);
	}
	else
	{
		iResult = select((int)socksrv+1, &sock_set, NULL, NULL, NULL);
	}

	// Remove the server socket from the set.
	// No need to use FD_ISSET() here, as we only have one socket the return value of select() is 
	// sufficient to know what happened.
	FD_CLR(socksrv, &sock_set); 

	if (iResult == SOCKET_ERROR)
	{
		printf("select() failed.\n");
		return EXIT_FAILURE;
	}

	if (iResult == 0)
	{ 
		printf("select() timed out.\n");
		return EXIT_TIMEOUT;
	}

	*pSockCli = accept(socksrv, (struct sockaddr*)&addr, &addrlen);
	if (*pSockCli == INVALID_SOCKET)
	{
		printf("accept() failed.\n");
		return EXIT_FAILURE;
	}

	// Display hostname and service.
	if (getnameinfo((struct sockaddr*)&addr, addrlen, hostname, NI_MAXHOST, service,
		NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV) != EXIT_SUCCESS)
	{
		printf("getnameinfo() failed.\n");
		return EXIT_FAILURE;
	}

	printf("Accepted connection from host %s and port %s.\n", hostname, service);

	return EXIT_SUCCESS;
}
#ifdef _MSC_VER
// Restore the Visual Studio warnings previously disabled.
#pragma warning(default : 4127) 
#endif // _MSC_VER

/*
Disconnect a client from an IPv4 UDP server.

SOCKET sock : (IN) Client socket to disconnect from the server.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int disconnectclifromudpsrv(SOCKET sock)
{
	// Shutdown the connection.
	if (shutdown(sock, SD_BOTH) != EXIT_SUCCESS)
	{
		printf("shutdown() failed.\n");
		//return EXIT_FAILURE;
	}

	// Destroy the socket created by socket().
	if (closesocket(sock) != EXIT_SUCCESS)
	{
		printf("closesocket() failed.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Disconnect a client from an IPv4 TCP server.

SOCKET sock : (IN) Client socket to disconnect from the server.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int disconnectclifromtcpsrv(SOCKET sock)
{
	// Shutdown the connection.
	if (shutdown(sock, SD_BOTH) != EXIT_SUCCESS)
	{
		printf("shutdown() failed.\n");
		//return EXIT_FAILURE;
	}

	// Destroy the socket created by socket().
	if (closesocket(sock) != EXIT_SUCCESS)
	{
		printf("closesocket() failed.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Stop an IPv4 UDP server.

SOCKET sock : (IN) Server socket.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int releaseudpsrv(SOCKET sock)
{
	// Shutdown the connection.
	if (shutdown(sock, SD_BOTH) != EXIT_SUCCESS)
	{
		printf("shutdown() failed.\n");
		//return EXIT_FAILURE;
	}

	// Destroy the socket created by socket().
	if (closesocket(sock) != EXIT_SUCCESS)
	{
		printf("closesocket() failed.\n");
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	// Terminate use of the Winsock 2 DLL (Ws2_32.dll).
	if (WSACleanup() != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	return EXIT_SUCCESS;
}

/*
Stop an IPv4 TCP server.

SOCKET sock : (IN) Server socket.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int releasetcpsrv(SOCKET sock)
{
	// Shutdown the connection.
	if (shutdown(sock, SD_BOTH) != EXIT_SUCCESS)
	{
		printf("shutdown() failed.\n");
		//return EXIT_FAILURE;
	}

	// Destroy the socket created by socket().
	if (closesocket(sock) != EXIT_SUCCESS)
	{
		printf("closesocket() failed.\n");
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	// Terminate use of the Winsock 2 DLL (Ws2_32.dll).
	if (WSACleanup() != EXIT_SUCCESS)
	{
		printf("WSAStartup() failed.\n");
		return EXIT_FAILURE;
	}
#endif // _WIN32

	return EXIT_SUCCESS;
}

#ifdef _MSC_VER
// Disable some Visual Studio warnings.
#pragma warning(disable : 4127) 
#pragma warning(disable : 4702) 
#endif // _MSC_VER
int LaunchUDPSrv(char* port, int (*handlecli)(SOCKET, void*), void* pParam)
{
	int iResult = EXIT_FAILURE;
	SOCKET socksrv = INVALID_SOCKET;
	SOCKET sockcli = INVALID_SOCKET;

	if (initudpsrv(&socksrv, "0.0.0.0", port, DEFAULT_SOCK_TIMEOUT) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	for (;;)
	{
		iResult = waitforcliforudpsrv(socksrv, &sockcli, DEFAULT_SOCK_TIMEOUT);
		switch (iResult)
		{
		case EXIT_SUCCESS:
			if (handlecli(sockcli, pParam) != EXIT_SUCCESS)
			{
				printf("Error while communicating with the client.\n");
			}
			if (disconnectclifromudpsrv(sockcli) != EXIT_SUCCESS)
			{
				releaseudpsrv(socksrv);
				return EXIT_FAILURE;
			}
			break;
		case EXIT_TIMEOUT:
			break;
		default:
			releaseudpsrv(socksrv);
			return EXIT_FAILURE;
		}
	}

	if (releaseudpsrv(socksrv) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int LaunchSingleCliTCPSrv(char* port, int (*handlecli)(SOCKET, void*), void* pParam)
{
	int iResult = EXIT_FAILURE;
	SOCKET socksrv = INVALID_SOCKET;
	SOCKET sockcli = INVALID_SOCKET;

	if (inittcpsrv(&socksrv, "0.0.0.0", port, 1, DEFAULT_SOCK_TIMEOUT) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	for (;;)
	{
		iResult = waitforclifortcpsrv(socksrv, &sockcli, DEFAULT_SOCK_TIMEOUT);
		switch (iResult)
		{
		case EXIT_SUCCESS:
			if (handlecli(sockcli, pParam) != EXIT_SUCCESS)
			{
				printf("Error while communicating with the client.\n");
			}
			if (disconnectclifromtcpsrv(sockcli) != EXIT_SUCCESS)
			{
				releasetcpsrv(socksrv);
				return EXIT_FAILURE;
			}
			break;
		case EXIT_TIMEOUT:
			break;
		default:
			releasetcpsrv(socksrv);
			return EXIT_FAILURE;
		}
	}

	if (releasetcpsrv(socksrv) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
#ifdef _MSC_VER
// Restore the Visual Studio warnings previously disabled.
#pragma warning(default : 4127) 
#pragma warning(default : 4702) 
#endif // _MSC_VER

char* FindNMEASentence(char sentencebegin[7], char* str)
{
	char* ptr = NULL;
	char* foundstr = NULL;

	ptr = strstr(str, sentencebegin);
	if (!ptr)
	{
		// Could not find the beginning of the sentence.
		return NULL;
	}

	// Save the position of the beginning of the sentence.
	foundstr = ptr;

	// Check if the sentence is complete.
	ptr = strstr(foundstr+strlen(sentencebegin), "\r");
	if (!ptr)
	{
		// The sentence is incomplete.
		return NULL;
	}

	return foundstr;
}

char* FindLatestNMEASentence(char sentencebegin[7], char* str)
{
	char* ptr = NULL;
	char* foundstr = NULL;

	ptr = FindNMEASentence(sentencebegin, str);
	while (ptr) 
	{
		// Save the position of the beginning of the sentence.
		foundstr = ptr;

		// Search just after the beginning of the sentence.
		ptr = FindNMEASentence(sentencebegin, foundstr+strlen(sentencebegin));
	}

	return foundstr;
}

void ComputeNMEAchecksum(char* sentence, char checksum[4])
{
	int i = 0;
	char res = 0;

	memset(checksum, 0, sizeof(checksum));
	while (sentence[i])
	{
		if (sentence[i] == '$')
		{
			i++;
			continue;
		}
		if (sentence[i] == '*')
		{
			break;
		}
		res ^= sentence[i];
		i++;
	}

	sprintf(checksum, "*%02x", (int)res);
}

/*
Wait some time...

long Milliseconds : (IN) Time to wait in ms.

Return : Nothing.
*/
void mSleep(long Milliseconds)
{
#ifdef _WIN32
	Sleep(Milliseconds);
#else 
	// usleep() is considered as obsolete.
	//usleep(Milliseconds*1000);
	struct timespec req;

	req.tv_sec = Milliseconds/1000; // Seconds.
	req.tv_nsec = (Milliseconds%1000)*1000000; // Additional nanoseconds.
	nanosleep(&req, NULL);
#endif // _WIN32
}

#ifdef _WIN32
/*
Start a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int StartChrono(CHRONO* pChrono)
{
	if (!QueryPerformanceFrequency(&pChrono->Frequency))
	{
		printf("QueryPerformanceFrequency() failed.\n");
		return EXIT_FAILURE;
	}

	if (!QueryPerformanceCounter(&pChrono->Start))
	{
		printf("QueryPerformanceCounter() failed.\n");
		return EXIT_FAILURE;
	}

	pChrono->Finish.QuadPart = 0;
	pChrono->Duration.QuadPart = 0;
	pChrono->Suspended = 0;

	return EXIT_SUCCESS;
}

/*
Suspend a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.
double* pDuration : (INOUT) Duration (in s).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int SuspendChrono(CHRONO* pChrono, double* pDuration)
{
	LARGE_INTEGER temp;

	if (!pChrono->Suspended)
	{
		pChrono->Suspended = 1;
		if (!QueryPerformanceCounter(&temp))
		{
			printf("QueryPerformanceCounter() failed.\n");
			return EXIT_FAILURE;
		}

		//pChrono->Duration += pChrono->Finish-pChrono->Start
		pChrono->Finish = temp;
		temp.QuadPart += pChrono->Duration.QuadPart-pChrono->Start.QuadPart;
		pChrono->Duration = temp;
		*pDuration = (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
	else
	{
		printf("Chrono already suspended.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Suspend a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : Duration (in s).
*/
double SuspendChronoQuick(CHRONO* pChrono)
{
	LARGE_INTEGER temp;

	if (!pChrono->Suspended)
	{
		pChrono->Suspended = 1;
		QueryPerformanceCounter(&temp);

		//pChrono->Duration += pChrono->Finish-pChrono->Start
		pChrono->Finish = temp;
		temp.QuadPart += pChrono->Duration.QuadPart-pChrono->Start.QuadPart;
		pChrono->Duration = temp;
		return (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
	else
	{
		printf("Chrono already suspended.\n");
		return -1;
	}
}

/*
Resume a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int ResumeChrono(CHRONO* pChrono)
{
	if (pChrono->Suspended)
	{
		pChrono->Suspended = 0;
		if (!QueryPerformanceCounter(&pChrono->Start))
		{
			printf("QueryPerformanceCounter() failed.\n");
			return EXIT_FAILURE;
		}
	}
	else
	{
		printf("Chrono not suspended.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Get the time elapsed of a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.
double* pDuration : (INOUT) Duration (in s).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int GetTimeElapsedChrono(CHRONO* pChrono, double* pDuration)
{
	LARGE_INTEGER temp;

	if (pChrono->Suspended)
	{
		*pDuration = (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
	else
	{
		if (!QueryPerformanceCounter(&temp))
		{
			printf("QueryPerformanceCounter() failed.\n");
			return EXIT_FAILURE;
		}

		//*pDuration = pChrono->Duration+temp-pChrono->Start
		temp.QuadPart += pChrono->Duration.QuadPart-pChrono->Start.QuadPart;
		*pDuration = (double)temp.QuadPart/(double)pChrono->Frequency.QuadPart;
	}

	return EXIT_SUCCESS;
}

/*
Get the time elapsed of a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : Duration (in s).
*/
double GetTimeElapsedChronoQuick(CHRONO* pChrono)
{
	LARGE_INTEGER temp;

	if (pChrono->Suspended)
	{
		return (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
	else
	{
		QueryPerformanceCounter(&temp);

		//*pDuration = pChrono->Duration+temp-pChrono->Start
		temp.QuadPart += pChrono->Duration.QuadPart-pChrono->Start.QuadPart;
		return (double)temp.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
}

/*
Stop a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.
double* pDuration : (INOUT) Duration (in s).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int StopChrono(CHRONO* pChrono, double* pDuration)
{
	LARGE_INTEGER temp;

	if (pChrono->Suspended)
	{
		*pDuration = (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
	else
	{
		if (!QueryPerformanceCounter(&temp))
		{
			printf("QueryPerformanceCounter() failed.\n");
			return EXIT_FAILURE;
		}

		//pChrono->Duration += pChrono->Finish-pChrono->Start
		pChrono->Finish = temp;
		temp.QuadPart += pChrono->Duration.QuadPart-pChrono->Start.QuadPart;
		pChrono->Duration = temp;
		*pDuration = (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}

	return EXIT_SUCCESS;
}

/*
Stop a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : Duration (in s).
*/
double StopChronoQuick(CHRONO* pChrono)
{
	LARGE_INTEGER temp;

	if (pChrono->Suspended)
	{
		return (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
	else
	{
		QueryPerformanceCounter(&temp);

		//pChrono->Duration += pChrono->Finish-pChrono->Start
		pChrono->Finish = temp;
		temp.QuadPart += pChrono->Duration.QuadPart-pChrono->Start.QuadPart;
		pChrono->Duration = temp;
		return (double)pChrono->Duration.QuadPart/(double)pChrono->Frequency.QuadPart;
	}
}
#else 
/*
Start a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int StartChrono(CHRONO* pChrono)
{
	if (clock_getres(CLOCK_MONOTONIC_RAW, &pChrono->Frequency) != EXIT_SUCCESS)
	{
		printf("clock_getres() failed.\n");
		return EXIT_FAILURE;
	}

	if (clock_gettime(CLOCK_MONOTONIC_RAW, &pChrono->Start) != EXIT_SUCCESS)
	{
		printf("clock_gettime() failed.\n");
		return EXIT_FAILURE;
	}

	pChrono->Finish.tv_sec = 0;
	pChrono->Finish.tv_nsec = 0;
	pChrono->Duration.tv_sec = 0;
	pChrono->Duration.tv_nsec = 0;
	pChrono->Suspended = 0;

	return EXIT_SUCCESS;
}

/*
Suspend a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.
double* pDuration : (INOUT) Duration (in s).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int SuspendChrono(CHRONO* pChrono, double* pDuration)
{
	struct timespec temp;

	if (!pChrono->Suspended)
	{
		pChrono->Suspended = 1;
		if (clock_gettime(CLOCK_MONOTONIC_RAW, &temp) != EXIT_SUCCESS)
		{
			printf("clock_gettime() failed.\n");
			return EXIT_FAILURE;
		}

		//pChrono->Duration += pChrono->Finish-pChrono->Start

		pChrono->Finish = temp;

		temp.tv_sec += pChrono->Duration.tv_sec; // Seconds.
		temp.tv_nsec += pChrono->Duration.tv_nsec; // Additional nanoseconds.
		// tv_nsec must stay in [0,1000000000].
		temp.tv_sec += temp.tv_nsec/1000000000;
		temp.tv_nsec = temp.tv_nsec%1000000000;

		temp.tv_sec -= pChrono->Start.tv_sec;
		temp.tv_nsec -= pChrono->Start.tv_nsec;
		// tv_nsec must stay in [0,1000000000].
		if (temp.tv_nsec < 0)
		{
			temp.tv_sec += -1+temp.tv_nsec/1000000000;
			temp.tv_nsec = 1000000000+temp.tv_nsec%1000000000;
		}

		pChrono->Duration = temp;

		*pDuration = pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}
	else
	{
		printf("Chrono already suspended.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Suspend a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : Duration (in s).
*/
double SuspendChronoQuick(CHRONO* pChrono)
{
	struct timespec temp;

	if (!pChrono->Suspended)
	{
		pChrono->Suspended = 1;
		clock_gettime(CLOCK_MONOTONIC_RAW, &temp);

		//pChrono->Duration += pChrono->Finish-pChrono->Start

		pChrono->Finish = temp;

		temp.tv_sec += pChrono->Duration.tv_sec; // Seconds.
		temp.tv_nsec += pChrono->Duration.tv_nsec; // Additional nanoseconds.
		// tv_nsec must stay in [0,1000000000].
		temp.tv_sec += temp.tv_nsec/1000000000;
		temp.tv_nsec = temp.tv_nsec%1000000000;

		temp.tv_sec -= pChrono->Start.tv_sec;
		temp.tv_nsec -= pChrono->Start.tv_nsec;
		// tv_nsec must stay in [0,1000000000].
		if (temp.tv_nsec < 0)
		{
			temp.tv_sec += -1+temp.tv_nsec/1000000000;
			temp.tv_nsec = 1000000000+temp.tv_nsec%1000000000;
		}

		pChrono->Duration = temp;

		return pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}
	else
	{
		printf("Chrono already suspended.\n");
		return -1;
	}
}

/*
Resume a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int ResumeChrono(CHRONO* pChrono)
{
	if (pChrono->Suspended)
	{
		pChrono->Suspended = 0;
		if (clock_gettime(CLOCK_MONOTONIC_RAW, &pChrono->Start) != EXIT_SUCCESS)
		{
			printf("clock_gettime() failed.\n");
			return EXIT_FAILURE;
		}
	}
	else
	{
		printf("Chrono not suspended.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/*
Get the time elapsed of a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.
double* pDuration : (INOUT) Duration (in s).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int GetTimeElapsedChrono(CHRONO* pChrono, double* pDuration)
{
	struct timespec temp;

	if (pChrono->Suspended)
	{
		*pDuration = pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}
	else
	{
		if (clock_gettime(CLOCK_MONOTONIC_RAW, &temp) != EXIT_SUCCESS)
		{
			printf("clock_gettime() failed.\n");
			return EXIT_FAILURE;
		}

		//*pDuration = pChrono->Duration+temp-pChrono->Start

		temp.tv_sec += pChrono->Duration.tv_sec; // Seconds.
		temp.tv_nsec += pChrono->Duration.tv_nsec; // Additional nanoseconds.
		// tv_nsec must stay in [0,1000000000].
		temp.tv_sec += temp.tv_nsec/1000000000;
		temp.tv_nsec = temp.tv_nsec%1000000000;

		temp.tv_sec -= pChrono->Start.tv_sec;
		temp.tv_nsec -= pChrono->Start.tv_nsec;
		// tv_nsec must stay in [0,1000000000].
		if (temp.tv_nsec < 0)
		{
			temp.tv_sec += -1+temp.tv_nsec/1000000000;
			temp.tv_nsec = 1000000000+temp.tv_nsec%1000000000;
		}

		*pDuration = temp.tv_sec+(double)temp.tv_nsec/1000000000.0;
	}

	return EXIT_SUCCESS;
}

/*
Get the time elapsed of a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : Duration (in s).
*/
double GetTimeElapsedChronoQuick(CHRONO* pChrono)
{
	struct timespec temp;

	if (pChrono->Suspended)
	{
		return pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}
	else
	{
		clock_gettime(CLOCK_MONOTONIC_RAW, &temp);

		//*pDuration = pChrono->Duration+temp-pChrono->Start

		temp.tv_sec += pChrono->Duration.tv_sec; // Seconds.
		temp.tv_nsec += pChrono->Duration.tv_nsec; // Additional nanoseconds.
		// tv_nsec must stay in [0,1000000000].
		temp.tv_sec += temp.tv_nsec/1000000000;
		temp.tv_nsec = temp.tv_nsec%1000000000;

		temp.tv_sec -= pChrono->Start.tv_sec;
		temp.tv_nsec -= pChrono->Start.tv_nsec;
		// tv_nsec must stay in [0,1000000000].
		if (temp.tv_nsec < 0)
		{
			temp.tv_sec += -1+temp.tv_nsec/1000000000;
			temp.tv_nsec = 1000000000+temp.tv_nsec%1000000000;
		}

		return temp.tv_sec+(double)temp.tv_nsec/1000000000.0;
	}
}

/*
Stop a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.
double* pDuration : (INOUT) Duration (in s).

Return : EXIT_SUCCESS or EXIT_FAILURE if there is an error.
*/
int StopChrono(CHRONO* pChrono, double* pDuration)
{
	struct timespec temp;

	if (pChrono->Suspended)
	{
		*pDuration = pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}
	else
	{
		if (clock_gettime(CLOCK_MONOTONIC_RAW, &temp) != EXIT_SUCCESS)
		{
			printf("clock_gettime() failed.\n");
			return EXIT_FAILURE;
		}

		//pChrono->Duration += pChrono->Finish-pChrono->Start

		pChrono->Finish = temp;

		temp.tv_sec += pChrono->Duration.tv_sec; // Seconds.
		temp.tv_nsec += pChrono->Duration.tv_nsec; // Additional nanoseconds.
		// tv_nsec must stay in [0,1000000000].
		temp.tv_sec += temp.tv_nsec/1000000000;
		temp.tv_nsec = temp.tv_nsec%1000000000;

		temp.tv_sec -= pChrono->Start.tv_sec;
		temp.tv_nsec -= pChrono->Start.tv_nsec;
		// tv_nsec must stay in [0,1000000000].
		if (temp.tv_nsec < 0)
		{
			temp.tv_sec += -1+temp.tv_nsec/1000000000;
			temp.tv_nsec = 1000000000+temp.tv_nsec%1000000000;
		}

		pChrono->Duration = temp;

		*pDuration = pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}

	return EXIT_SUCCESS;
}

/*
Stop a chronometer.

CHRONO* pChrono : (INOUT) Valid pointer to the structure.

Return : Duration (in s).
*/
double StopChronoQuick(CHRONO* pChrono)
{
	struct timespec temp;

	if (pChrono->Suspended)
	{
		return pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}
	else
	{
		clock_gettime(CLOCK_MONOTONIC_RAW, &temp);

		//pChrono->Duration += pChrono->Finish-pChrono->Start

		pChrono->Finish = temp;

		temp.tv_sec += pChrono->Duration.tv_sec; // Seconds.
		temp.tv_nsec += pChrono->Duration.tv_nsec; // Additional nanoseconds.
		// tv_nsec must stay in [0,1000000000].
		temp.tv_sec += temp.tv_nsec/1000000000;
		temp.tv_nsec = temp.tv_nsec%1000000000;

		temp.tv_sec -= pChrono->Start.tv_sec;
		temp.tv_nsec -= pChrono->Start.tv_nsec;
		// tv_nsec must stay in [0,1000000000].
		if (temp.tv_nsec < 0)
		{
			temp.tv_sec += -1+temp.tv_nsec/1000000000;
			temp.tv_nsec = 1000000000+temp.tv_nsec%1000000000;
		}

		pChrono->Duration = temp;

		return pChrono->Duration.tv_sec+(double)pChrono->Duration.tv_nsec/1000000000.0;
	}
}
#endif // _WIN32

/*
Wait for the user to press any key until a specified timeout.

unsigned int timeout : (IN) Time to wait to get at least 1 char in ms (with a 
precision of tenths of s, max is MAX_TIMEOUT_PROMPTGETUSERINPUTTIMEOUT).

Return : The first character pressed.
*/
char GetUserInputTimeout(unsigned int timeout)
{
#ifdef _WIN32
	HANDLE hStdin = INVALID_HANDLE_VALUE;
	DWORD fdwMode = 0, fdwSaveOldMode = 0;
	INPUT_RECORD irInBuf[1]; 
	DWORD cNumRead = 0;
	//char buf[32];
	//DWORD dwReadBytes = 0;
	CHRONO chrono;
	double duration = 0;

	hStdin = CreateFile("CONIN$", 
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		NULL, // No security attributes.
		OPEN_EXISTING, // Must use OPEN_EXISTING.
		0,    // Ignored for console.
		NULL  // Ignored for console.
		);

	//hStdin = GetStdHandle(STD_INPUT_HANDLE); 

	GetConsoleMode(hStdin, &fdwSaveOldMode);

	//fdwMode = fdwSaveOldMode&~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT);
	fdwMode = ENABLE_PROCESSED_INPUT;

	SetConsoleMode(hStdin, fdwMode);

	FlushConsoleInputBuffer(hStdin);

	memset(irInBuf, 0, 1*sizeof(INPUT_RECORD));
	//memset(buf, 0, 32);

	//if (WaitForSingleObject(hStdin, (DWORD)timeout) == WAIT_OBJECT_0)
	//	ReadConsoleInput(hStdin, irInBuf, 1, &cNumRead);
	//	//ReadFile(hStdin, buf, 32, &dwReadBytes, NULL);

	StartChrono(&chrono);
	while (irInBuf->EventType != KEY_EVENT)
	{
		memset(irInBuf, 0, 1*sizeof(INPUT_RECORD));
		GetTimeElapsedChrono(&chrono, &duration);
		if ((int)timeout-(int)(duration*1000) > 0)
		{
			// min to try to handle also wrap around of the time in the integer values...
			timeout = (unsigned int)min((int)timeout-(int)(duration*1000), (int)timeout);
		}
		else
		{
			// We reached the timeout.
			break;
		}
		if (WaitForSingleObject(hStdin, (DWORD)timeout) == WAIT_OBJECT_0)
		{
			//PRINT_DEBUG_MESSAGE_OSMISC(("%s. \n", GetLastErrorMsg()));
			ReadConsoleInput(hStdin, irInBuf, 1, &cNumRead);
		}
	}
	StopChrono(&chrono, &duration);

	SetConsoleMode(hStdin, fdwSaveOldMode);

	//FlushConsoleInputBuffer(hStdin);

	CloseHandle(hStdin);

	return (char)irInBuf[0].Event.KeyEvent.uChar.AsciiChar;
	//return buf[0];
#else 
	struct termios term, back;
	char buf[32];
	ssize_t nbbytes = 0;

	tcgetattr(STDIN_FILENO, &term); // Get the stdin terminal information.
	memcpy(&back, &term, sizeof(term)); // Save it.

	term.c_lflag &= ~(ICANON|ECHO); // Modify terminal options.
	term.c_cc[VTIME] = timeout/100; // Time to wait for every character read in tenths of seconds.
	term.c_cc[VMIN] = 0; // Minimum number of characters to read. 

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term); // Modify stdin attributes.

	tcflush(STDIN_FILENO, TCIFLUSH); // Flush any previous input on stdin.

	memset(buf, 0, 32);

	nbbytes = read(STDIN_FILENO, buf, 32); // Wait for a character.

	if (nbbytes == 0) buf[0] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &back); // Restore previous attributes.

	return buf[0];
#endif // _WIN32
}

/*
Return a line of a file using fgets(), skipping lines that begin with a '%' 
(Scilab-style comments), a '#' (Linux configuration files-style comments) or 
"//" (C-style comments). 
Return NULL when a line begin with a '$' or when fgets() return NULL, or if 
the maximum number of characters to read is less than 2.

FILE* file : (IN) Pointer to a file.
char* line : (IN) Storage location for data.
int nbChar : (IN) Maximum number of characters to read.

Return : The line or NULL.
*/
char* fgets3(FILE* file, char* line, int nbChar)
{ 
	char* r = NULL;

	if (nbChar < 2)
	{
		return NULL;
	}

	do   
	{
		r = fgets(line, nbChar, file);
	} 
	while ((
		(line[0] == '%')||
		(line[0] == '#')||
		((line[0] == '/')&&(line[1] == '/'))
		) && (r != NULL));

	if (line[0] == '$')
	{
		r = NULL;
	}

	return r;
}

char strftime_m_tmpbuf[32]; // Used to store the string returned by strtime_m().
char strftime_fns_tmpbuf[32]; // Used to store the string returned by strtime_fns().

/*
Return a string like ctime() but in this format :

Mon Aug 27 19:28:04 2007\0

(without the "\n" of ctime()).
Should not be used in concurrent threads as the string value returned might 
be changed by another thread.

Return : This string.
*/
char* strtime_m(void)
{
	time_t t;
	struct tm *timeptr = NULL;

	time(&t);
	timeptr = localtime(&t);

	if (timeptr == NULL)
	{
		return NULL;
	}

	// Use strftime to build a customized time string. 
	if (strftime(
		strftime_m_tmpbuf,
		32,
		"%a %b %d %H:%M:%S %Y",
		timeptr
		) <= 0)
	{
		return NULL;
	}

	return strftime_m_tmpbuf;
}

/*
Return a string like ctime() but in this format :

Mon_Aug_27_19h28min04sec_2007\0

(without the "\n", ":", " " of ctime() in order to be safely used in filenames).
Should not be used in concurrent threads as the string value returned might 
be changed by another thread.

Return : This string.
*/
char* strtime_fns(void)	
{
	time_t t;
	struct tm *timeptr = NULL;

	time(&t);
	timeptr = localtime(&t);

	if (timeptr == NULL)
	{
		return NULL;
	}

	// Use strftime to build a customized time string. 
	if (strftime(
		strftime_fns_tmpbuf,
		32,
		"%a_%b_%d_%Hh%Mmin%Ssec_%Y",
		timeptr
		) <= 0)
	{
		return NULL;
	}

	return strftime_fns_tmpbuf;
}

void DecSec2DaysHoursMinSec(double decsec, int* pDays, int* pHours, int* pMin, int* pSec, double* pDeccsec)
{
	int hours = 0, minutes = 0, seconds = 0; 

	seconds = (int)decsec;
	*pSec = seconds%60;
	minutes = seconds/60;
	*pMin = minutes%60;
	hours = minutes/60;
	*pHours = hours%24;
	*pDays = hours/24;

	*pDeccsec = (decsec-seconds)*100.0;
}

/*
Compute the square of a value.

double x : (IN) Value.

Return : The square of x.
*/
double sqr(double x)
{
	return x*x;
}

/*
Return x/epsilon if x is between -epsilon and epsilon or -1 if x is negative, 
+1 if x is positive.

double x : (IN) Value.
double epsilon : (IN) Threshold.

Return : -1, +1 or x/epsilon.
*/
double sign(double x, double epsilon)
{ 
	if (x >= epsilon) 
		return 1;
	else if (x <= -epsilon) 
		return -1;
	else 
		return x/epsilon;
}

/*
Return an angle between -M_PI and M_PI.

double theta : (IN) Value.

Return : The converted angle.
*/
double fmod_2PI(double theta)
{
	return fmod(fmod(theta, 2*M_PI)+3*M_PI, 2*M_PI)-M_PI;
}

/*
Calculation of CRC-16 checksum over an amount of bytes in the serial buffer.
The calculation is done without the 2 bytes from crc16 (receive-mode).

unsigned char* SC_Buffer : (IN) Buffer from the serial interface.
int SC_Amount : (IN) Amount of bytes which are received or should be transmitted (without CRC16).
unsigned char* crc_h : (INOUT) Valid pointer that should receive the MSB of the CRC-16.
unsigned char* crc_l: (INOUT) Valid pointer that should receive the LSB of the CRC-16.

Return : Nothing.
*/
void CalcCRC16(unsigned char* SC_Buffer, int SC_Amount, unsigned char* crc_h, unsigned char* crc_l)
{
	// Locals.
	unsigned int Crc;
	unsigned char n, m, x;

	// Initialization.
	Crc = 0xFFFF;
	m = (unsigned char)SC_Amount;
	x = 0;

	// Loop over all bits.
	while (m > 0)
	{
		Crc ^= SC_Buffer[x];
		for (n = 0; n < 8 ; n++)
		{
			if (Crc &1)
			{
				Crc >>= 1;
				Crc ^= 0xA001;
			}
			else
				Crc >>= 1;
		}
		m--;
		x++;
	}

	// Result.
	*crc_h = (unsigned char)((Crc>>8)&0xFF);
	*crc_l = (unsigned char)(Crc&0xFF);
}

void GPS2RefCoordSystem(double lat0, double long0, double latitude, double longitude, double* pX, double* pY)
{
	*pX = (M_PI/180.0)*EARTH_RADIUS*(longitude-long0)*cos((M_PI/180.0)*latitude);
	*pY = (M_PI/180.0)*EARTH_RADIUS*(latitude-lat0);
}

void RefCoordSystem2GPS(double lat0, double long0, double x, double y, double* pLatitude, double* pLongitude)
{
	*pLatitude = (y/(double)EARTH_RADIUS)*(180.0/M_PI)+lat0;
	if ((fabs(*pLatitude-90.0) < DBL_EPSILON)||(fabs(*pLatitude+90.0) < DBL_EPSILON))
	{
		*pLongitude = 0;
	}
	else
	{
		*pLongitude = (x/(double)EARTH_RADIUS)*(180.0/M_PI)/cos((M_PI/180.0)*(*pLatitude))+long0;
	}
}

void DecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin)
{
	*pDeg = (int)decdeg;
	*pDecMin = fabs((decdeg-*pDeg)*60.0);
}

void DecDeg2DegMinDecSec(double decdeg, int* pDeg, int* pMin, double* pDecSec)
{
	double decmin;
	DecDeg2DegDecMin(decdeg, pDeg, &decmin);
	*pMin = (int)decmin;
	*pDecSec = (decmin-*pMin)*60.0;
}

void DegDecMin2DecDeg(int deg, double decmin, double* pDecDeg)
{
	*pDecDeg = (deg >= 0)?deg+fabs(decmin/60.0):deg-fabs(decmin/60.0);
}

void DegMinDecSec2DecDeg(int deg, int min, double decsec, double* pDecDeg)
{
	double decmin = abs(min)+fabs(decsec)/60.0;
	DegDecMin2DecDeg(deg, decmin, pDecDeg);
}

void GPSLatitudeDecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin, char* pNorthOrSouth)
{
	DecDeg2DegDecMin(decdeg, pDeg, pDecMin);
	*pDeg = abs(*pDeg);
	*pNorthOrSouth = (decdeg >= 0)?'N':'S';
}

void GPSLongitudeDecDeg2DegDecMin(double decdeg, int* pDeg, double* pDecMin, char* pEastOrWest)
{
	DecDeg2DegDecMin(decdeg, pDeg, pDecMin);
	*pDeg = abs(*pDeg);
	*pEastOrWest = (decdeg >= 0)?'E':'W';
}

void GPSLatitudeDegDecMin2DecDeg(int deg, double decmin, char NorthOrSouth, double* pDecDeg)
{
	DegDecMin2DecDeg(abs(deg), fabs(decmin), pDecDeg);
	*pDecDeg = (NorthOrSouth == 'N')?*pDecDeg:-*pDecDeg;
}

void GPSLongitudeDegDecMin2DecDeg(int deg, double decmin, char EastOrWest, double* pDecDeg)
{
	DegDecMin2DecDeg(abs(deg), fabs(decmin), pDecDeg);
	*pDecDeg = (EastOrWest == 'E')?*pDecDeg:-*pDecDeg;
}

void GPSLatitudeDecDeg2DegMinDecSec(double val, int* pDeg, int* pMin, double* pDecSec, char* pNorthOrSouth)
{
	DecDeg2DegMinDecSec(val, pDeg, pMin, pDecSec);
	*pDeg = abs(*pDeg);
	*pNorthOrSouth = (val >= 0)?'N':'S';
}

void GPSLongitudeDecDeg2DegMinDecSec(double val, int* pDeg, int* pMin, double* pDecSec, char* pEastOrWest)
{
	DecDeg2DegMinDecSec(val, pDeg, pMin, pDecSec);
	*pDeg = abs(*pDeg);
	*pEastOrWest = (val >= 0)?'E':'W';
}

void GPSLatitudeDegMinDecSec2DecDeg(int deg, int min, double decsec, char NorthOrSouth, double* pDecDeg)
{
	DegMinDecSec2DecDeg(abs(deg), abs(min), fabs(decsec), pDecDeg);
	*pDecDeg = (NorthOrSouth == 'N')?*pDecDeg:-*pDecDeg;
}

void GPSLongitudeDegMinDecSec2DecDeg(int deg, int min, double decsec, char EastOrWest, double* pDecDeg)
{
	DegMinDecSec2DecDeg(abs(deg), abs(min), fabs(decsec), pDecDeg);
	*pDecDeg = (EastOrWest == 'E')?*pDecDeg:-*pDecDeg;
}
