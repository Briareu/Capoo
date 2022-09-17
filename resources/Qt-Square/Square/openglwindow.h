#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class openglwindow : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit openglwindow(QWidget *parent = nullptr);
    ~openglwindow();

protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w,int h) Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;

private:
    bool createShader();
    QOpenGLShaderProgram shader;
    QOpenGLBuffer VBO,EBO;
    QOpenGLVertexArrayObject VAO;
    int size;

signals:

};

#endif // OPENGLWINDOW_H
