#include "queue.h"

#define QUEUE_FLAGS_OVERRUN 0x001

void Initialize_Queue8(Queue8* fifo, int size, unsigned char* data) {
    fifo->data = data;
    fifo->write_point = 0;
    fifo->read_point = 0;
    fifo->data_size = size;
    fifo->free_size = size;
    fifo->flags = 0;
}

int Queue8_In(Queue8* fifo, unsigned char data) {
    if (fifo->free_size == 0) {
        fifo->flags |= QUEUE_FLAGS_OVERRUN;
        return -1;
    }

    fifo->data[fifo->write_point] = data;
    fifo->write_point++;

    if (fifo->write_point == fifo->data_size) {
        fifo->write_point = 0;
    }

    fifo->free_size--;
    return 0;
}

int Queue8_Out(Queue8* fifo) {
    if (fifo->free_size == fifo->data_size) return -1;

    int data = 0;
    data = fifo->data[fifo->read_point];
    fifo->read_point++;
    if (fifo->read_point == fifo->data_size) fifo->read_point = 0;

    fifo->free_size++;
    return data;
}

int Queue8_GetStatus(Queue8* fifo) {
    return fifo->data_size - fifo->free_size;
}
