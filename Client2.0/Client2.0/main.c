#define SDL_MAIN_HANDLED 1
#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <string.h>
#include <stdio.h>
#include "usefull_func.h"
#include "client.h"
#include "OpCodes.h"

int main(int argc, char** argv)
{
	
	int tries_done = 0;
	int status = -1;
	uint8_t version = 1;


	SDL_Init(SDL_INIT_VIDEO);
	client_init();

	while (status != 0 && tries_done < MAX_TRIES)
	{
		send_cl_check(version);
		status = receive_data_from_server();
		++tries_done;

		if (status == SRV_WRONG_VERSION)
		{
			//TO DO->FORCE UPDATE
			goto quit;
		}
	}

	tries_done = 0;
	status = -1;
	//to do -> refactoring
	while (status != 0 && tries_done < MAX_TRIES)
	{
		send_cl_auth("adminn", "adminn");
		status = receive_data_from_server();
		++tries_done;
		if (status == SRV_WRONG_USR_PASS)
		{
			//TO DO->Give chance to change pass and user and re do the auth
			goto quit;//placeholder
		}
	}

	status = -1;
	tries_done = 0;

	//i'm authorized and with proper client version
	
	//game loop
	send_cl_ping();
	/*while (true)
	{
	}*/
	

	int running = 1;
	SDL_Quit();
	return 0;
	

quit:
	//implement all the quit for socket,etc
	SDL_Quit();
	return 0;
}