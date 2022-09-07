#include"buffer.h"

template<typename _TYPE>
int LoopQueue_impl<_TYPE>::addsize(int max)
{
	return max_size * sizeof(_TYPE);
}

template<typename _TYPE>
LoopQueue_impl<_TYPE>::LoopQueue_impl(int msize) {
	max_size = msize;
	_front = 0;
	_rear = 0;
	_size = 0;
}

template<typename _TYPE>
_TYPE &LoopQueue_impl<_TYPE>::front()
{
	return data[_front];
}

template<typename _TYPE>
void LoopQueue_impl<_TYPE>::push(const _TYPE &value)
{
	data[_rear] = value;
	_rear = (_rear + 1) % max_size;
	_size++;
}

template<typename _TYPE>
void LoopQueue_impl<_TYPE>::pop()
{
	
	_size--;
}

template<typename _TYPE>
int LoopQueue_impl<_TYPE>::check_pop(_TYPE &tp) {
	if (empty()) {
		return -1;
	}
	tp = front();
	pop();
	return 0;
}

template<typename _TYPE>
int LoopQueue_impl<_TYPE>::check_push(const _TYPE &value) {
	if (full()) {
		return -1;
	}
	push(value);
	return 0;
}