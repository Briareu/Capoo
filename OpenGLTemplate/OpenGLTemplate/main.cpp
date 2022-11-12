#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stack>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Utils.h"
using namespace std;

#define M_PI acos(-1)

#define numVAOs 2
#define numVBOs 10//5*2

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, vColorLoc, viewLoc, nLoc;
int width, height;
GLfloat aspect;
glm::mat4 pMat, vMat, mMat, mvMat;
stack<glm::mat4> mvStack;

//Camera
float cameraX, cameraY, cameraZ;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat fov = 45.0f;
GLfloat lastX = 300, lastY = 300; //初始值设置为屏幕的中心
bool firstMouse = true;
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
//鼠标输入
bool keys[1024];

int num_cir1 = 0, num_cir2 = 0, num_cy = 0;
int num_tail = 0;
//将球横纵划分成50*50的网格
const int Y_SEGMENTS = 50;
const int X_SEGMENTS = 50;
vector<float> sphereVertices;
vector<int> sphereIndices;

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

//light-related
glm::vec3 lightLoc = glm::vec3(5.0f, 0.0f, 5.0f);
float amt = 0.0f;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
float lightPos[3];
glm::vec3 currentLightPos, transformed;

// white light
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

//material blue
float matAmbBlue[4] = { 0.28f,0.66f,1.00f,1.00f };
float matDifBlue[4] = { 0.49f,0.74f,1.00f,1.00f };
float matSpeBlue[4] = { 0.33f,0.42f,1.00f,1.00f };
float matShiBlue = 20;
//material black
float matAmbBlack[4] = { 0.0f,0.0f,0.0f,0.0f };
float matDifBlack[4] = { 0.0f,0.0f,0.0f,0.0f };
float matSpeBlack[4] = { 0.0f,0.0f,0.0f,0.0f };
float matShiBlack = 20;
//material

//material black
float* matAmbBron = Utils::bronzeAmbient();
float* matDifBron = Utils::bronzeDiffuse();
float* matSpeBron = Utils::bronzeSpecular();
float matShiBron = Utils::bronzeShininess();

void setMaterialBlue() {
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmbBlue);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDifBlue);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpeBlue);
	glProgramUniform1f(renderingProgram, mshiLoc, matShiBlue);
}

void setMaterialBlack() {
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmbBlack);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDifBlack);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpeBlack);
	glProgramUniform1f(renderingProgram, mshiLoc, matShiBlack);
}

void setMaterialBron() {
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmbBron);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDifBron);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpeBron);
	glProgramUniform1f(renderingProgram, mshiLoc, matShiBron);
}

void installLights(glm::mat4 vMatrix) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
}

void setColor(std::vector<float> &color, int num_inner, glm::vec3 col) {
	for (int i = 0; i < num_inner; i++) {
		color.push_back(col.x);
		color.push_back(col.y);
		color.push_back(col.z);
		color.push_back(1.0f);
	}
}

void drawCube(std::vector<float> &nodes, std::vector<float> &normals, float x = 4.0f, float z = 2.0f, float y = 0.2f) {
	//up
	glm::vec3 dir_1(0.0f, 1.0f, 0.0f);
	nodes.push_back(-x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(-x);  nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(x);  nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(x);  nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(-x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_1.x);
		normals.push_back(dir_1.y);
		normals.push_back(dir_1.z);
	}
	//down
	glm::vec3 dir_2(0.0f, -1.0f, 0.0f);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	nodes.push_back(-x);  nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(x);  nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(x);  nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	nodes.push_back(x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_2.x);
		normals.push_back(dir_2.y);
		normals.push_back(dir_2.z);
	}
	//front
	glm::vec3 dir_3(0.0f, 0.0f, 1.0f);
	nodes.push_back(-x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	nodes.push_back(x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_3.x);
		normals.push_back(dir_3.y);
		normals.push_back(dir_3.z);
	}
	//back
	glm::vec3 dir_4(0.0f, 0.0f, -1.0f);
	nodes.push_back(-x);   nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(x);   nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_4.x);
		normals.push_back(dir_4.y);
		normals.push_back(dir_4.z);
	}
	//left
	glm::vec3 dir_5(-1.0f, 0.0f, 0.0f);
	nodes.push_back(-x);   nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(-x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(-x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(-x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_5.x);
		normals.push_back(dir_5.y);
		normals.push_back(dir_5.z);
	}
	//right
	glm::vec3 dir_6(1.0f, 0.0f, 0.0f);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(-z);
	nodes.push_back(x);   nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(x);   nodes.push_back(-2.0f);  nodes.push_back(z);
	nodes.push_back(x);   nodes.push_back(-2.0f - y);  nodes.push_back(-z);
	nodes.push_back(x);   nodes.push_back(-2.0f - y);  nodes.push_back(z);
	for (int i = 0; i < 6; i++) {
		normals.push_back(dir_6.x);
		normals.push_back(dir_6.y);
		normals.push_back(dir_6.z);
	}
}

// 绘制球体
void drawSphere(std::vector<float> &normals)
{
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			glm::vec3 dir_1;
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);
			float yPos = cos(ySegment * M_PI);
			float zPos = sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);
			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
			dir_1.x = xPos, dir_1.y = yPos, dir_1.z = zPos;
			dir_1 = glm::normalize(dir_1);
			normals.push_back(dir_1.x), normals.push_back(dir_1.y), normals.push_back(dir_1.z);
		}
	}

	//生成球的Indices
	for (int i = 0; i < Y_SEGMENTS; i++)
	{
		for (int j = 0; j < X_SEGMENTS; j++)
		{
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);;

			sphereIndices.push_back(i* (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}
}

//绘制圆锥
void drawTail(glm::vec3 center, float R, glm::vec3 top, std::vector<float> &nodes, std::vector<float> &normals, int &num_inner) {
	//center底部圆心
	//node_top顶点
	//R底部半径
	//num_inner--传全局变量的num_tail
	for (int i = 0; i < 100; i++) {
		glm::vec3 dir1, dir2, dir_cir1, dir_cir2;
		nodes.push_back(top.x);
		nodes.push_back(top.y);
		nodes.push_back(top.z);
		num_inner++;

		glm::vec3 temp;
		temp.x = R * cos(M_PI * 2 * i / 100) + center.x;
		temp.y = R * sin(M_PI * 2 * i / 100) + center.y;
		temp.z = center.z;
		nodes.push_back(temp.x);
		nodes.push_back(temp.y);
		nodes.push_back(temp.z);
		num_inner++;
		dir1.x = temp.x - top.x;
		dir1.y = temp.y - top.y;
		dir1.z = temp.z - top.z;
		dir_cir1 = glm::vec3(temp.x, temp.y, 0);
		dir_cir1 = glm::normalize(dir_cir1);

		i++;
		temp.x = R * cos(M_PI * 2 * i / 100) + center.x;
		temp.y = R * sin(M_PI * 2 * i / 100) + center.y;
		temp.z = center.z;
		nodes.push_back(temp.x);
		nodes.push_back(temp.y);
		nodes.push_back(temp.z);
		num_inner++;
		i--;
		dir2.x = temp.x - top.x;
		dir2.y = temp.y - top.y;
		dir2.z = temp.z - top.z;
		dir_cir2 = glm::vec3(temp.x, temp.y, 0);
		dir_cir2 = glm::normalize(dir_cir2);

		glm::vec3 dir;
		dir = glm::cross(dir1, dir2);
		dir = glm::normalize(dir);

		normals.push_back(dir.x);
		normals.push_back(dir.y);
		normals.push_back(dir.z);

		normals.push_back(dir_cir1.x);
		normals.push_back(dir_cir1.y);
		normals.push_back(dir_cir1.z);

		normals.push_back(dir_cir2.x);
		normals.push_back(dir_cir2.y);
		normals.push_back(dir_cir2.z);
	}
}

//绘制耳朵（四棱锥）
void drawEar(std::vector<float> &nodes, std::vector<float> &normals, glm::vec3 temp) {
	glm::vec3 node_top, node_fl, node_fr, node_sl, node_sr, node_st, node_end;
	glm::vec3 dir1, dir2, dir;

	node_top.x = temp.x;  node_top.y = temp.y;  node_top.z = temp.z + 2.5f;//the top
	node_fl.x = temp.x + 1.5f;  node_fl.y = temp.y + 1.5f;  node_fl.z = temp.z;//the front left
	node_fr.x = temp.x - 1.5f;  node_fr.y = temp.y + 1.5f;  node_fr.z = temp.z;//the front right
	node_sl.x = temp.x + 1.3f;  node_sl.y = temp.y;  node_sl.z = temp.z;//the second left
	node_sr.x = temp.x - 1.3f;  node_sr.y = temp.y;  node_sr.z = temp.z;//the second right
	node_st.x = temp.x;  node_st.y = temp.y - 1.0f;  node_st.z = temp.z + 1.7f;//the second top
	node_end.x = temp.x;  node_end.y = temp.y - 1.3f;  node_end.z = temp.z;//the end


	nodes.push_back(node_top.x);  nodes.push_back(node_top.y);  nodes.push_back(node_top.z);//top
	nodes.push_back(node_fl.x);  nodes.push_back(node_fl.y);  nodes.push_back(node_fl.z);//fl
	nodes.push_back(node_fr.x);  nodes.push_back(node_fr.y);  nodes.push_back(node_fr.z);//fr
	dir1 = node_fl - node_top, dir2 = node_fr - node_top;
	dir = glm::cross(dir1, dir2);
	dir = glm::normalize(dir);
	for (int i = 0; i < 3; i++) {
		normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
	}

	nodes.push_back(node_top.x);  nodes.push_back(node_top.y);  nodes.push_back(node_top.z);//top
	nodes.push_back(node_sl.x);  nodes.push_back(node_sl.y);  nodes.push_back(node_sl.z);//sl
	nodes.push_back(node_fl.x);  nodes.push_back(node_fl.y);  nodes.push_back(node_fl.z);//fl
	dir1 = node_fl - node_top, dir2 = node_sl - node_top;
	dir = glm::cross(dir1, dir2);
	dir = glm::normalize(dir);
	for (int i = 0; i < 3; i++) {
		normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
	}

	nodes.push_back(node_top.x);  nodes.push_back(node_top.y);  nodes.push_back(node_top.z);//top
	nodes.push_back(node_sr.x);  nodes.push_back(node_sr.y);  nodes.push_back(node_sr.z);//sr
	nodes.push_back(node_fr.x);  nodes.push_back(node_fr.y);  nodes.push_back(node_fr.z);//fr
	dir1 = node_sr - node_top, dir2 = node_fr - node_top;
	dir = glm::cross(dir1, dir2);
	dir = glm::normalize(dir);
	for (int i = 0; i < 3; i++) {
		normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
	}

	nodes.push_back(node_top.x);  nodes.push_back(node_top.y);  nodes.push_back(node_top.z);//top
	nodes.push_back(node_st.x);  nodes.push_back(node_st.y);  nodes.push_back(node_st.z);//st
	nodes.push_back(node_sr.x);  nodes.push_back(node_sr.y);  nodes.push_back(node_sr.z);//sr
	dir1 = node_st - node_top, dir2 = node_sr - node_top;
	dir = glm::cross(dir1, dir2);
	dir = glm::normalize(dir);
	dir = glm::normalize(dir);
	for (int i = 0; i < 3; i++) {
		normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
	}

	nodes.push_back(node_top.x);  nodes.push_back(node_top.y);  nodes.push_back(node_top.z);//top
	nodes.push_back(node_st.x);  nodes.push_back(node_st.y);  nodes.push_back(node_st.z);//st
	nodes.push_back(node_sl.x);  nodes.push_back(node_sl.y);  nodes.push_back(node_sl.z);//sl
	dir1 = node_st - node_top, dir2 = node_sl - node_top;
	dir = glm::cross(dir1, dir2);
	dir = glm::normalize(dir);
	dir = glm::normalize(dir);
	for (int i = 0; i < 3; i++) {
		normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
	}

	nodes.push_back(node_end.x);  nodes.push_back(node_end.y);  nodes.push_back(node_end.z);//end
	nodes.push_back(node_sl.x);  nodes.push_back(node_sl.y);  nodes.push_back(node_sl.z);//sl
	nodes.push_back(node_st.x);  nodes.push_back(node_st.y);  nodes.push_back(node_st.z);//st
	dir1 = node_sl - node_top, dir2 = node_st - node_top;
	dir = glm::cross(dir1, dir2);
	dir = glm::normalize(dir);
	for (int i = 0; i < 3; i++) {
		normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
	}

	nodes.push_back(node_end.x);  nodes.push_back(node_end.y);  nodes.push_back(node_end.z);//end
	nodes.push_back(node_sr.x);  nodes.push_back(node_sr.y);  nodes.push_back(node_sr.z);//sr
	nodes.push_back(node_st.x);  nodes.push_back(node_st.y);  nodes.push_back(node_st.z);//st
	dir1 = node_sr - node_top, dir2 = node_st - node_top;
	dir = glm::cross(dir1, dir2);
	dir = glm::normalize(dir);
	for (int i = 0; i < 3; i++) {
		normals.push_back(dir.x), normals.push_back(dir.y), normals.push_back(dir.z);
	}
}

void drawCylinder(vector<float> &circle1, std::vector<float> &normals,
	glm::vec3 cir_1, glm::vec3 cir_2, int R) {
	vector<float> circle2;
	vector<float> cylinder;

	vector<float> normals_2;
	vector<float> normals_cy;

	for (int n = 0; n < 100; n++) {
		glm::vec3 dir_cy, a, b;

		glm::vec3 dir_cir1;
		circle1.push_back(cir_1.x);
		circle1.push_back(cir_1.y);
		circle1.push_back(cir_1.z);
		num_cir1++;
		dir_cir1.x = -(cir_1.x - cir_2.x), dir_cir1.y = -(cir_1.y - cir_2.y), dir_cir1.z = -(cir_1.z - cir_2.z);
		dir_cir1 = glm::normalize(dir_cir1);
		normals.push_back(dir_cir1.x), normals.push_back(dir_cir1.y), normals.push_back(dir_cir1.z);

		glm::vec3 dir_cir2;
		circle2.push_back(cir_2.x);
		circle2.push_back(cir_2.y);
		circle2.push_back(cir_2.z);
		num_cir2++;
		//dir_cir2.x = -(cir_2.x - cir_1.x), dir_cir2.y = -(cir_2.y - cir_1.y), dir_cir2.z = -(cir_2.z - cir_1.z);
		dir_cir2 = -dir_cir1;
		dir_cir2 = glm::normalize(dir_cir2);
		normals_2.push_back(dir_cir2.x), normals_2.push_back(dir_cir2.y), normals_2.push_back(dir_cir2.z);

		circle1.push_back(R * cos(M_PI * 2 * n / 100));
		circle1.push_back(R * sin(M_PI * 2 * n / 100));
		circle1.push_back(cir_1.z);
		num_cir1++;
		normals.push_back(dir_cir1.x), normals.push_back(dir_cir1.y), normals.push_back(dir_cir1.z);

		circle2.push_back(R * cos(M_PI * 2 * n / 100));
		circle2.push_back(R * sin(M_PI * 2 * n / 100));
		circle2.push_back(cir_2.z);
		num_cir2++;
		normals_2.push_back(dir_cir2.x), normals_2.push_back(dir_cir2.y), normals_2.push_back(dir_cir2.z);

		glm::vec3 temp_1;
		glm::vec3 temp_cy;
		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_1.z);
		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_2.z);
		temp_cy.x = R * cos(M_PI * 2 * n / 100), temp_cy.y = R * sin(M_PI * 2 * n / 100), temp_cy.z = cir_2.z;
		temp_1.x = R * cos(M_PI * 2 * n / 100);
		temp_1.y = R * sin(M_PI * 2 * n / 100);
		num_cy += 2;
		dir_cy = glm::vec3(R * cos(M_PI * 2 * n / 100), R * sin(M_PI * 2 * n / 100), 0);
		dir_cy = glm::normalize(dir_cy);
		normals_cy.push_back(dir_cy.x), normals_cy.push_back(dir_cy.y), normals_cy.push_back(dir_cy.z);
		dir_cy = glm::vec3(R * cos(M_PI * 2 * n / 100), R * sin(M_PI * 2 * n / 100), 0);
		dir_cy = glm::normalize(dir_cy);
		normals_cy.push_back(dir_cy.x), normals_cy.push_back(dir_cy.y), normals_cy.push_back(dir_cy.z);

		n++;

		circle1.push_back(R * cos(M_PI * 2 * n / 100));
		circle1.push_back(R * sin(M_PI * 2 * n / 100));
		circle1.push_back(cir_1.z);
		num_cir1++;
		normals.push_back(dir_cir1.x), normals.push_back(dir_cir1.y), normals.push_back(dir_cir1.z);

		circle2.push_back(R * cos(M_PI * 2 * n / 100));
		circle2.push_back(R * sin(M_PI * 2 * n / 100));
		circle2.push_back(cir_2.z);
		num_cir2++;
		normals_2.push_back(dir_cir2.x), normals_2.push_back(dir_cir2.y), normals_2.push_back(dir_cir2.z);

		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_1.z);
		num_cy++;
		dir_cy = glm::vec3(R * cos(M_PI * 2 * n / 100), R * sin(M_PI * 2 * n / 100), 0);
		dir_cy = glm::normalize(dir_cy);
		normals_cy.push_back(dir_cy.x), normals_cy.push_back(dir_cy.y), normals_cy.push_back(dir_cy.z);

		cylinder.push_back(temp_1.x);
		cylinder.push_back(temp_1.y);
		cylinder.push_back(cir_2.z);
		num_cy++;
		dir_cy = glm::vec3(temp_1.x, temp_1.y, 0);
		dir_cy = glm::normalize(dir_cy);
		normals_cy.push_back(dir_cy.x), normals_cy.push_back(dir_cy.y), normals_cy.push_back(dir_cy.z);

		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_1.z);
		num_cy++;
		dir_cy = glm::vec3(R * cos(M_PI * 2 * n / 100), R * sin(M_PI * 2 * n / 100), 0);
		dir_cy = glm::normalize(dir_cy);
		normals_cy.push_back(dir_cy.x), normals_cy.push_back(dir_cy.y), normals_cy.push_back(dir_cy.z);

		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_2.z);
		num_cy++;
		dir_cy = glm::vec3(R * cos(M_PI * 2 * n / 100), R * sin(M_PI * 2 * n / 100), 0);
		dir_cy = glm::normalize(dir_cy);
		normals_cy.push_back(dir_cy.x), normals_cy.push_back(dir_cy.y), normals_cy.push_back(dir_cy.z);

		n--;
	}

	circle1.insert(circle1.end(), circle2.begin(), circle2.end());
	circle1.insert(circle1.end(), cylinder.begin(), cylinder.end());

	normals.insert(normals.end(), normals_2.begin(), normals_2.end());
	normals.insert(normals.end(), normals_cy.begin(), normals_cy.end());
}

void setupVertices(void) {
	std::vector<float> circle1;
	std::vector<float> normals;

	glm::vec3 col_1;
	col_1.x = 0.0f;  col_1.y = 0.6f;  col_1.z = 1.0f;

	glm::vec3 cir_1;
	cir_1.x = 0.0f;  cir_1.y = 0.0f;  cir_1.z = 2.0f;
	glm::vec3 cir_2;
	cir_2.x = 0.0f;  cir_2.y = 0.0f;  cir_2.z = -2.0f;

	int R = 2.0f;
	drawCylinder(circle1, normals, cir_1, cir_2, R);

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, circle1.size() * 4, &circle1[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, &normals[0], GL_STATIC_DRAW);

	//球
	normals.clear();
	drawSphere(normals); //生成球的顶点

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	GLuint element_buffer_object;//EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, &normals[0], GL_STATIC_DRAW);

	//圆锥
	normals.clear();
	circle1.clear();
	glm::vec3 cen;  cen.x = 0, cen.y = 0, cen.z = 0.0;
	glm::vec3 top;  top.x = 0.0, top.y = 2.2, top.z = 1.5;
	drawTail(cen, 2.0, top, circle1, normals, num_tail);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, circle1.size() * 4, &circle1[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, &normals[0], GL_STATIC_DRAW);

	//耳朵（棱锥）
	normals.clear();
	vector<float> pyrnode;
	glm::vec3 topear;  topear.x = 0.0, topear.y = 1.0, topear.z = 2.0;
	drawEar(pyrnode, normals, topear);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, pyrnode.size() * 4, &pyrnode[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, &normals[0], GL_STATIC_DRAW);

	normals.clear();
	circle1.clear();
	drawCube(circle1, normals);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, circle1.size() * 4, &circle1[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * 4, &normals[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	pMat = glm::perspective(fov, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	/*
	GLfloat radius = 10.0f;
	GLfloat camX = sin(glfwGetTime()) * radius;
	GLfloat camZ = cos(glfwGetTime()) * radius;
	mvMat = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	*/
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 10.0f;
	setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
	glm::mat4 invTrMat;
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	viewLoc = glGetUniformLocation(renderingProgram, "view_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

	//mvMat = glm::lookAt(glm::vec3(8, 4.5, 1), glm::vec3(6.3, 4.2, 0), glm::vec3(0.5, 1.8, 0));

	mvMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));


	currentLightPos = glm::vec3(lightLoc.x, lightLoc.y, lightLoc.z);
	amt += 0.5f;
	//glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), toRadians(amt), glm::vec3(0.0f, 0.0f, 1.0f));
	//currentLightPos = glm::vec3(rMat * glm::vec4(currentLightPos, 1.0f));
	currentLightPos = glm::vec3(glm::vec4(currentLightPos, 1.0f));
	invTrMat = glm::transpose(glm::inverse(mvStack.top()));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	//set matAmb, matDif, matSpe, matShi

	installLights(vMat);


	//mvStack.pop();

	//绘制身体（圆柱体）
	setMaterialBlue();
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	mvStack.push(mvStack.top());
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 0.7));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glDrawArrays(GL_TRIANGLES, 0, num_cir1 + num_cir2 + num_cy); //绘制
	mvStack.pop();


	//绘制尾巴（圆锥）
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1.4));
	invTrMat = mvStack.top();
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, num_tail);
	mvStack.pop();
	//mvStack.pop();


	//绘制脸（球体）
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1.4));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0); //绘制
	mvStack.pop();


	//绘制耳朵（左）
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.4, -0.4));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(145.0f), glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(-66.0f), glm::vec3(1.0, 0.0, 0.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.6, 0.6, 0.6));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 21); //绘制
	mvStack.pop();

	//绘制耳朵（右）
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.4, -0.4));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(-145.0f), glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(-66.0f), glm::vec3(1.0, 0.0, 0.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.6, 0.6, 0.6));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 21); //绘制
	mvStack.pop();

	//绘制眼睛（左）
	setMaterialBlack();
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.6, 0.2, -2.28));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0); //绘制
	mvStack.pop();

	//绘制眼睛（右）
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-0.6, 0.2, -2.28));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0); //绘制
	mvStack.pop();

	mvStack.pop(); //去除旋转

	//跑步机的长方体
	setMaterialBron();
	//glBindVertexArray(vao[1]);
	mvStack.push(mvStack.top()); invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.9, 1, 0.8));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.4)); invTrMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.4));
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 36); //绘制
	mvStack.pop();

	//左边的棍子
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.3, 7.5, 1.0));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.6, 2.0, 0.09));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 36); //绘制
	mvStack.pop();

	//右边的棍子
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.3, 7.5, -1.6));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.6, 2.0, 0.09));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 36); //绘制
	mvStack.pop();

	//中间的棍子
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -0.9));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	mvStack.top() *= rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.6, 7.5, -1.7));
	invTrMat = mvStack.top();
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.62, 2.0, 0.092));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	invTrMat = glm::transpose(glm::inverse(invTrMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 36); //绘制
	mvStack.pop();

	mvStack.pop();  // the final pop is for the view matrix
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	GLfloat cameraSpeed = 0.05f;
	if (key == GLFW_KEY_W)
		cameraPos += cameraSpeed * cameraFront;
	if (key == GLFW_KEY_S)
		cameraPos -= cameraSpeed * cameraFront;
	if (key == GLFW_KEY_A)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (key == GLFW_KEY_D)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (key == GLFW_KEY_END)  //end键退出程序
		exit(EXIT_SUCCESS);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

//鼠标滚轮的回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void do_movement()
{
	// 摄像机控制
	GLfloat cameraSpeed = 0.01f;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (keys[GLFW_KEY_UP])
		cameraY += 0.1f;
	if (keys[GLFW_KEY_DOWN])
		cameraY -= 0.1f;
	if (keys[GLFW_KEY_RIGHT])
		cameraZ += 0.1f;
	if (keys[GLFW_KEY_LEFT])
		cameraZ -= 0.1f;
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	//pMat = glm::perspective(fov, aspect, 0.1f, 1000.0f);
	//mvMat = glm::lookAt(glm::vec3(1.5, 0.9, 2), glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0));
}

int main(void)
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 600, "Stack", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
		do_movement();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}