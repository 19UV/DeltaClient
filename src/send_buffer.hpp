#ifndef SEND_BUFFER_HPP
#define SEND_BUFFER_HPP

#include "../configuration.h"

#include <iostream>
#include <new>

#include <cstdint>

#include <SDL2/SDL_net.h>

class SendBuffer {
public:
    SendBuffer();
    SendBuffer(unsigned int size, TCPsocket* socket);
private:
    uint8_t* buffer;
    unsigned int buffer_size;

    uint8_t* write_ptr;

    TCPsocket* socket;
};

#endif // SEND_BUFFER_HPP