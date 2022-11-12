#include <cmath>
#include <vector>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "Cube.h"
using namespace std;

Cube::Cube(float cx, float cy, float cz){
	x = cx; y = cy; z = cz;
	init();
}

void Cube::init() {
	//up
	glm::vec3 dir_1(0.0f, 1.0f, 0.0f);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(-x);  cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);  cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);  cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_1.x);
		normals.push_back(dir_1.y);
		normals.push_back(dir_1.z);
	}
	//down
	glm::vec3 dir_2(0.0f, -1.0f, 0.0f);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(z);
	cubeVertices.push_back(-x);  cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);  cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);  cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y); cubeVertices.push_back(z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_2.x);
		normals.push_back(dir_2.y);
		normals.push_back(dir_2.z);
	}
	//front
	glm::vec3 dir_3(0.0f, 0.0f, 1.0f);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_3.x);
		normals.push_back(dir_3.y);
		normals.push_back(dir_3.z);
	}
	//back
	glm::vec3 dir_4(0.0f, 0.0f, -1.0f);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_4.x);
		normals.push_back(dir_4.y);
		normals.push_back(dir_4.z);
	}
	//left
	glm::vec3 dir_5(-1.0f, 0.0f, 0.0f);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(-x);  cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(-x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_5.x);
		normals.push_back(dir_5.y);
		normals.push_back(dir_5.z);
	}
	//right
	glm::vec3 dir_6(1.0f, 0.0f, 0.0f);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(x);  cubeVertices.push_back(-2.0f);  cubeVertices.push_back(z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(-z);
	cubeVertices.push_back(x);   cubeVertices.push_back(-2.0f - y);  cubeVertices.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_6.x);
		normals.push_back(dir_6.y);
		normals.push_back(dir_6.z);
	}
}

vector<float> Cube::getVertices() { return cubeVertices; }
vector<float> Cube::getNormals() { return normals; }