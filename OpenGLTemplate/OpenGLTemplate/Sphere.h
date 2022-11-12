#pragma once
#include <cmath>
#include <vector>
#include <glm\glm.hpp>
using namespace std;

#define M_PI acos(-1)

class Sphere
{
public:
	Sphere(int X_SEGMENTS, int Y_SEGMENTS);
	void init();
	vector<int> getIndices();
	vector<float> getVertices();
	vector<float> getNormals();

private:
	//将球横纵划分成50*50的网格
	int Y_SEGMENTS;
	int X_SEGMENTS;

	vector<float> sphereVertices;
	vector<int> sphereIndices;
	vector<float> normals;
};