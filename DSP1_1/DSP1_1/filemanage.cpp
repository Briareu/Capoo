#include<fstream>
using namespace std;

struct matrix {
	int row;
	int col;
	int num;
	int cal;
	int start;
	int end;
	matrix(int r, int c, int n, int ca, int s, int e) {
		row = r;
		col = c;
		num = n;
		cal = ca;
		start = s;
		end = e;
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
	void setcal(int c) {
		cal = c;
	}
	void setstart(int s) {
		start = s;
	}
	void setend(int e) {
		end = e;
	}
};

class filemanage {
public:
	int num;
	int *matrix;
	char *filename;
public:
	filemanage() {
		num = 0;
		matrix = NULL;
		filename = NULL:
	}
	~filemanage() {
		if (!matrix) {
			delete[] matrix;
			matrix = NULL;
		}
	}
	void execute(char *name) {
		filename = name;
		ifstream infile;
		infile.open(filename, ios::in);
	}
};