#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 1

//ȫ�ֱ���
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint vColorLoc, vPositionLoc;

//������ɫ������ʼ������
void init(GLFWwindow* window)
{
	//���롢������ɫ������
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	float vertices[9] = {
		-0.5f, -0.5f,0.0f,
		0.5f,-0.5f, 0.0f,
		0.0f, 0.5f,0.0f
	};

	glGenVertexArrays(numVAOs, vao);
	glGenBuffers(numVBOs, vbo);

	// Associate out shader variables with our data buffer    
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");

	//�󶨵�ǰVAO
	glBindVertexArray(vao[0]);
	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��VBO������������ɫ������Ӧ�Ķ�������
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

}
//��������
void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);

	glClear(GL_COLOR_BUFFER_BIT);

	//����ģ��
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main(void)
{
	//glfw��ʼ��
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	//���ڰ汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //�ΰ汾��

	//��������
	GLFWwindow* window = glfwCreateWindow(800, 800, "simple", NULL, NULL);
	glfwMakeContextCurrent(window);
	//���û���ˢ��ʱ��
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)//glew��ʼ��
	{
		exit(EXIT_FAILURE);
	}

	//������ɫ���������Ͱ�VAO��VBO
	init(window);

	//�¼�ѭ�������������¼�
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		display(window, glfwGetTime()); //���ƺ���������
		glfwSwapBuffers(window); //������ɫ����
		glfwPollEvents(); // �����û�д���ʲô�¼�������������롢����ƶ��ȣ�
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}