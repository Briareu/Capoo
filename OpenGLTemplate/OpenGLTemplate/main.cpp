#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "Utils.h"
#include <math.h>
using namespace std;

#define numVAOs 12 //脸，眼睛*2，耳朵*2
#define numVBOs 24

#define M_PI acos(-1)

//全局变量
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

const int ELLIPSE_NUM_POINTS = 100;
const int LINE_NUM_POINTS = 2;

GLuint vColorLoc, vPositionLoc;

/*
// 根据角度生成颜色
float generateAngleColor(double angle)
{
	return 1.0 / (2 * M_PI) * angle;
}*/


// 计算椭圆/圆上的点
glm::vec2 getEllipseVertex(glm::vec2 center, double scale, double verticalScale, double angle)
{
	glm::vec2 vertex(sin(angle), cos(angle));
	vertex *= scale;
	vertex.y *= verticalScale;
	vertex += center;
	return vertex;
}

// 获得椭圆/圆的每个顶点
void generateEllipsePoints(glm::vec2 vertices[], glm::vec3 colors[], glm::vec3 decolor, int startVertexIndex, int numPoints,
	glm::vec2 center, double scale, double verticalScale)
{
	double angleIncrement = (2 * M_PI) / numPoints;
	double currentAngle = M_PI / 2;

	for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
		vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
		if (verticalScale == 1.0) { //圆形
			colors[i] = decolor;
		}
		else { //椭圆
			colors[i] = decolor;
		}
		currentAngle += angleIncrement;
	}
}

//导入着色器，初始化窗口
void init(GLFWwindow* window)
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	glGenVertexArrays(numVAOs, vao);
	glGenBuffers(numVBOs, vbo);


	//生成椭圆（脸）
	glm::vec2 ellipse_vertices[ELLIPSE_NUM_POINTS];
	glm::vec3 ellipse_colors[ELLIPSE_NUM_POINTS];
	glm::vec3 decolors = glm::vec3(0.0, 0.6, 1.0);
	glm::vec2 ellipsecenter(0, 0);
	generateEllipsePoints(ellipse_vertices, ellipse_colors, decolors, 0, ELLIPSE_NUM_POINTS, ellipsecenter, 0.6, 0.8);

	//生成椭圆（眼睛）
	//右眼
	glm::vec2 ellipse_vertices1[ELLIPSE_NUM_POINTS];
	glm::vec3 ellipse_colors1[ELLIPSE_NUM_POINTS];
	glm::vec3 decolors1 = glm::vec3(0.0, 0.0, 0.0);
	glm::vec2 ellipsecenter1(0.35, 0.04);
	generateEllipsePoints(ellipse_vertices1, ellipse_colors1, decolors1, 0, ELLIPSE_NUM_POINTS, ellipsecenter1, 0.12, 1.2);
	//左眼
	glm::vec2 ellipse_vertices2[ELLIPSE_NUM_POINTS];
	glm::vec3 ellipse_colors2[ELLIPSE_NUM_POINTS];
	glm::vec3 decolors2 = glm::vec3(0.0, 0.0, 0.0);
	glm::vec2 ellipsecenter2(-0.35, 0.04);
	generateEllipsePoints(ellipse_vertices2, ellipse_colors2, decolors2, 0, ELLIPSE_NUM_POINTS, ellipsecenter2, 0.12, 1.2);

	// 初始化椭圆（脸）
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_vertices), ellipse_vertices, GL_STATIC_DRAW);
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glEnableVertexAttribArray(vPositionLoc);
	glVertexAttribPointer(
		vPositionLoc,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_colors), ellipse_colors, GL_STATIC_DRAW);
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glEnableVertexAttribArray(vColorLoc);
	glVertexAttribPointer(
		vColorLoc,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		0);
	glEnableVertexAttribArray(vColorLoc);

	// 初始化椭圆（眼睛）
	//右眼
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glBindVertexArray(vao[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_vertices1), ellipse_vertices1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vPositionLoc);
	glVertexAttribPointer(
		vPositionLoc,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		0);

	glGenBuffers(1, &vbo[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_colors1), ellipse_colors1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vColorLoc);
	glVertexAttribPointer(
		vColorLoc,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		0);
	glEnableVertexAttribArray(vColorLoc);

	//左眼
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glBindVertexArray(vao[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_vertices2), ellipse_vertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vPositionLoc);
	glVertexAttribPointer(
		vPositionLoc,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		0);

	glGenBuffers(1, &vbo[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ellipse_colors2), ellipse_colors2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vColorLoc);
	glVertexAttribPointer(
		vColorLoc,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		0);
	glEnableVertexAttribArray(vColorLoc);

	//耳朵
	float vertices[27] = {
		0.0f, 0.4f, 0.0f,
		0.17f,0.55f,0.0f,
		0.3f, 0.65f, 0.0f,
		0.32f, 0.66f, 0.0f,
		0.35f, 0.65f, 0.0f,
		0.39f,0.6f,0.0f,
		0.49f,0.4f,0.0f,
		0.54f,0.257f,0.0f,
		0.592f, 0.047f, 0.0f
	};
	float vertexColors[36] = {
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f
	};
	//绑定当前VAO
	glBindVertexArray(vao[3]);
	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);


	float vertices2[27] = {
		0.0f, 0.4f, 0.0f,
		-0.17f,0.55f,0.0f,
		-0.3f, 0.65f, 0.0f,
		-0.32f, 0.66f, 0.0f,
		-0.35f, 0.65f, 0.0f,
		-0.39f,0.6f,0.0f,
		-0.49f,0.4f,0.0f,
		-0.54f,0.257f,0.0f,
		-0.592f, 0.047f, 0.0f
	};
	float vertexColors1[36] = {
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f,
	   0.0f,0.6f,1.0f,1.0f
	};
	//绑定当前VAO
	glBindVertexArray(vao[4]);
	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors1), vertexColors1, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);

	//嘴
	float vertex_mouth[26 * 3] = {
		-0.14f, -0.02f, 0.0f,
		-0.14f, -0.02f, 0.0f,
		-0.13f, -0.02f, 0.0f,
		-0.125f, -0.02f, 0.0f,
		-0.12f, -0.05f, 0.0f,
		-0.11f, -0.055f, 0.0f,
		-0.10f, -0.065f, 0.0f,
		-0.09f, -0.07f, 0.0f,
		-0.07f, -0.07f, 0.0f,
		-0.05f, -0.07f, 0.0f,
		-0.02f, -0.05f, 0.0f,
		-0.01f, -0.03f, 0.0f,
		0.0f, -0.02f, 0.0f,
		0.0f, -0.02f, 0.0f,
		0.01f, -0.03f, 0.0f,
		0.02f, -0.05f, 0.0f,
		0.05f, -0.07f, 0.0f,
		0.07f, -0.07f, 0.0f,
		0.09f, -0.07f, 0.0f,
		0.10f, -0.065f, 0.0f,
		0.11f, -0.055f, 0.0f,
		0.12f, -0.05f, 0.0f,
		0.125f, -0.03f, 0.0f,
		0.13f, -0.03f, 0.0f,
		0.14f, -0.02f, 0.0f,
		0.14f, -0.02f, 0.0f
	};
	float mouth_color[26 * 4] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_mouth), vertex_mouth, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouth_color), mouth_color, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);

	//身体描边
	float vertex_circle[100 * 3];
	float vertex_cir_color[100 * 4];
	for (int i = 0; i < 100; i++) {
		vertex_circle[i * 3] = 0.6 * cos(M_PI * 2 * i / 100);
		vertex_circle[i * 3 + 1] = 0.48 * sin(M_PI * 2 * i / 100);
		vertex_circle[i * 3 + 2] = 0;

		vertex_cir_color[i * 4] = 0.0f;
		vertex_cir_color[i * 4 + 1] = 0.0f;
		vertex_cir_color[i * 4 + 2] = 0.0f;
		vertex_cir_color[i * 4 + 3] = 0.0f;
	}
	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_circle), vertex_circle, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_cir_color), vertex_cir_color, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);

	//花
	float circle1[50 * 3];
	float circle1_col[50 * 4];
	//circle(circle1, 0.7, 0.7, 0.2, 50);
	//circlecolor(circle1_col, 1.0f, 0.0f, 0.0f, 50);
	for (int i = 0; i < 50; i++) {
		circle1[i * 3] = 0.10 * cos(2 * M_PI * i / 50) + 0.77;
		circle1[i * 3 + 1] = 0.10 * sin(2 * M_PI * i / 50) + 0.45;
		circle1[i * 3 + 2] = 0;

		circle1_col[i * 4] = 1.0f;
		circle1_col[i * 4 + 1] = 0.84f;
		circle1_col[i * 4 + 2] = 0.0f;
		circle1_col[i * 4 + 3] = 1.0f;
	}
	glBindVertexArray(vao[7]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle1), circle1, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle1_col), circle1_col, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);


	float eye[3 * 3]{
		0.707f, 0.48f, 0.0f,
		0.742f, 0.46f, 0.0f,
		0.706f, 0.441f, 0.0f
	};
	float eye_col[3 * 4]{
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	glBindVertexArray(vao[8]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[16]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(eye), eye, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[17]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(eye_col), eye_col, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);


	float eye1[3 * 3]{
		0.833f, 0.48f, 0.0f,
		0.798f, 0.46f, 0.0f,
		0.834f, 0.441f, 0.0f
	};
	float eye_col1[3 * 4]{
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	glBindVertexArray(vao[9]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[18]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(eye1), eye1, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[19]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(eye_col1), eye_col1, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);


	float mouth_ch[5 * 3]{
		0.77f, 0.46f, 0.0f,
		0.753f, 0.44f, 0.0f,
		0.77f, 0.42f, 0.0f,
		0.787f, 0.44f, 0.0f,
		0.77f, 0.46f, 0.0f
	};
	float mouth_ch_col[5 * 4]{
		1.0f, 0.72f, 0.058f, 0.0f,
		1.0f, 0.72f, 0.058f, 0.0f,
		1.0f, 0.72f, 0.058f, 0.0f,
		1.0f, 0.72f, 0.058f, 0.0f,
		1.0f, 0.72f, 0.058f, 0.0f
	};
	glBindVertexArray(vao[10]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[20]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouth_ch), mouth_ch, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[21]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouth_ch_col), mouth_ch_col, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);


	float mouth_ch_col1[4 * 4]{
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	glBindVertexArray(vao[11]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[22]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouth_ch), mouth_ch, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
	glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPositionLoc);

	// Load the data into the GPU  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[23]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouth_ch_col1), mouth_ch_col1, GL_STATIC_DRAW);
	//将VBO关联给顶点着色器中相应的顶点属性
	vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
	glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColorLoc);
}
//函数绘制
void display(GLFWwindow* window, double currentTime)
{
	glUseProgram(renderingProgram);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	//绘制椭圆
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, ELLIPSE_NUM_POINTS);

	glBindVertexArray(vao[3]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 9);

	glBindVertexArray(vao[4]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, ELLIPSE_NUM_POINTS);
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, ELLIPSE_NUM_POINTS);

	glBindVertexArray(vao[5]);
	glLineWidth(9);
	glDrawArrays(GL_LINE_STRIP, 0, 26);

	glBindVertexArray(vao[6]);
	glLineWidth(10);
	glDrawArrays(GL_LINE_LOOP, 0, 100);

	glBindVertexArray(vao[7]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 50);

	glBindVertexArray(vao[8]);
	glLineWidth(4);
	glDrawArrays(GL_LINE_STRIP, 0, 3);

	glBindVertexArray(vao[9]);
	glLineWidth(4);
	glDrawArrays(GL_LINE_STRIP, 0, 3);

	glBindVertexArray(vao[10]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

	glBindVertexArray(vao[11]);
	glLineWidth(4);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}

int main(void)
{
	//glfw初始化
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	//窗口版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 800, "Capoo", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)//glew初始化
	{
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	//导入着色器，创建和绑定VAO和VBO
	init(window);

	//事件循环
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