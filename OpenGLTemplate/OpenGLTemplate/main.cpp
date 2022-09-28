#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "Utils.h"
#include <math.h>
using namespace std;

#define numVAOs 4
#define numVBOs 5

#define M_PI acos(-1)

//ȫ�ֱ���
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

const int ELLIPSE_NUM_POINTS = 100;
const int LINE_NUM_POINTS = 2;

GLuint vColorLoc, vPositionLoc;


// ���ݽǶ�������ɫ
float generateAngleColor(double angle)
{
	return 1.0 / (2 * M_PI) * angle;
}

// ������Բ/Բ�ϵĵ�
glm::vec2 getEllipseVertex(glm::vec2 center, double scale, double verticalScale, double angle)
{
	glm::vec2 vertex(sin(angle), cos(angle));
	vertex *= scale;
	vertex.y *= verticalScale;
	vertex += center;
	return vertex;
}

// �����Բ/Բ��ÿ������
void generateEllipsePoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex, int numPoints,
	glm::vec2 center, double scale, double verticalScale)
{
	double angleIncrement = (2 * M_PI) / numPoints;
	double currentAngle = M_PI / 2;

	for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
		vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
		if (verticalScale == 1.0) { //Բ��
			colors[i] = glm::vec3(generateAngleColor(currentAngle), 0.0, 0.0);
		}
		else { //��Բ
			colors[i] = glm::vec3(generateAngleColor(currentAngle), 0.0, 0.0);//RED;
		}
		currentAngle += angleIncrement;
	}
}

//������ɫ������ʼ������
void init(GLFWwindow* window)
{

	// �����ߵĵ�
	glm::vec2 line_vertices[LINE_NUM_POINTS];
	glm::vec3 line_colors[LINE_NUM_POINTS];

	//������Բ
	glm::vec2 ellipse_vertices[ELLIPSE_NUM_POINTS];
	glm::vec3 ellipse_colors[ELLIPSE_NUM_POINTS];
	glm::vec2 ellipsecenter(0, 0);
	generateEllipsePoints(ellipse_vertices, ellipse_colors, 0, ELLIPSE_NUM_POINTS, ellipsecenter, 0.6, 0.9);

	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	// ��ʼ����Բ
	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_vertices), ellipse_vertices, GL_STATIC_DRAW);
	GLuint location = glGetAttribLocation(renderingProgram, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_colors), ellipse_colors, GL_STATIC_DRAW);
	GLuint cLocation = glGetAttribLocation(renderingProgram, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		0);


	float vertices[18] = {
		-0.25f, 0.4f, 0.0f,
		0.45f, 0.55f, 0.0f,
		0.5f, 0.6f, 0.0f,
		0.53f, 0.59f, 0.0f,
		0.55f,0.55f,0.0f,
		0.6f, 0.0f, 0.0f
	};
	glGenVertexArrays(1, &vao[2]);
	glGenBuffers(1, &vbo[3]);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);

	float vertices2[18] = {
		0.25f, 0.4f, 0.0f,
		-0.45f, 0.55f, 0.0f,
		-0.5f, 0.6f, 0.0f,
		-0.53f, 0.59f, 0.0f,
		-0.55f,0.55f,0.0f,
		-0.6f, 0.0f, 0.0f
	};
	glGenVertexArrays(1, &vao[3]);
	glGenBuffers(1, &vbo[4]);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
}
//��������
void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	//������Բ
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, ELLIPSE_NUM_POINTS);


	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);


	glBindVertexArray(vao[3]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
}

int main(void)
{
	//glfw��ʼ��
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	//���ڰ汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//��������
	GLFWwindow* window = glfwCreateWindow(800, 800, "A triangle", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)//glew��ʼ��
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	//������ɫ���������Ͱ�VAO��VBO
	init(window);

	//�¼�ѭ��
	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}