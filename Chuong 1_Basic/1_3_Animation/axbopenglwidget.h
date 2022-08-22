#ifndef AXBOPENGLWIDGET_H
#define AXBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <QTime>

class AXBOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit AXBOpenGLWidget(QWidget *parent = nullptr);
    enum Shape{None,Rect,Circle,Triangle};
    void drawShape(Shape shape);
    void setWireframe(bool wireframe);
private slots:
        void timeoutFunc();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
private:
    unsigned int VAO,VBO,shaderProgram,EBO;
    Shape m_shape;
    QTimer m_Timer;
    QTime m_Time;
};

#endif // AXBOPENGLWIDGET_H
