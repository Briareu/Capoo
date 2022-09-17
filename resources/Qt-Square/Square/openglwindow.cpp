#include "openglwindow.h"

openglwindow::openglwindow(QWidget *parent)
    : QOpenGLWidget(parent),VBO(QOpenGLBuffer::VertexBuffer),EBO(QOpenGLBuffer::IndexBuffer)
{

}
openglwindow:: ~openglwindow()
{
    makeCurrent();
    VAO.destroy();
    VBO.destroy();
    EBO.destroy();
    doneCurrent();
}

float vertices[]={
-0.5f,-0.5f,0.0f,
0.5f,-0.5f,0.0f,
0.5f,0.5f,0.0f,
-0.5f,0.5f,0.0f
};

unsigned int indices[]={//索引从0开始
    0,1,3,//第一个三角形
    1,2,3//第二个三角形
};

void openglwindow::initializeGL()
{
    this->initializeOpenGLFunctions();
    createShader();
    size = sizeof(vertices);
    QOpenGLVertexArrayObject::Binder vaoBind(&VAO);
    VBO.create();
    VBO.bind();
    VBO.allocate(vertices, size);
    EBO.create();
    EBO.bind();
    EBO.allocate(indices, sizeof(indices));
    {
        // position attribute
        int attr = -1;
        attr = shader.attributeLocation("aPos");
        shader.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
        shader.enableAttributeArray(attr);
    }
    VBO.release();
}

void openglwindow::resizeGL(int w, int h)
{

}

void openglwindow::paintGL()
{
//    glClearColor(0.2f,0.3f,0.3f,1.0f);//设置gl清空状态
    glClear(GL_COLOR_BUFFER_BIT);//使用此状态

    shader.bind();
    QOpenGLVertexArrayObject::Binder vaoBind(&VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    shader.release();
}

bool openglwindow::createShader()
{
    bool success = shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/shape.vert");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shader.log();
        return success;
    }

    success = shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/shape.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shader.log();
        return success;
    }

    success = shader.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << shader.log();
    }
    return success;
}
