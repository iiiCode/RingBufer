#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "RingBuffer.h"

#define NUM 1

static void destroy(DataType *item)
{
	free(item);
}

static void *do_producer(void *args)
{
	int value = 0;
	RingBuffer *buffer = (RingBuffer *)args;

	while(true) {
		int *p = (int *)malloc(sizeof(int));
		*p = value;
		printf("Enqueue: %d\n", value);
		value ++;

		buffer->enqueue(p);

		sleep(1);
	}

	return NULL;
}

static void *do_consumer(void *args)
{
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t ptid[NUM];
	RingBuffer buffer;

	buffer.setDestroy(destroy);

	for (int i = 0; i < NUM; i ++) {
		pthread_create(&ptid[i], NULL, (NUM % 2)? do_producer : do_consumer, &buffer);
	}

	for (int i = 0; i < NUM; i ++) {
		pthread_join(ptid[i], NULL);
	}

	return 0;
}