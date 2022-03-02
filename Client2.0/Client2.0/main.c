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
	
	int status = -1;
	uint8_t version = 1;
	int ID = 0;

	SDL_Init(SDL_INIT_VIDEO);
	client_init();
	send_cl_check(version);
	status = receive_data_from_server(&ID);

	if (status == SRV_WRONG_VERSION)
	{
		//TO DO->FORCE UPDATE
		goto quit;
	}
	
	send_cl_auth("adminn", "adminn");
	status = receive_data_from_server(&ID);
	
	if (status == SRV_WRONG_USR_PASS)
	{
		//TO DO->FORCE UPDATE
		goto quit;
	}

	int running = 1;
	SDL_Quit();
	return 0;


quit:
	SDL_Quit();
	return 0;
}