#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <semaphore.h>

typedef int DataType;
#define RING_BUFFER_SIZE 4

class RingBuffer {
public:
	RingBuffer();
	~RingBuffer();

public:
	void enqueue(DataType *item);
	void dequeue(DataType **item);
	void setDestroy(void (*destroy)(DataType *));
private:
	void init();
	void destroy();
private:
	sem_t *mWriteSem;
	sem_t *mReadSem;

	int mReadIndex;
	int mWriteIndex;
	DataType *mRingBuffer[RING_BUFFER_SIZE];
	void (*mDestroyFunc)(DataType *);
};

#endif //_RING_BUFFER_H_