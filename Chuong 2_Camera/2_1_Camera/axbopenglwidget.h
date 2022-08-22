#ifndef AXBOPENGLWIDGET_H
#define AXBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include "camera.h"
#include <QPointF>
#include <QOpenGLShaderProgram>
#include <QTimer>
class AXBOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit AXBOpenGLWidget(QWidget *parent = nullptr);

private slots:
    void on_TimeOut();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    // Event
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    unsigned int VAO, VBO;
    Camera m_camera;
    QOpenGLShaderProgram shaderProgram;
    QTimer m_timer;
    QMatrix4x4 projection,view,model;
};

#endif // AXBOPENGLWIDGET_H
