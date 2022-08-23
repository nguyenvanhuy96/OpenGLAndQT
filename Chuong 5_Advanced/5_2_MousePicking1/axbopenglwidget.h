#ifndef AXBOPENGLWIDGET_H
#define AXBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include "camera.h"
#include <QPointF>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLTexture>
#include "mesh.h"


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
    Camera m_camera;
    QOpenGLShaderProgram shaderProgram, lightshaderProgram;
    QTimer m_timer;
    QMatrix4x4 projection,view,model,modelLight;
    QOpenGLTexture *texDiffuse;
    QOpenGLTexture *texSpecular;
    Mesh *cubeMesh;
    Mesh *planeMesh;
    Mesh * processMesh(float *vertices, int size , unsigned int textureId);
};

#endif // AXBOPENGLWIDGET_H
