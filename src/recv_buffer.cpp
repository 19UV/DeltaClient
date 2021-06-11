#include "recv_buffer.hpp"

RecvBuffer::RecvBuffer() {}
RecvBuffer::RecvBuffer(unsigned int size, TCPsocket* socket) {
    this->buffer = new (std::nothrow)uint8_t[size];
    
    if(this->buffer == NULL) {
        throw RECV_BUFFER_ALLOCATE_FAILED;
    }

    this->buffer_size = size;
    this->buffer_end = this->buffer + size;

    this->write_ptr = this->buffer;
    this->read_ptr = this->buffer;
    this->filled = false;

    this->socket = socket;
}

int RecvBuffer::write(uint8_t value) {
    if((this->write_ptr == this->read_ptr) && this->filled) {
        return 1;
    }

    *(this->write_ptr++) = value;
    this->filled = true;

    if(this->write_ptr == this->buffer_end) {
        this->write_ptr = this->buffer;
    }

    return 0;
}
int RecvBuffer::read(uint8_t* value) {
    if((this->write_ptr == this->read_ptr) && !this->filled) {
        return 1;
    }

    *value = *(this->read_ptr++);
    
    if(this->read_ptr == this->buffer_end) {
        this->read_ptr = this->buffer;
    }

    if(this->read_ptr == this->write_ptr) {
        this->filled = false;
    }

    return 0;
}
int RecvBuffer::remove() {
    delete[] this->buffer;
    return 0;
}

int RecvBuffer::getUsed() {
    if(this->read_ptr == this->write_ptr) {
        return this->filled ? this->buffer_size : 0;
    }
    if(this->read_ptr < this->write_ptr) {
        return this->write_ptr - this->read_ptr;
    } else {
        return (this->buffer_end - this->read_ptr) + (this->write_ptr - this->buffer);
    }
}
int RecvBuffer::getFree() { return this->buffer_size - this->getUsed(); }

int RecvBuffer::recv(unsigned int* amount_read) {
    if((this->read_ptr == this->write_ptr) && this->filled) {
        *amount_read = 0;
        return 0;
    }

    this->filled = true;

    unsigned int free_size = this->buffer_end - this->write_ptr;
    int size = SDLNet_TCP_Recv(*this->socket, this->write_ptr, free_size);
    if(size <= 0) {
        *amount_read = 0;
        return 1;
    }

    // NOTE: May be able to use reinterpret_cast, as the int is 'technically' unsigned
    unsigned int u_size = static_cast<unsigned int>(size);
    *amount_read = u_size;
    this->write_ptr += u_size;

    if(this->write_ptr >= this->buffer_end) {
        this->write_ptr = this->buffer;

        free_size = (this->read_ptr - this->buffer) - 1; // May remove -1
        size = SDLNet_TCP_Recv(*this->socket, this->write_ptr, free_size);

        if(size <= 0) {
            return 1;
        }

        // NOTE: May be able to use reinterpret_cast, as the int is 'technically' unsigned
        u_size = static_cast<unsigned int>(size);
        *amount_read += u_size;
    }

    return 0;
}

