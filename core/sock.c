
#ifdef _WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include "tpipv6.h"
int close(int );
#pragma comment (lib, "ws2_32")

void init_windows_sock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return;
	}

	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) {
		WSACleanup( );
		return;
	}
}

#else

#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LEN 256
#define MAX_DATA_PAIR 10
#define MAX_ADDR_LEN  64


static struct data_pair{
	unsigned char data1[MAX_DATA_LEN];
	unsigned char data2[MAX_DATA_LEN];
}g_data_pair[MAX_DATA_PAIR] = {0};

static char addr[MAX_ADDR_LEN] = {0};
static int g_data_num = 0;
int convert_to_bin(char *buff, int maxlen, const char *data, int size, const char*infmt);


int parse_data_pair(const char *data, int isStr)
{
	char buff[1024] = {0};
	struct data_pair *pdata = &g_data_pair[g_data_num];
	char *temp = NULL;

	if(data == NULL || data[0] == 0) {
		g_data_num = 0;
		memset(&g_data_pair, 0, sizeof(g_data_pair));
		return 0;
	}

	while(*data == ' ') data++;
	strcpy(buff, data);
	temp = buff;

	if((temp = strtok(temp, "|")) == NULL) {
		return -1;
	}

	memset(pdata, 0, sizeof(struct data_pair));
	if(isStr) {
		pdata->data1[0] = strlen(temp)+1;
		strcpy(&pdata->data1[1], temp);
	} else {
		pdata->data1[0] = (unsigned char)convert_to_bin((char*)(pdata->data1+1), sizeof(pdata->data1)-1, temp, 3, "%02X ");
	}
	
	if(pdata->data1[0] == 0) {
		return -1;
	}

	g_data_num++;

	if((temp = strtok(NULL, "|")) == NULL) {
		return 0;
	}
	
	while(*temp == ' ') temp++;

	if(isStr) {
		pdata->data2[0] = strlen(temp)+1;
		strcpy(&pdata->data2[1], temp);
	} else {
		pdata->data2[0] = (unsigned char)convert_to_bin((char*)(pdata->data2+1), sizeof(pdata->data2)-1, temp, 3, "%02X ");
	}

	return 0;
}

const unsigned char *get_data_pair(const unsigned char *data, int len)
{
	int index = 0;

	for(index = 0; index < g_data_num; index++) {
		if(len == g_data_pair[index].data1[0] && memcmp(data, &g_data_pair[index].data1[1], len) == 0) {
			return g_data_pair[index].data2;
		}
	}

	return NULL;
}

int get_host_addr(void *addr, unsigned int addrlen, const char *host, unsigned short port)
{
	struct addrinfo hint = {0};
	struct addrinfo *addrlink = NULL;
	struct addrinfo *tmp = NULL;
	unsigned short *lpport = (unsigned short *)((char*)addr + 1 + sizeof(unsigned short));
	hint.ai_flags = AI_CANONNAME;

	if(getaddrinfo(host, NULL, &hint, &addrlink) != 0) {
		return -1;
	}

	for(tmp = addrlink; tmp != NULL; tmp = tmp->ai_next) {
		if(tmp->ai_family == AF_INET6) {
			if(IN6_IS_ADDR_MULTICAST(&((struct sockaddr_in6 *)tmp->ai_addr)->sin6_addr)
				|| IN6_IS_ADDR_LINKLOCAL(&((struct sockaddr_in6 *)tmp->ai_addr)->sin6_addr)
				|| IN6_IS_ADDR_SITELOCAL(&((struct sockaddr_in6 *)tmp->ai_addr)->sin6_addr)
				|| IN6_IS_ADDR_V4MAPPED(&((struct sockaddr_in6 *)tmp->ai_addr)->sin6_addr)
				|| IN6_IS_ADDR_V4COMPAT(&((struct sockaddr_in6 *)tmp->ai_addr)->sin6_addr) ) {
				continue;
			}
		} 

		((char*)addr)[0] = (char)(tmp->ai_addrlen > addrlen ? addrlen : tmp->ai_addrlen);
		memcpy((char*)addr+1, tmp->ai_addr, ((char*)addr)[0]);
		*lpport = htons(port);
		break;
	}

	freeaddrinfo(addrlink);
	return 0;
}

static int get_free_index(int array[], int cnt)
{
	int index = 0;
	for(index = 0; index < cnt; index++) {
		if(array[index] <= 0) {
			return index;
		}
	}

	return -1;
}


#define MAX_TCP_LINK 100
int server(int *isExit, unsigned short port, int type)
{
	struct sockaddr_in   *paddrv4 = (struct sockaddr_in*)addr;
	struct sockaddr_in6  *paddrv6 = (struct sockaddr_in6*)addr;
	char dst[MAX_ADDR_LEN] = {0};
	char buff[MAX_DATA_LEN] = {0};
	int len = 0, cnt = 0, index = 0;
	int dst_len = sizeof(dst);
	int sock = 0,sockv6 = 0, tmpsock = 0;
	fd_set fdset, tmpfs;
	struct timeval tm = {1, 0};
	int clients[MAX_TCP_LINK] = {0};
	const char *lptemp = NULL;

	if(port == 0) {
		return -1;
	}

	sock = socket(AF_INET, type, 0);
	sockv6 = socket(AF_INET6, type, 0);

	if(sock < 3 && sockv6 < 3) {
		return -1;
	}

#ifdef _WINDOWS
	{
		unsigned long isNonBlocked = 1;
		ioctlsocket(sock, FIONBIO, (unsigned long*)&isNonBlocked); 
		isNonBlocked = 1;
		ioctlsocket(sockv6, FIONBIO, (unsigned long*)&isNonBlocked); 
	}
#else
	fcntl(sock,F_SETFL,fcntl(sock,F_GETFL) | O_NONBLOCK);
	fcntl(sockv6,F_SETFL,fcntl(sockv6,F_GETFL) | O_NONBLOCK);
#endif

	FD_ZERO(&fdset);

	if(sock > 0) {
		memset(addr, 0, sizeof addr);
		paddrv4->sin_family = AF_INET;
		paddrv4->sin_port = htons(port);
		
		if(bind(sock, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) != 0) {
			return -1;
		}

		FD_SET(sock, &fdset);
	}

	if(sockv6 > 0) {
		memset(addr, 0, sizeof addr);
		paddrv6->sin6_family = AF_INET6;
		paddrv6->sin6_port = htons(port);
		bind(sockv6, (struct sockaddr*)addr, 0);
		FD_SET(sockv6, &fdset);
	}

	if(type == SOCK_STREAM) {
		listen(sock, 4);
		listen(sockv6, 4);
	}

	tmpfs = fdset;

	while(!(*isExit)) {
		tm.tv_sec = 1;
		tmpfs = fdset;
		cnt = select((sock>sockv6?sock:sockv6) + 1, &tmpfs, NULL, NULL, &tm);
		
		if(cnt <= 0) {
			continue;
		}

		if(FD_ISSET(sock, &tmpfs)) {
			tmpsock = sock;
		} else if(FD_ISSET(sockv6, &tmpfs)) {
			tmpsock = sockv6; 
		} else {
			for(index = 0; index < MAX_TCP_LINK; index++) {
				if(clients[index] > 0 && FD_ISSET(clients[index], &tmpfs)) {
					if((len = recv(clients[index], buff, sizeof buff, 0)) == 0) {
#ifdef _WINDOWS
						closesocket(clients[index]);
#else
						close(clients[index]);
#endif
						FD_CLR(clients[index], &fdset);
						clients[index] = 0;
						continue;
					}

					if((lptemp = get_data_pair(buff, len)) != NULL) {
						send(clients[index], lptemp+1, lptemp[0], 0);
					}
				}
			}

			continue;
		}

		if(type == SOCK_DGRAM) {
			while((len = recvfrom(tmpsock, buff, sizeof buff, 0, (struct sockaddr *)dst, &dst_len)) > 0) {
				const unsigned char * ptemp = get_data_pair(buff, len);
				if(ptemp != NULL) {
					sendto(tmpsock, ptemp+1, ptemp[0], 0, (struct sockaddr *)dst, dst_len);
				}
			}
		} else if(type == SOCK_STREAM) {
			index = get_free_index(clients, MAX_TCP_LINK);
			while(index >= 0 && (clients[index] = accept(tmpsock, (struct sockaddr *)dst, &dst_len)) > 0) {
				FD_SET(clients[index], &fdset);
				index = get_free_index(clients, MAX_TCP_LINK);
			}
		}
	}
#ifdef _WINDOWS
	closesocket(sock);
	closesocket(sockv6);
#else
	close(sock);
	close(sockv6);
#endif

	for(index = 0; index < MAX_TCP_LINK; index++) {
		if(clients[index] > 0) {
#ifdef _WINDOWS
			closesocket(clients[index]);
#else
			close(clients[index]);
#endif
		}
	}

	return 0;
}

int client(int sock_type, const void *host, unsigned short port, int isHdrIncl, int timeout)
{
	int sock = 0, ret = 0;
	char buff[MAX_DATA_LEN] = {0};
	int len = 0;
	int index = 0, grp = 0;
	struct timeval tim = {timeout, 0};

	if(((char*)host)[0] == 0 || get_host_addr(addr, sizeof(addr), host, port) != 0) {
		return -1;
	}

	if((sock = socket(*(unsigned short*)(addr+1), sock_type, 0)) < 3) {
	//if((sock = socket(AF_INET, sock_type, 0)) < 3) {
		return -1;
	}

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tim, sizeof(tim));
	if(sock_type == SOCK_STREAM) {
		if(connect(sock, (struct sockaddr *)(addr+1), addr[0]) != 0) {
			ret = -1;
			goto done;
		}
	}
	
	for(index = 0; index < g_data_num; index++) {
		len = sizeof buff;
		if(sock_type == SOCK_DGRAM) {
			char dst[MAX_ADDR_LEN] = {0};
			int dst_len = sizeof(dst);

			if((len = sendto(sock, g_data_pair[index].data1+1, g_data_pair[index].data1[0], 0, (struct sockaddr *)(addr+1), addr[0])) < 0)  {
				//return -1;
				ret = -1;
				goto done;
			}

			len = recvfrom(sock, buff, sizeof buff, 0,  (struct sockaddr *)dst, &dst_len);
		} else if(sock_type == SOCK_STREAM) {			
			if(send(sock, g_data_pair[index].data1+1, g_data_pair[index].data1[0], 0) < 0) {
				ret = -1;
				goto done;
			}
			
			len = recv(sock, buff, sizeof buff, 0);
		}

		if(g_data_pair[index].data2[0] == 0) {
			continue;
		}

		if(len != g_data_pair[index].data2[0] || memcmp(&g_data_pair[index].data2[1], buff, len) != 0) {
			ret = index+1;
			goto done; 
		}
	}
done:
#ifdef _WINDOWS
	closesocket(sock);
#else
	close(sock);
#endif
	return ret;
}

