#include <iostream>
#include <iomanip>

#include <cstdint>
#include <cstring>

#include "../configuration.h"

#include "recv_buffer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include <thread>

int main(int argc, char* argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "[ERROR] Failed to initialize SDL2." << std::endl;
		std::cout << "  Error Message: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	if(SDLNet_Init() != 0) {
		std::cout << "[ERROR] Failed to initialize SDL2_NET." << std::endl;
		std::cout << "  Error Message: " << SDLNet_GetError() << std::endl;
		return 1;
	}

	IPaddress ip;
	TCPsocket socket;

	if(SDLNet_ResolveHost(&ip, "us.hypixel.net", 25565) == -1) {
		std::cout << "[ERROR] Failed to resolve host." << std::endl;
		std::cout << "  Error Message: " << SDLNet_GetError() << std::endl;
		return 1;
	}

	socket = SDLNet_TCP_Open(&ip);
	if(!socket) {
		std::cout << "[ERROR] Failed to open socket." << std::endl;
		std::cout << "  Error Message: " << SDLNet_GetError() << std::endl;
		return 1;
	}
	
	RecvBuffer recv;
	try {
		recv = RecvBuffer(MAX_RECV_PACKET_SIZE, &socket);
	} catch(int e) { // Failed to allocate memory
		std::cout << "[ERROR] Failed to allocate recieve buffer" << std::endl;
		return 1;
	}

	uint8_t packet1[] = {20,0,47,14,117,115,46,104,121,112,105,120,101,108,46,110,101,116,99,221,1};
	uint8_t packet2[] = {1,0};

	unsigned int packet1_size = sizeof(packet1);
	unsigned int packet2_size = sizeof(packet2);

	int result;
	result = SDLNet_TCP_Send(socket, packet1, packet1_size);
	if(result < 0) { std::cout << "SEND ERROR" << std::endl; return 1; }
	result = SDLNet_TCP_Send(socket, packet2, packet2_size);
	if(result < 0) { std::cout << "SEND ERROR" << std::endl; return 1; }

	while(!recv.isFullPacket()) {
		unsigned int amount_read = 0;
		recv.recv(&amount_read);
		std::cout << "Amount Received: " << amount_read << std::endl;
	}

	int32_t packet_length; int32_t packet_id;
	result = recv.readVarInt(&packet_length);
	if(result < 0) { std::cout << "Failed to get packet_length" << std::endl; }
	result = recv.readVarInt(&packet_id);
	if(result < 0) { std::cout << "Failed to get packet_id" << std::endl; }

	char* motd;
	result = recv.readString(&motd);
	if(result != 0) { std::cout << "Failed to read motd" << std::endl; }

	std::cout << "Packet Length: " << packet_length << std::endl;
	std::cout << "Packet ID:     " << packet_id << std::endl;
	std::cout << "Packet motd:   " << motd << std::endl;
	
	SDLNet_TCP_Close(socket);
	
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}