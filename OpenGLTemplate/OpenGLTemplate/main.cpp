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

#define numVAOs 1
#define numVBOs 4

float cameraX, cameraY, cameraZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// variable allocation for display
GLuint mvLoc, projLoc, vColorLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

int num_cir1 = 0, num_cir2 = 0, num_cy = 0, num_cir;
int num_tail = 0;


stack<glm::mat4> mvStack;

void drawEar(std::vector<float> &nodes, int &num_inner) {
	nodes.push_back();
}

void drawCone(glm::vec3 center, float R, float dp, std::vector<float> &nodes, int &num_inner) {
	//R for the redium of the circle of the cone, 
	//dp is the tall of this cone
	for (int i = 0; i < 100; i++) {
		nodes.push_back(center.x);
		nodes.push_back(center.y);
		nodes.push_back(center.z + dp);
		num_inner++;

		glm::vec3 temp;
		temp.x = R * cos(M_PI * 2 * i / 100);
		temp.y = R * sin(M_PI * 2 * i / 100);
		temp.z = center.z;
		nodes.push_back(temp.x);
		nodes.push_back(temp.y);
		nodes.push_back(temp.z);
		num_inner++;

		i++;
		temp.x = R * cos(M_PI * 2 * i / 100);
		temp.y = R * sin(M_PI * 2 * i / 100);
		temp.z = center.z;
		nodes.push_back(temp.x);
		nodes.push_back(temp.y);
		nodes.push_back(temp.z);
		num_inner++;
		i--;
	}
}

void setcolor(vector<float> &color, glm::vec3 col, int num_inner) {
	for (int i = 0; i < num_inner; i++) {
		color.push_back(col.x);
		color.push_back(col.y);
		color.push_back(col.z);
		color.push_back(1.0f);
	}
}

void drawTail(glm::vec3 center, float R, glm::vec3 top, std::vector<float> &nodes, int &num_inner) {
	//center底部圆心
	//top顶点
	//R底部半径
	//num_inner--传全局变量的num_tail
	for (int i = 0; i < 100; i++) {
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

		i++;
		temp.x = R * cos(M_PI * 2 * i / 100) + center.x;
		temp.y = R * sin(M_PI * 2 * i / 100) + center.y;
		temp.z = center.z;
		nodes.push_back(temp.x);
		nodes.push_back(temp.y);
		nodes.push_back(temp.z);
		num_inner++;
		i--;
	}
}

void setupVertices(void) {
	std::vector<float> circle1;
	std::vector<float> circle2;
	std::vector<float> cylinder;

	std::vector<float> color;
	std::vector<float> color_cir;

	glm::vec3 col_1;
	col_1.x = 0.0f;  col_1.y = 0.6f;  col_1.z = 1.0f;

	glm::vec3 cir_1;
	cir_1.x = 0.0f; cir_1.y = 0.0f;  cir_1.z = 2.0f;

	glm::vec3 cir_2;
	cir_2.x = 0.0f;  cir_2.y = 0.0f;  cir_2.z = -2.0f;

	int R = 2.0f;
	for (int n = 0; n < 100; n++) {
		circle1.push_back(cir_1.x);
		circle1.push_back(cir_1.y);
		circle1.push_back(cir_1.z);
		num_cir1++;

		circle2.push_back(cir_2.x);
		circle2.push_back(cir_2.y);
		circle2.push_back(cir_2.z);
		num_cir2++;

		circle1.push_back(R * cos(M_PI * 2 * n / 100));
		circle1.push_back(R * sin(M_PI * 2 * n / 100));
		circle1.push_back(cir_1.z);
		num_cir1++;

		circle2.push_back(R * cos(M_PI * 2 * n / 100));
		circle2.push_back(R * sin(M_PI * 2 * n / 100));
		circle2.push_back(cir_2.z);
		num_cir2++;

		glm::vec3 temp_1;
		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_1.z);
		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_2.z);
		temp_1.x = R * cos(M_PI * 2 * n / 100);
		temp_1.y = R * sin(M_PI * 2 * n / 100);
		num_cy += 2;

		n++;

		circle1.push_back(R * cos(M_PI * 2 * n / 100));
		circle1.push_back(R * sin(M_PI * 2 * n / 100));
		circle1.push_back(cir_1.z);
		num_cir1++;

		circle2.push_back(R * cos(M_PI * 2 * n / 100));
		circle2.push_back(R * sin(M_PI * 2 * n / 100));
		circle2.push_back(cir_2.z);
		num_cir2++;

		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_1.z);
		num_cy++;

		cylinder.push_back(temp_1.x);
		cylinder.push_back(temp_1.y);
		cylinder.push_back(cir_2.z);
		num_cy++;

		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_1.z);
		num_cy++;

		cylinder.push_back(R * cos(M_PI * 2 * n / 100));
		cylinder.push_back(R * sin(M_PI * 2 * n / 100));
		cylinder.push_back(cir_2.z);
		num_cy++;

		n--;
	}

	for (int i = 0; i < num_cir1; i++) {
		color_cir.push_back(col_1.x);
		color_cir.push_back(col_1.y);
		color_cir.push_back(col_1.z);
		color_cir.push_back(1.0f);
	}
	for (int i = 0; i < num_cy; i++) {
		color.push_back(col_1.x);
		color.push_back(col_1.y);
		color.push_back(col_1.z);
		color.push_back(1.0f);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	circle1.insert(circle1.end(), circle2.begin(), circle2.end());
	color_cir.insert(color_cir.end(), color_cir.begin(), color_cir.end());

	circle1.insert(circle1.end(), cylinder.begin(), cylinder.end());
	color_cir.insert(color_cir.end(), color.begin(), color.end());

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, circle1.size() * 4, &circle1[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, color_cir.size() * 4, &color_cir[0], GL_STATIC_DRAW);
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);

	circle1.clear();
	glm::vec3 cen;  cen.x = 0, cen.y = 0, cen.z = 2.0;
	glm::vec3 top;  top.x = 0.0, top.y = 2.0, top.z = 3.0;
	drawTail(cen, 2.0, top, circle1, num_tail);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, circle1.size() * 4, &circle1[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 12.0f;
	setupVertices();
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	mvStack.push(mvStack.top());
	mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, num_cir1 + num_cir2 + num_cy); //绘制

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, num_cir1 + num_cir2 + num_cy); //绘制

	mvStack.pop();  // the final pop is for the view matrix
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
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

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}