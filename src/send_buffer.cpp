#include "send_buffer.hpp"

SendBuffer::SendBuffer() {}
SendBuffer::SendBuffer(unsigned int size, TCPsocket* socket) {
    this->buffer_size = size;
    this->buffer = new (std::nothrow)uint8_t[size];

    if(this->buffer == NULL) {
        throw SEND_BUFFER_ALLOCATE_FAILED;
    }

    this->write_ptr = this->buffer;

    this->socket = socket;
}