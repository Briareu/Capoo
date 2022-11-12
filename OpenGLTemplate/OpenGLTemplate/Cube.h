#pragma once
#pragma once
#include <cmath>
#include <vector>
#include <glm\glm.hpp>
using namespace std;

class Cube
{
public:
	Cube(float cx, float cy, float cz);
	void init();
	vector<float> getVertices();
	vector<float> getNormals();

private:
	vector<float> cubeVertices;
	vector<float> normals;
	float x, y, z;
};