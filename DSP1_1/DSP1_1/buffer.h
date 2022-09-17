#pragma once
#ifndef LOOPQUE
#include<new>
#define LOOPQUE

using namespace std;

template<typename _TYPE>
class LoopQueue {
private:
	int size;
	int cap;
	int dehit;
	int ptr;
	int* que;
public:
	LoopQueue(int mcap = 12) {
		cap = mcap;
		size = 0;
		dehit = 0;
		ptr = 0;
		que = new int[cap];
	}
	bool push(int data) {
		if (size == cap) {
			return false;
		}
		else {
			que[ptr] = data;
			size++;
			ptr++;
		}
	}
	bool pop() {
		if (size == 0) {
			return false;
		}
		else {
			ptr--;
			que[ptr] = 0;
			size--;
		}
	}
	pair<int, bool> get(int of) {
		if (of > size) {
			return pair<int,bool>(0, false);
		}
		else {

		}
	}
};

#endif // !LOOPQUE