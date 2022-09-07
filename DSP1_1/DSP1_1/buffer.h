#pragma once
#ifndef LOOPQUE
#include<new>
#define LOOPQUE

using namespace std;

template<typename _TYPE>
class LoopQueue_impl {
private:
	int _front;//front index
	int _rear;//rear index
	int _size;//queue data

	const int max_size;//capacity

	_TYPE data[0];//cannot use this class directly


public:
	static int addsize(int max);
	LoopQueue_impl(int msize);//the queue's size is confirmed at first
	_TYPE &front();
	void push(const _TYPE &value);
	void pop();
	int check_pop(_TYPE &tp);
	int check_push(const _TYPE &value);

	bool full() const { return _size == max_size; };
	bool empty() const { return _size == 0; };
	int size() const { return _size; };
	int capacity() const { return max_size; };
};

template<typename _TYPE>
struct LoopQueue_allocate {
	LoopQueue_impl<_TYPE> &allocate(int msize) {
		char *p = new char[sizeof(LoopQueue_impl<_TYPE>) + LoopQueue_impl<_TYPE>::addsize(msize)];
		return *(new (p) LoopQueue_impl<_TYPE>(msize));
	}

	void deallocate(void *p) {
		delete[](char*)p;
	}
};

template<typename _TYPE, typename Alloc = LoopQueue_allocate<_TYPE>>
class LoopQueue {
private:
	Alloc alloc;
	LoopQueue_impl<_TYPE> &impl;

public:
	typedef _TYPE value_type;

	LoopQueue(int msize) :impl(alloc.allocate(msize)) {};
	LoopQueue() { alloc.deallocate((void *)&impl); };

	value_type &front() { return impl.front() };
	const value_type &front() const { return impl.front; };

	void push(const value_type &value) { impl.push(value); };
	void pop() { impl.pop(); };

	int check_pop(value_type &tp) { return impl.check_pop(tp); };
	int check_push(const value_type &value) { return impl.check_push(value); };

	bool full() const { return impl.full(); };
	bool empty() const { return impl.empty(); };
	int size() const { return impl.size(); };
};

#endif // !LOOPQUE