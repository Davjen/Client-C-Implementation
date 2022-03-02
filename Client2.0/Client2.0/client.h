#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>


//EVERYTHING IS OVERSIZED->TO DO->TURN EVERYTHING DOWN

typedef struct CL_CHECK
{
	uint8_t packet_type;
	uint8_t version;

} CL_CHECK_T;

typedef struct SRV_CHECK
{
	int packet_type;
	int srv_answer;

}SRV_CHECK_T;

typedef struct CL_AUTH
{
	int packet_type;
	char username[18];
	char password[18];
	
}CL_AUTH_T;

typedef struct SRV_AUTH
{
	int packet_type;
	int srv_answer;
	int id;
}SRV_AUTH_T;

typedef struct CL_DISCONNECT 
{
	int packet_type;
	int id;
}CL_DISCONNECT_T;

typedef struct CL_PING
{
	int packet_type;
	int id;
}CL_PING_T;


int client_init();
int _set_timeout();
void send_packet(char* packet, int len);
void send_cl_check(uint8_t version);
void send_cl_auth(char* username, char* password);
void send_cl_disconnect(int id);
void send_cl_ping(int id);

int receive_data_from_server(int* ID);


