#ifndef RECV_BUFFER_HPP
#define RECV_BUFFER_HPP

#define RECV_BUFFER_ALLOCATE_FAILED 0

#include <new>

#include <cstdint>

#include <SDL2/SDL_net.h>

class RecvBuffer {
public:
    RecvBuffer();
	RecvBuffer(unsigned int size, TCPsocket* socket);

	int write(uint8_t value);
	int read(uint8_t* value);
	int remove();

	int getUsed();
	int getFree();

	int recv(unsigned int* amount_read);
private:
	uint8_t* buffer;
	uint8_t* buffer_end;
    unsigned int buffer_size;

	TCPsocket* socket;

	uint8_t* write_ptr;
	uint8_t* read_ptr;
	
	bool filled;
};

#endif // RECV_BUFFER_HPP