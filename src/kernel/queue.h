#ifndef INCLUDED_QUEUE
#define INCLUDED_QUEUE

typedef struct _queue_8 {
    unsigned char* data;
    int write_point;
    int read_point;
    int data_size;
    int free_size;
    int flags;
} Queue8;

void Queue8_InitializeQueue8(Queue8* fifo, int size, unsigned char* data);
int Queue8_In(Queue8* fifo, unsigned char data);
int Queue8_Out(Queue8* fifo);
int Queue8_GetStatus(Queue8* fifo);

#endif
