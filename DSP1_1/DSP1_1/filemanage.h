#pragma once
#ifndef FILEMANAGE
#include<fstream>
#define FILENAME

#ifndef BUFFER
#define BUFFER
#include"buffer.h"
#endif // !FILEMANAGE

#ifndef IOSTREAM
#define IOSTREAM
#include<iostream>
#endif // !IOSTREAM

#ifndef SSTREAM
#define SSTREAM
#include<sstream>
#endif // !SSTREAM


using namespace std;

struct input {
	int matrixnum;
	char data;
	int row;
	int col;
};

struct matrix {
	int row;
	int col;
	int num;
	int start;
	int end;
	int n_r;
	int n_c;
	bool isend;
	matrix(int r = 0, int c = 0, int n = 0, int s = 0, int e = 0) {
		row = r;
		col = c;
		num = n;

		start = s;
		end = e;

		isend = false;
		if (start > end) {
			isend = true;
		}
	}
	void setrow(int r) {
		row = r;
	}
	void setcol(int c) {
		col = c;
	}
	void setnum(int n) {
		num = n;
	}
	void setstart(int s) {
		start = s;
	}
	void setend(int e) {
		end = e;
	}
	void setisend(bool flag) {
		isend = flag;
	}
	void checkisend() {
		isend = start > end ? true : false;
	}
	void setnow(int r, int c) {
		n_r = r, n_c = c;
	}
};

class filemanage {
public:
	int num;
	matrix *mat;
	string filename;
	string outfilename;
public:
	filemanage() {
		num = 0;
		mat = NULL;
		filename = nullptr;
		outfilename = nullptr;
	}
	~filemanage() {
		if (!mat) {
			delete[] mat;
			mat = NULL;
		}
	}
	void execute(string name, string outname, int size = 2, int buffersize = 12) {
		filename = name;
		outfilename = outname;
		fstream m_file;
		m_file.open(filename, ios::in||ios::out,0);
		bool endflag = true;

		if (!m_file.is_open()) {
			cout << "open file failed(for reading)" << endl;
			return;
		}

		LoopQueue<input> buf(buffersize);

		//initialization
		stringstream ss;
		string buff;
		int inline_size = 0;
		m_file >> buff;
		inline_size = buff.length() + 1;
		ss << buff;
		ss >> num;
		ss.clear();
		buff.clear();

		mat = new matrix[num];
		
		//matrix initial
		for (int i = 0; i < num; i++) {
			m_file >> buff;//row
			inline_size += buff.length();
			inline_size++;
			ss << buff;
			ss >> mat[i].row;
			ss.clear();
			buff.clear();

			m_file >> buff;//col
			inline_size += buff.length();
			inline_size++;
			ss << buff;
			ss >> mat[i].col;
			ss.clear();
			buff.clear();

			m_file >> buff;//num
			inline_size += buff.length();
			inline_size++;
			ss << buff;
			ss >> mat[i].num;
			ss.clear();
			buff.clear();

			m_file >> buff;//start
			inline_size += buff.length();
			inline_size++;
			ss << buff;
			ss >> mat[i].start;
			ss.clear();
			buff.clear();

			m_file >> buff;//end
			inline_size += buff.length();
			inline_size++;
			ss << buff;
			ss >> mat[i].end;
			ss.clear();
			buff.clear();

			mat[i].checkisend();
		}

		while (endflag) {
			for (int i = 0; i < mat[0].row; i++) {
				int *temp = new int[mat[1].col];
				for (int j = 0; j < mat[1].col; j++) {
					temp[j] = 0;
				}

				for (int k = 0; k < mat[0].col; k++) {
					for (int j = 0; j < mat[1].col; j++) {
						//get a[i][k] and b[k][j]
					}
				}
			}
		}
	}
};
#endif