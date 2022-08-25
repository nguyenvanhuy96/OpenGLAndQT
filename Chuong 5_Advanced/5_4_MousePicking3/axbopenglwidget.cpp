#include "axbopenglwidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>

#include <QVector4D>

QPoint lastPos(0,0);
int perFrame = 100;// delta time / frame
float _near = 0.1f;
float _far = 100.0f;
AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    /*
        Cho phép kích hoạt sử dụng event keypress
        nếu khong có dòng này mà vẫn có code
        event key press thì nó cũng không chạy được
     */
    setFocusPolicy(Qt::StrongFocus);
    /* Cho phép kích hoạt sự kiện liên quan đến mouse*/
    //setMouseTracking(true);
    m_camera= Camera(QVector3D(0.0f, 1.0f, 3.0f));

    m_timer.setInterval(perFrame);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_TimeOut()));
    m_timer.start();

    // Ban đầu chưa select đối tượng thì chế độ active model sẽ là false
    isActiveEditModel = false;
}

AXBOpenGLWidget::~AXBOpenGLWidget()
{
    for(auto inter=m_Models.begin(); inter!=m_Models.end(); inter++){
        ModelInfo *modelInfo = &inter.value();
        delete  modelInfo->model;
    }
}

void AXBOpenGLWidget::on_TimeOut()
{
    update();
}

void AXBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    // shader
    bool success;
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Resources/Shaders/obj.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Resources/Shaders/obj.frag");
    shaderProgram.link();
    success = shaderProgram.link();
    if(!success)
        qDebug() << "Error shader: " << shaderProgram.log();

    lightshaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/Resources/Shaders/light.vert");
    lightshaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/Resources/Shaders/light.frag");
    lightshaderProgram.link();
    success = lightshaderProgram.link();
    if(!success)
        qDebug() << "Error shader: " << lightshaderProgram.log();

    // Texture
    texDiffuse = new QOpenGLTexture(QImage(":/Images/Resources/Images/diffuseMapTex.png").mirrored());
    texSpecular = new QOpenGLTexture(QImage(":/Images/Resources/Images/co.png").mirrored());

    // Load model
    auto m_model= new Model(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>(),
                            "C:/Users/HUY NGUYEN/Desktop/OpenGL vs QT/OpenGLAndQT/Chuong 4_Assimp/backpack/backpack.obj");

    auto m_model2= new Model(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>(),
                             "C:/Users/HUY NGUYEN/Desktop/OpenGL vs QT/OpenGLAndQT/Chuong 4_Assimp/backpack/backpack.obj");

    // đẩy models vào đối tượng m_Models để quản lý dữ liệu
    m_Models["cube" + QString::number(1)]=ModelInfo{m_model,QVector3D(0.0f,0.0f,0.0f),0.0f,0.0f,0.0f, false,QString::fromUtf8( "Hình hộp 1")};
    m_Models["cube" + QString::number(2)]=ModelInfo{m_model2,QVector3D(5.0f,0.0f,6.0f),0.0f,0.0f,0.0f, false,QString::fromUtf8( "Hình hộp 2")};

}

void AXBOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

QVector3D material_ambient= QVector3D( 1.0f, 0.5f, 0.31f);
//QVector3D material_diffuse=QVector3D(1.0f, 0.5f, 0.31f);
//QVector3D material_specular=QVector3D(0.5f, 0.5f, 0.5f);

QVector3D light_ambient= QVector3D(0.9f, 0.9f, 0.9f);
QVector3D light_diffuse= QVector3D(0.5f, 0.5f, 0.5f);
QVector3D light_specular= QVector3D(1.0f, 1.0f, 1.0f);
QVector3D light_color = QVector3D(1.0f, 1.0f,0.0f);
QVector3D light_pos= QVector3D(0.0f, -1.0f, 5.0f);

void AXBOpenGLWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    projection.setToIdentity();
    projection.perspective(m_camera.Zoom, (float)width()/height(),_near, _far);
    view.setToIdentity();
    view= m_camera.GetViewMatrix();
    model.setToIdentity();
    //model.rotate(10, QVector3D(1.0f, 1.0f, 0.0f));

    shaderProgram.bind();
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", view);
    shaderProgram.setUniformValue("model", model);
    // material
    shaderProgram.setUniformValue("material.ambient", material_ambient);
    shaderProgram.setUniformValue("material.shininess", 32.0f);
    // view pos
    shaderProgram.setUniformValue("viewPos", m_camera.Position);
    // light
    shaderProgram.setUniformValue("light.ambient",light_ambient);
    shaderProgram.setUniformValue("light.diffuse", light_diffuse);
    shaderProgram.setUniformValue("light.specular", light_specular);
    shaderProgram.setUniformValue("light.lightColor", light_color);
    shaderProgram.setUniformValue("light.lightPos", light_pos);

    // draw models
    foreach (auto modelInfo, m_Models) {
        model.setToIdentity();
        model.translate(modelInfo.worldPos);
        model.rotate(modelInfo.pitch, QVector3D(1.0f,0.0f,0.0f));
        model.rotate(modelInfo.yaw, QVector3D(0.0f,1.0f,0.0f));
        model.rotate(modelInfo.roll, QVector3D(0.0f,0.0f,1.0f));
        shaderProgram.setUniformValue("model", model);
        modelInfo.model->Draw(shaderProgram);
    }

}

void AXBOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
     makeCurrent();

    lastPos = event->pos();
    if(isActiveEditModel==true) return;
    if(event->buttons() && Qt::LeftButton){
        int screenX= event->pos().x();
        //  event->pos:toạ độ điểm lấy theo điểm gốc là (trên,trái)
        // sử dụng glReadPixels thì toạ độ điểm x,y phải tính từ điểm gốc (dưới,trái).
        int screenY= event->pos().y();
        auto worldPosition = worldPosFromViewPort(screenX,screenY);

        MousePickingPos(QVector3D(worldPosition));
       qDebug()<<event->pos().x() << ","<< event->pos().y()<< "}- " << "WorldPos: " <<
                                    worldPosition.x() << ","<< worldPosition.y() << "," << worldPosition.z();
        // Selection
        for(QMap<QString, ModelInfo>::iterator iter = m_Models.begin(); iter!=m_Models.end();iter++){

            ModelInfo *modelInfo = &iter.value();
            // Tính bán kính của model
            float r = (modelInfo->model->m_maxY - modelInfo->model->m_minY)/2;
            // Tính toán khoảng cách từ điểm click đến điểm position đặt model
            float lenthpick= modelInfo->worldPos.distanceToPoint(QVector3D(worldPosition));
            if(lenthpick<r){
                modelInfo->isSelected= true;
                qDebug() << modelInfo->name <<lenthpick <<"<" << r<< "=" <<  modelInfo->isSelected << "WorldPos: " <<
                            modelInfo->worldPos.x() << ","<< modelInfo->worldPos.y() << "," << modelInfo->worldPos.z();
            }else{
                modelInfo->isSelected= false;
            }
        }
    }
    doneCurrent();
}


void AXBOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    makeCurrent();
    auto posIn= event->pos();
    // reversed since y-coordinates go from bottom to top
    int xoffset = posIn.x() - lastPos.x();
    int yoffset = lastPos.y() - posIn.y();

    if(isActiveEditModel){
        // move đối tượng
        for (auto iter = m_Models.begin(); iter != m_Models.end();iter++) {
            ModelInfo *modelInfo = &iter.value();
            if(modelInfo->isSelected==false) continue;
            modelInfo->worldPos = QVector3D(worldPosFromViewPort(event->pos().x(), event->pos().y()));
        }
    }else {
        if(event->buttons()& Qt::RightButton
                || event->buttons()& Qt::LeftButton
                || event->buttons()& Qt::MiddleButton )

            if(event->buttons()& Qt::RightButton){
                m_camera.ProcessMouseMovement(xoffset, yoffset);
            }else{
                QPoint deltaPos= posIn - lastPos;
                for (auto iter = m_Models.begin(); iter != m_Models.end();iter++) {
                    ModelInfo *modelInfo = &iter.value();
                    if(modelInfo->isSelected==false) continue;
                    if( event->buttons()& Qt::MiddleButton){
                        modelInfo->roll += deltaPos.x();
                    }
                    if( event->buttons()& Qt::LeftButton){
                        modelInfo->yaw += deltaPos.x();
                        modelInfo->pitch  += deltaPos.y();
                    }
                }
            }
    }
    lastPos=posIn;
    update();
    doneCurrent();
}

void AXBOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    m_camera.ProcessMouseScroll( event->angleDelta().y()/120);
    update();
}

void AXBOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float detatime2= 2.5*perFrame/1000;
    switch (event->key()) {
    case Qt::Key_W:m_camera.ProcessKeyboard(FORWARD,detatime2); break;
    case Qt::Key_S:m_camera.ProcessKeyboard(BACKWARD,detatime2); break;
    case Qt::Key_D:m_camera.ProcessKeyboard(RIGHT,detatime2); break;
    case Qt::Key_A:m_camera.ProcessKeyboard(LEFT,detatime2); break;
    case Qt::Key_M:isActiveEditModel=!isActiveEditModel; break;
    }
    update();
}

QVector4D AXBOpenGLWidget::worldPosFromViewPort(int screenX, int screenY)
{
    float winz; // độ sâu của điểm trên viewport khi pick
    //  event->pos:toạ độ điểm lấy theo điểm gốc là (trên,trái)
    // sử dụng glReadPixels thì toạ độ điểm x,y phải tính từ điểm gốc (dưới,trái).
    int screenYTransform= this->height()- screenY;
    glReadPixels(screenX, screenYTransform,1,1, GL_DEPTH_COMPONENT,GL_FLOAT,&winz);

    // Chuyển đổi toạ độ ViewSpace(màn hình) sang toạ độ NDC
    float x= (2.0f*screenX)/this->width()-1.0f;
    float y = 1.0f-(2.0f*screenY)/this->height();
    float z= winz*2.0f -1.0f;

    //if(winz<1.0f){
    float  w= (2.0f * _near*_far)/(_far+_near-z*(_far - _near));
    QVector4D worldPos (x,y,z,1);
    worldPos= w * worldPos;
    worldPos = view.inverted()*projection.inverted()*worldPos;
    //qDebug() << "x: " << worldPos.x() << ",y: " << worldPos.y() << ",z: " <<worldPos.z() << ",w: " << w ;
    //}
    return  worldPos;
}
