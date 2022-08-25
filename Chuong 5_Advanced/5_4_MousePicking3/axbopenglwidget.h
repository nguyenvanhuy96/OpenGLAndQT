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
#include "model.h"
#include <QVector3D>
class AXBOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    struct ModelInfo{
        Model *model;
        QVector3D worldPos;
        float pitch;
        float yaw;
        float roll;
        bool isSelected;
        QString name;
    };
    explicit AXBOpenGLWidget(QWidget *parent = nullptr);
    ~AXBOpenGLWidget();
signals:
    void MousePickingPos(QVector3D pos);
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
    QVector4D worldPosFromViewPort(int xOffset, int yOffset);
private:
    Camera m_camera;
    QOpenGLShaderProgram shaderProgram, lightshaderProgram;
    QTimer m_timer;
    QMatrix4x4 projection,view,model,modelLight;
    QOpenGLTexture *texDiffuse;
    QOpenGLTexture *texSpecular;
    QMap<QString,ModelInfo> m_Models;
    bool isActiveEditModel;

};

#endif // AXBOPENGLWIDGET_H
