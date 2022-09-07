#ifndef IOSTREAM
#include<iostream>
#endif // !IOSTREAM

#ifndef BUFFER_H
#include"buffer.h"
#endif // !BUFFER_H
using namespace std;

MatrixProcess::MatrixProcess()
{
	//set the memory size
	m_freequeue = (MatrixQueue *)malloc(sizeof(struct MatrixQueue));
	m_workqueue = (MatrixQueue *)malloc(sizeof(struct MatrixQueue));

	InitQueue(m_freequeue, FREEQUEUE);
	InitQueue(m_workqueue, WORKQUEUE);

	for (int i = 0; i < MatrixQueueSize; i++) {
		QueueNode *node = (QueueNode *)malloc(sizeof(struct QueueNode));
		node->data = NULL;
		node->size = 0;
		node->index = 0;
		this->EnQueue(m_freequeue, node);
	}

	pthread_mutex_init(&freequeue_mutex, NULL);
	pthread_mutex_init(&workqueue_mutex, NULL);

	cout << "MatrixProcess Initialize finish!\n";
}

MatrixProcess::~MatrixProcess()
{
}

void MatrixProcess::InitQueue(MatrixQueue * q, QueueType type)
{
	if (q != NULL) {
		q->type = type;
		q->size = 0;
		q->front = 0;
		q->rear = 0;
	}
}

//add node
void MatrixProcess::EnQueue(MatrixQueue * q, QueueNode * node)
{
	if (q == NULL) {
		cout << "MatrixProcess Enqueue: current queue is NULL\n";
		return;
	}
	if (node == NULL) {
		cout << "MatrixProcess Enqueue: current node is NULL\n";
		return;
	}

	node->next = NULL;

	if (FREEQUEUE == q->type) {
		pthread_mutex_lock(&freequeue_mutex);

		if (q->front == NULL) {//node gonna be the first
			q->front = node;
			q->rear = node;
		}
		else {
			//head in, head out
			node->next = q->front;
			q->front = node;
		}
		q->size++;
		cout << "MatrixProcess Enqueue: free queue size = " << q->size << endl;
		pthread_mutex_unlock(&freequeue_mutex);
	}
	else {
		pthread_mutex_lock(&workqueue_mutex);

		//action

		static long nodeIndex = 0;
		node->index = (++nodeIndex);
		if (q->front == NULL) {
			q->front = node;
			q->rear = node;
		}
		else {
			q->rear->next = node;
			q->rear = node;
		}
		q->size++;
		cout << "MatrixProcess Enqueue: free queue size = " << q->size << endl;
		pthread_mutex_unlock(&workqueue_mutex);
	}
}

QueueNode * MatrixProcess::DeQueue(MatrixQueue * q)
{
	if (q == NULL) {
		cout << "MatrixProcess Dequeue: current queue is NULL!\n";
		return nullptr;
	}

	const char *type = q->type == WORKQUEUE ? "work queue" : "free queue";
	pthread_mutex_t *queue_mutex = ((q->type == WORKQUEUE) ? &workqueue_mutex : &freequeue_mutex);
	QueueNode *element = NULL;

	pthread_mutex_lock(queue_mutex);
	element = q->front;
	if (element == NULL) {
		pthread_mutex_unlock(queue_mutex);
		cout << "MatrixProcess Dequeue: the node is NULL\n";
		return NULL;
	}

	q->front = q->front->next;
	q->size--;
	pthread_mutex_unlock(queue_mutex);

	cout << "MatrixProcess Dequeue: " << type << " size = " << q->size << endl;
	return element;
}

void MatrixProcess::ClearMatrixQueue(MatrixQueue * q)
{
	while (q->size) {
		QueueNode *node = this->DeQueue(q);
		this->FreeNode(node);
	}
	cout << "Clear MatrixProcess queue" << endl;
}

void MatrixProcess::FreeNode(QueueNode * node)
{
	if (node != NULL) {
		free(node->data);
		free(node);
	}
}

void MatrixProcess::ResetFreeQueue(MatrixQueue * workq, MatrixQueue * freeq)
{
	if (workq == NULL) {
		cout << "MatrixProcess ResetFreeQueue: The WorkQueue is NULL!\n";
		return;
	}

	if (freeq == NULL) {
		cout << "MatrixProcess ResetFreeQueue: The FreeQueue is NULL!\n";
		return;
	}

	int workQueueSize = workq->size;
	if (workQueueSize > 0) {
		for (int i = 0; i < workQueueSize; i++) {
			QueueNode *node = DeQueue(workq);
			free(node->data);
			node->data = NULL;
			EnQueue(freeq, node);
		}
	}

	cout << "MatrixProcess RestFreeQueue: The work queue size is " << workq->size << ", free queue size is " << freeq->size << endl;
}
