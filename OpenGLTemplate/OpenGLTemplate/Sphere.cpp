#include <cmath>
#include <vector>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "Sphere.h"
using namespace std;

Sphere::Sphere(int X_segment,int Y_segment) {
	X_SEGMENTS = X_segment;
	Y_SEGMENTS = Y_segment;
	init();
}

void Sphere::init() {

	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			glm::vec3 dir;
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
			float yPos = cos(ySegment * M_PI);
			float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
			dir.x = xPos, dir.y = yPos, dir.z = zPos;
			dir= glm::normalize(dir);
			normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
		}
	}

	//Éú³ÉÇòµÄIndices
	for (int i = 0; i < Y_SEGMENTS; i++)
	{
		for (int j = 0; j < X_SEGMENTS; j++)
		{
			glm::vec3 dir_1;
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			dir_1.x = sphereVertices[i * (X_SEGMENTS + 1) + j];
			dir_1.y = sphereVertices[(i + 1) * (X_SEGMENTS + 1) + j];
			dir_1.z = sphereVertices[(i + 1) * (X_SEGMENTS + 1) + j + 1];
			dir_1 = glm::normalize(dir_1);

			glm::vec3 dir_2;
			sphereIndices.push_back(i* (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
			dir_2.x = sphereVertices[i* (X_SEGMENTS + 1) + j];
			dir_2.y = sphereVertices[(i + 1) * (X_SEGMENTS + 1) + j + 1];
			dir_2.z = sphereVertices[i * (X_SEGMENTS + 1) + j + 1];
			dir_2 = glm::normalize(dir_2);
		}
	}
}

vector<int> Sphere::getIndices() { return sphereIndices; }
vector<float> Sphere::getVertices() { return sphereVertices; }
vector<float> Sphere::getNormals() { return normals; }