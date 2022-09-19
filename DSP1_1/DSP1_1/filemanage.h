#pragma once
#ifndef FSTREAM
#include<fstream>
#define FSTREAM
#endif

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

struct matrix {
	int row;
	int col;
	int start;
	matrix(int r = 0, int c = 0, int s = 0) {
		row = r;
		col = c;

		start = s;
	}
	void setrow(int r) {
		row = r;
	}
	void setcol(int c) {
		col = c;
	}
	void setstart(int s) {
		start = s;
	}
};

class filemanage {
private:
	int num;
	int failed;
	int hit;
	matrix *mat;
	string filename;
	string outfile;
public:
	filemanage() {
		num = 0;
		failed = 0;
		hit = 0;
	}
	~filemanage() {
		if (!mat) {
			delete[] mat;
			mat = NULL;
		}
	}
	void execute(string name, string out, int size = 2, int buffersize = 12) {
		filename = name;
		outfile = out;
		fstream m_file;
		m_file.open(filename, ios::in||ios::out||ios::app);
		if (!m_file.is_open()) {
			cout << "open file failed" << endl;
			return;
		}
		
		LoopQueue buf(buffersize);

		//initialization
		stringstream ss;
		string buff;
		m_file >> buff;
		ss << buff;
		ss >> num;
		ss.clear();
		buff.clear();

		mat = new matrix[num];
		
		//matrix initial
		for (int i = 0; i < num; i++) {
			m_file >> buff;//row
			ss << buff;
			ss >> mat[i].row;
			ss.clear();
			buff.clear();

			m_file >> buff;//col
			ss << buff;
			ss >> mat[i].col;
			ss.clear();
			buff.clear();

			m_file >> buff;//start
			ss << buff;
			ss >> mat[i].start;
			ss.clear();
			buff.clear();


/*			for (int u = 0; u < 100; u++) {
				cout << endl;
				m_file.seekg(u, ios::beg);
				m_file >> buff;//start
				ss << buff;
				cout << u << ":   ";
				cout << buff;
				ss.clear();
				buff.clear();
			}
			return;
*/
		}
		cout << "\nend of initialization!\nstart to execute.\n";
		m_file.flush();
		m_file.close();
		m_file.clear();

		input tempinput;
		for (int i = 0; i < mat[0].row; i++) {
			m_file.open(filename, ios::in || ios::out || ios::app);
			if (!m_file.is_open()) {
				cout << "open file failed" << endl;
				return;
			}

			int *temp = new int[mat[1].col];
			for (int j = 0; j < mat[1].col; j++) {
				temp[j] = 0;
			}

			for (int k = 0; k < mat[0].col; k++) {
				for (int j = 0; j < mat[1].col; j++) {
					//get a[i][k] and b[k][j]
					int a, b;
					pair<input, bool> res;
					res = buf.find(i, k, 0);
					if (res.second == false) {
						//read
						failed++;
						int i_temp = 0;
						m_file.seekg(mat[0].start, ios::beg);

						while (m_file >> buff && i_temp < i*mat[0].col + k + 1) {
							if (i_temp == i * mat[0].col + k) {
								int i_read = i;
								int j_read = k;
								input resin;
								for (int red = 0; red < size; red++) {
									if (j_read == mat[0].col - 1 && i_read == mat[0].row - 1) {
										if (red != 0) {
											m_file >> buff;
										}
										ss << buff;
										ss >> resin.data;
										ss.clear();
										buff.clear();

										resin.setr(i_read);
										resin.setc(j_read);
										resin.setnum(0);
										buf.push(resin);
										break;
									}
									else if (j_read == mat[0].col - 1) {
										if (red != 0) {
											m_file >> buff;
										}
										ss << buff;
										ss >> resin.data;
										ss.clear();
										buff.clear();

										resin.setr(i_read);
										resin.setc(j_read);
										resin.setnum(0);
										buf.push(resin);
										i_read++;
										j_read = 0;
									}
									else {
										if (red != 0) {
											m_file >> buff;
										}
										ss << buff;
										ss >> resin.data;
										ss.clear();
										buff.clear();

										resin.setr(i_read);
										resin.setc(j_read);
										resin.setnum(0);
										buf.push(resin);
										j_read++;
									}
								}
								i_temp++;
								res = buf.find(i, k, 0);
							}
							buff.clear();
							i_temp++;
						}
					}
					else {
						hit++;
					}
					a = res.first.data;

					res = pair<input, bool>();
					res = buf.find(k, j, 1);
					if (res.second == false) {
						//read
						failed++;
						int i_temp = 0;
						m_file.seekg(mat[1].start, ios::beg);
						while (m_file >> buff && i_temp < k * mat[1].col + j + 1) {
							if (i_temp == k * mat[0].col + j) {
								int i_read = k;
								int j_read = j;
								input resin;
								for (int red = 0; red < size; red++) {
									if (j_read == mat[1].col - 1 && i_read == mat[1].row - 1) {
										if (red != 0) {
											m_file >> buff;
										}
										ss << buff;
										ss >> resin.data;
										ss.clear();
										buff.clear();

										resin.setr(i_read);
										resin.setc(j_read);
										resin.setnum(1);
										buf.push(resin);
										break;
									}
									else if (j_read == mat[0].col - 1) {
										if (red != 0) {
											m_file >> buff;
										}
										ss << buff;
										ss >> resin.data;
										ss.clear();
										buff.clear();

										resin.setr(i_read);
										resin.setc(j_read);
										resin.setnum(1);
										buf.push(resin);
										i_read++;
										j_read = 0;
									}
									else {
										if (red != 0) {
											m_file >> buff;
										}
										ss << buff;
										ss >> resin.data;
										ss.clear();
										buff.clear();

										resin.setr(i_read);
										resin.setc(j_read);
										resin.setnum(1);
										buf.push(resin);
										j_read++;
									}
								}
								i_temp++;
								res = buf.find(k, j, 1);
							}
							buff.clear();
							i_temp++;
						}
					}
					else {
						hit++;

					}
					b = res.first.data;

					//record
					temp[j] += a * b;
				}
			}

			m_file.flush();
			m_file.close();
			m_file.clear();

			if (i == 0) {
				ofstream o_file(outfile, ios::out);

				if (!o_file.is_open()) {
					cout << "open file failed" << endl;
					return;
				}

				//write
				for (int j = 0; j < mat[1].col; j++) {
					o_file << temp[j];
					if (j == mat[1].col - 1) {
						o_file << "\n";
					}
					else {
						o_file << " ";
					}
				}

				o_file.flush();
				o_file.close();
				o_file.clear();
			}
			else {
				ofstream o_file(outfile, ios::app);

				if (!o_file.is_open()) {
					cout << "open file failed" << endl;
					return;
				}

				//write
				for (int j = 0; j < mat[1].col; j++) {
					o_file << temp[j];
					if (j == mat[1].col - 1) {
						o_file << "\n";
					}
					else {
						o_file << " ";
					}
				}

				o_file.flush();
				o_file.close();
				o_file.clear();
			}
		}

		cout << "\nend of execute!\n";
		cout << "\nthe number of hits is: " << hit << endl;
		cout << "the number of miss is: " << failed << endl;
	}
};