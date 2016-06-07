#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "RingBuffer.h"

#define NUM 2

static int p_times = 10000;
static int c_times = 10000;

static void destroy(DataType *item)
{
	free(item);
}

static void *do_producer(void *args)
{
	int value = 0;
	RingBuffer *buffer = (RingBuffer *)args;

	while(p_times --) {
		int *p = (int *)malloc(sizeof(int));
		*p = value;
		buffer->enqueue(p);
		printf("Enqueue: %d\n", value);
		value ++;
		usleep(100);
	}

	return NULL;
}

static void *do_consumer(void *args)
{
	RingBuffer *buffer = (RingBuffer *)args;

	while(c_times --) {
		int *p;
		buffer->dequeue(&p);
		printf("Value: %d\n", *p);
		free(p);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t ptid[NUM];
	RingBuffer buffer;

	buffer.setDestroy(destroy);

	for (int i = 0; i < NUM; i ++) {
		pthread_create(&ptid[i], NULL, ((i % 2) == 0 )? do_producer : do_consumer, &buffer);
	}

	for (int i = 0; i < NUM; i ++) {
		pthread_join(ptid[i], NULL);
	}

	return 0;
}