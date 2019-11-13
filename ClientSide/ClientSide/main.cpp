#include <iostream>
#include <Windows.h>

#include <SDL.h>
#include <SDL_net.h>

#include "String.h"
#include "ChatApplication.h"

int main(int argc, char* argv[])
{
	ChatApplication chatApp;

	if (!chatApp.Run()) return -1;

	system("pause");

	return 0;
}