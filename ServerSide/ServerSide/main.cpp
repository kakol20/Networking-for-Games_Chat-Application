#include <iostream>

#include <SDL.h>
#include <SDL_net.h>

#include "ChatApplication.h"

int main(int argc, char* argv[])
{
	ChatApplication host;

	if (!host.Run()) return -1;

	system("pause");

	return 0;
}