#ifndef AXBOPENGLWIDGET_H
#define AXBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class AXBOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit AXBOpenGLWidget(QWidget *parent = nullptr);
    enum Shape{None,Rect,Circle,Triangle};
    void drawShape(Shape shape);
    void setWireframe(bool wireframe);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
private:
    unsigned int VAO,VBO,shaderProgram,EBO;
    Shape m_shape;
};

#endif // AXBOPENGLWIDGET_H
