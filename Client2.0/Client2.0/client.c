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

#include "client.h"
#include "OpCodes.h"
#include <time.h>
#include <SDL.h>

#define PUBLIC_SERVER "192.168.1.142"
#define SERVER_PORT 9999

int new_socket;
struct sockaddr_in sk_in;
uint8_t ID;
int client_init()
{
#ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll
    WSADATA wsa_data;
    if (WSAStartup(0x0202, &wsa_data))
    {
        return -1;
    }
#endif
    new_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if (new_socket < 0)
    {
        return -2;
    }
    inet_pton(AF_INET, PUBLIC_SERVER, &sk_in.sin_addr); //parse of server ip and turn it into ipv4
    sk_in.sin_family = AF_INET;
    sk_in.sin_port = htons(SERVER_PORT);

    _set_timeout();
    return 0;
}

int _set_timeout()
{
    unsigned int timeout = 5;
    if (setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(unsigned int)))
    {
        return -1;
    }

    int current_buffer_size = 0;
    int sockopt_len = sizeof(int);
    getsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, (char*)&current_buffer_size, &sockopt_len);

    int buffer_size = 8192;
    if (setsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, (char*)&buffer_size, sizeof(int)))
    {
        return -2;
    }

    if (setsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, (char*)&buffer_size, sizeof(int)))
    {
        return -3;
    }

    getsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, (char*)&current_buffer_size, &sockopt_len);

    return 0;
}

#pragma region SEND_PACKET
void send_packet(char* packet, int len)
{
    int sent_bytes = sendto(new_socket, packet, len, 0, (struct sockaddr*)&sk_in, sizeof(sk_in));
    char addr_as_string[64];
    inet_ntop(AF_INET, &sk_in.sin_addr, addr_as_string, 64);
}

void send_cl_check(uint8_t version)
{

    CL_CHECK_T packet = { CL_CHECK, version };
    send_packet(&packet, sizeof(CL_CHECK_T));
}

void send_cl_auth(const char *username, const char *password)
{
    //call hashing function to encrypt username and password
    //maybe separate ID and usr/psw auth if i have to chose a specific server

    int username_len = SDL_strlen(username);
    int password_len = SDL_strlen(password);

    CL_AUTH_T packet = { CL_AUTH };
    SDL_memcpy(packet.username, username, username_len);
    SDL_memcpy(packet.password, password, password_len);
    send_packet(&packet, sizeof(CL_AUTH_T));
}

void send_cl_disconnect()
{
    CL_DISCONNECT_T packet = { CL_DISCONNECT,ID };
    send_packet(&packet, sizeof(CL_DISCONNECT_T));
}

void send_cl_ping()
{
    //TO DO -> COUNTDOWN FUNCTION
    
    CL_PING_T packet = { CL_PING,ID };
    send_packet(&packet, sizeof(CL_PING_T));
}
#pragma endregion

#pragma region RECEIVE_PACKET

//TO DO->SMALLE RECEIVE DATA FUNCTION CALLING BIGGER ONE BASED ON TYPE
int receive_data_from_server()
{
    //TO DO->TIME TO REDUCE DIMENSION OF BUFFER BASED ON THE PACKET!
    char buffer[8192];
    int sender_in_size = sizeof(sk_in);
    int len = recvfrom(new_socket, buffer, 8191, 0, (struct sockaddr*)&sk_in, &sender_in_size); //who's sending info (last 2 input) 

    if (len > 0)
    {
        char addr_as_string[64];
        inet_ntop(AF_INET, &sk_in.sin_addr, addr_as_string, 64); //turn int 32bit into a string
        printf("received %d bytes from %s:%d\n", len, addr_as_string, ntohs(sk_in.sin_port));

        uint8_t type;
        SDL_memcpy(&type, buffer, sizeof(uint8_t));
        printf("type is %d\n", type);
        if (SRV_CHECK == type)
        {
            SRV_CHECK_T check_packet;
            SDL_memcpy(&check_packet, buffer, sizeof(SRV_CHECK_T));
            
            if (check_packet.srv_answer == SRV_WRONG_VERSION)
            {
                puts("Wrong Server Version! Update it!");
                return SRV_WRONG_VERSION;
            }
            puts("Client Version is up to date");
            return 0;
        }

        if (SRV_AUTH == type)
        {
            SRV_AUTH_T auth_packet;
            SDL_memcpy(&auth_packet, buffer, sizeof(SRV_AUTH_T));
            if (auth_packet.srv_answer == SRV_WRONG_USR_PASS) 
            {
                puts("Wrong Username or Password");
                return SRV_WRONG_USR_PASS;
            }
            ID = auth_packet.id;
            printf("Client Authorized with ID: %d\n", ID);
            return 0;
            
        }




        return 2;
    }
    return 66;
}
#pragma endregion
