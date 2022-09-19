#pragma once
#ifndef LOOPQUE
#include<new>
#define LOOPQUE
#endif // !LOOPQUE

using namespace std;

struct input {
	int matrixnum;
	int data;
	int row;
	int col;
	input(int num = -1, int d = -1, int r = -1, int c = -1) {
		matrixnum = num;
		data = d;
		row = r;
		col = c;
	}
	void setnum(int num) {
		matrixnum = num;
	}
	void setd(int d) {
		data = d;
	}
	void setr(int r) {
		row = r;
	}
	void setc(int c) {
		col = c;
	}
};

class LoopQueue {
private:
	int size;
	int cap;
	int ptr;
	input* que;
public:
	LoopQueue(int mcap = 12) {
		cap = mcap;
		size = 0;
		ptr = 0;
		que = new input[cap];
	}
	void push(input data) {
		if (size == cap) {
			pop();
			push(data);
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
			que[0] = input();
			for (int i = 0; i < size - 1; i++) {
				que[i] = que[i + 1];
			}
			size--;
			return true;
		}
	}
	pair<input, bool> get(int of) {
		if (of > size) {
			return pair<input,bool>(input(), false);
		}
		else {
			return pair<input, bool>(que[of], true);
		}
	}
	pair<input, bool> find(int r, int c, int num) {
		for (int i = 0; i < this->size; i++) {
			if (que[i].col == c && que[i].row == r && que[i].matrixnum == num) {
				return pair<input, bool>(que[i], true);
			}
		}
		return pair<input, bool>(input(), false);
	}
};