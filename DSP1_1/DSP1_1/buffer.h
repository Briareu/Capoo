#pragma once
#ifndef QUEUE
#include<queue>
#endif // !QUEUE

#ifndef THREAD
#include<thread>
#endif // !THREAD

#ifndef PTHREAD_H
#include<pthread.h>
#endif // !PTHREAD_H

const int MatrixQueueSize = 3;//the queue's nodes in tital

enum QueueType
{
	FREEQUEUE,
	WORKQUEUE
};

typedef struct QueueNode {
	void *data;//any type
	size_t size;//data size
	long index;
	struct QueueNode *next;
} QueueNode;

typedef struct MatrixQueue {
	int size;
	QueueType type;
	QueueNode *front;
	QueueNode *rear;
} MatrixQueue;

class MatrixProcess {
private:
	pthread_mutex_t freequeue_mutex;
	pthread_mutex_t workqueue_mutex;
public:
	MatrixQueue *m_freequeue;
	MatrixQueue *m_workqueue;

	//constructor and destructor
	MatrixProcess();
	~MatrixProcess();

	void InitQueue(MatrixQueue *q, QueueType type);
	void EnQueue(MatrixQueue *q, QueueNode *node);

	QueueNode *DeQueue(MatrixQueue *q);

	void ClearMatrixQueue(MatrixQueue *q);
	void FreeNode(QueueNode *node);
	void ResetFreeQueue(MatrixQueue *workq, MatrixQueue *freeq);
};