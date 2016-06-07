#include "RingBuffer.h"

RingBuffer::RingBuffer()
{
	init();
}

RingBuffer::~RingBuffer()
{
	destroy();
}

void RingBuffer::init()
{
	mReadIndex = 0;
	mWriteIndex = 0;

	sem_init(&mWriteSem, 0, RING_BUFFER_SIZE);
	sem_init(&mReadSem, 0, 0);
}

void RingBuffer::destroy()
{
	if (mReadIndex <= mWriteIndex) {
		for (int i = mReadIndex; i < mWriteIndex; i ++) {
			mDestroyFunc(mRingBuffer[i]);
		}
	} else {
		for (int i = 0; i < mWriteIndex; i ++) {
			mDestroyFunc(mRingBuffer[i]);
		}

		for (int i = mReadIndex; i < RING_BUFFER_SIZE; i ++) {
			mDestroyFunc(mRingBuffer[i]);
		}
	}

	mReadIndex = 0;
	mWriteIndex = 0;

	sem_destroy(&mReadSem);
	sem_destroy(&mWriteSem);
}

void RingBuffer::setDestroy(void (*destroy)(DataType *))
{
	mDestroyFunc = destroy;
}

void RingBuffer::enqueue(DataType *item)
{
	sem_wait(&mWriteSem);

	mRingBuffer[mWriteIndex] = item;
	mWriteIndex = (mWriteIndex + 1) % RING_BUFFER_SIZE;

	sem_post(&mReadSem);
}

void RingBuffer::dequeue(DataType **item)
{
	sem_wait(&mReadSem);

	*item = mRingBuffer[mReadIndex];
	mReadIndex = (mReadIndex + 1) % RING_BUFFER_SIZE;

	sem_post(&mWriteSem);
}