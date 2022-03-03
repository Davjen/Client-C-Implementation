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
	uint8_t packet_type;
	uint8_t srv_answer;

}SRV_CHECK_T;

typedef struct CL_AUTH
{
	uint8_t packet_type;
	char username[18];
	char password[18];
	
}CL_AUTH_T;

typedef struct SRV_AUTH
{
	uint8_t packet_type;
	uint8_t srv_answer;
	uint8_t id;
}SRV_AUTH_T;

typedef struct CL_DISCONNECT 
{
	uint8_t packet_type;
	uint8_t id;
}CL_DISCONNECT_T;

typedef struct CL_PING
{
	uint8_t packet_type;
	uint8_t id;
}CL_PING_T;

typedef struct SRV_PONG
{
	uint8_t packet_type;
}SRV_PONG_T;

int client_init();
int _set_timeout();
void send_packet(char* packet, int len);
void send_cl_check(uint8_t version);
void send_cl_auth(char* username, char* password);
void send_cl_disconnect();
void send_cl_ping();

int receive_data_from_server();


