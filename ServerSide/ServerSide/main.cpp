#include <iostream>
//#include <sstream>
//#include <Windows.h>
//
#include <SDL.h>
#include <SDL_net.h>
//
//#include "String.h"
//
//#include "Connection.h"
//
//#

#include "ChatApplication.h"

int main(int argc, char* argv[])
{
	ChatApplication host;

	if (host.Run());

	system("pause");

	return 0;
}