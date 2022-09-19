#include<iostream>
#define IOSTREAM

#include"buffer.h"
#define BUFFER

#include"filemanage.h"
#define FILEMANAGE

using namespace std;
int main() {
	bool flag = true;
	while (flag) {
		filemanage man;
		string filename;
		string outfile;
		int size = 0;
		int line = 0;

		cout << "Please input file's name: ";
		cin >> filename;
		cout << "Please output file's name: ";
		cin >> outfile;
		cout << "Please input the cache's block size: ";
		cin >> size;
		cout << "Please input the block's num: ";
		cin >> line;
		man.execute(filename, outfile, size, size * line);

		flag = false;
		cout << "\n\nif you want to retest, please type 1: ";
		cin >> filename;
		if (filename == "1") {
			flag = true;
		}
	}
}